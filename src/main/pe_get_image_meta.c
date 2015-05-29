#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <perk/perk.h>
#include "perk_impl.h"

static int pe_free_image_meta_impl (struct pe_image_meta * meta, int status)
{
	unsigned i;

	if (!meta) return 0;

	for (i=0; i<meta->summary.num_of_implibs; i++)
		free(meta->idata[i].items);

	free(meta->idata);
	free(meta->sectbl);
	free(meta);

	return status;
}

int pe_free_image_meta (struct pe_image_meta * meta)
{
	return pe_free_image_meta_impl(meta,0);
}

int pe_get_named_section_index (const struct pe_image_meta * m, const char * name)
{
	int i; for (i=0; i<m->coff.num_of_sections; i++)
		if (!(strcmp(name,m->sectbl[i].name)))
			return i;

	return -1;
}

int pe_get_block_section_index (const struct pe_image_meta * m, const struct pe_block * block)
{
	int i;
	uint32_t low,high;

	for (i=0; i<m->coff.num_of_sections; i++) {
		low  = m->sectbl[i].virtual_addr;
		high = low + m->sectbl[i].virtual_size;

		if ((block->rva >= low) && (block->rva + block->size <= high))
			return i;
	}

	return -1;
}

int pe_get_image_meta (const struct pe_raw_image * image, struct pe_image_meta ** meta)
{
	int i,j,s,status;
	struct pe_image_meta * m;
	char * base = image->addr;

	if (!(m = calloc(1,sizeof(*m))))
		return errno;

	m->ados = (struct pe_image_dos_hdr *)base;

	if ((status = (pe_read_dos_header(m->ados,&m->dos))))
		return pe_free_image_meta_impl(m,status);

	m->acoff = (struct pe_coff_file_hdr *)(base + m->dos.dos_lfanew);

	if ((status = (pe_read_coff_header(m->acoff,&m->coff))))
		return pe_free_image_meta_impl(m,status);

	m->aopt = (union pe_opt_hdr *)((char *)m->acoff + sizeof(m->coff));

	if ((status = (pe_read_optional_header(m->aopt,&m->opt))))
		return pe_free_image_meta_impl(m,status);

	m->asectbl = (struct pe_sec_hdr *)((char *)m->aopt  + m->coff.size_of_opt_hdr);

	if (!(m->sectbl = calloc(m->coff.num_of_sections,sizeof(*(m->sectbl)))))
		return pe_free_image_meta_impl(m,status);

	for (i=0; i<m->coff.num_of_sections; i++)
		pe_read_section_header(&m->asectbl[i],&m->sectbl[i]);

	/* .edata */
	i = pe_get_named_section_index(m,".edata");
	s = pe_get_block_section_index(m,&m->opt.dirs.export_tbl);

	if ((i >= 0) && (i != s))
		return pe_free_image_meta_impl(m,PERK_MALFORMED_IMAGE);

	if (i >= 0 ) {
		m->hedata = &m->sectbl[i];
		m->aedata = (struct pe_export_hdr *)(base + m->sectbl[i].ptr_to_raw_data);
	} else if (s >= 0) {
		m->hedata = &m->sectbl[s];
		m->aedata = (struct pe_export_hdr *)(base + m->sectbl[s].ptr_to_raw_data
				+ m->opt.dirs.export_tbl.rva - m->sectbl[s].virtual_addr);
	}

	if (m->aedata)
		pe_read_export_header(m->aedata,&m->edata);

	/* .idata */
	struct pe_import_hdr * 		pidata;
	struct pe_import_lookup_item *	pitem;

	i = pe_get_named_section_index(m,".idata");
	s = pe_get_block_section_index(m,&m->opt.dirs.import_tbl);

	if ((i >= 0) && (i != s))
		return pe_free_image_meta_impl(m,PERK_MALFORMED_IMAGE);

	if (i >= 0 ) {
		m->hidata = &m->sectbl[i];
		m->aidata = (struct pe_import_hdr *)(base + m->sectbl[i].ptr_to_raw_data);
	} else if (s >= 0) {
		m->hidata = &m->sectbl[s];
		m->aidata = (struct pe_import_hdr *)(base + m->sectbl[s].ptr_to_raw_data
				+ m->opt.dirs.import_tbl.rva - m->sectbl[s].virtual_addr);
	}

	if (m->aidata) {
		/* num of implibs */
		for (pidata=m->aidata; pidata->name_rva[0]; pidata++,m->summary.num_of_implibs++);

		/* import headers */
		if (!(m->idata = calloc(m->summary.num_of_implibs,sizeof(*(m->idata)))))
			return pe_free_image_meta_impl(m,status);

		for (i=0; i<m->summary.num_of_implibs; i++) {
			pe_read_import_header(&m->aidata[i],&m->idata[i]);

			m->idata[i].name = base + m->hidata->ptr_to_raw_data
						+ m->idata[i].name_rva - m->hidata->virtual_addr;

			m->idata[i].aitems = (struct pe_import_lookup_item *)(base + m->hidata->ptr_to_raw_data
						+ m->idata[i].import_lookup_tbl_rva - m->hidata->virtual_addr);

			#ifdef PERK_DEVEL
			printf("%s\n",m->idata[i].name);
			#endif

			/* items */
			m->idata[i].count = 0;
			for (pitem=m->idata[i].aitems; *(uint32_t *)pitem->u.hint_name_tbl_rva; pitem++)
				m->idata[i].count++;

			if (!(m->idata[i].items = calloc(m->idata[i].count,sizeof(*(m->idata[i].items)))))
				return pe_free_image_meta_impl(m,status);

			for (j=0; j<m->idata[i].count; j++) {
				if ((status = pe_read_import_lookup_item(
						&(m->idata[i].aitems[j]),
						&(m->idata[i].items[j]),
						m->opt.std.magic)))
					return pe_free_image_meta_impl(m,status);

				switch (m->opt.std.magic) {
					case PE_MAGIC_PE32:
						m->idata[i].items[j].flags = m->idata[i].items[j].u.import_lookup_entry_32;
						break;

					case PE_MAGIC_PE32_PLUS:
						m->idata[i].items[j].flags = (m->idata[i].items[j].u.import_lookup_entry_64 >> 32);
						break;
				}

				struct pe_hint_name_entry * pentry = (struct pe_hint_name_entry *)(base + m->hidata->ptr_to_raw_data
						+ m->idata[i].items[j].u.hint_name_tbl_rva - m->hidata->virtual_addr);

				if (m->idata[i].items[j].flags)
					m->idata[i].items[j].hint = pe_read_short(pentry->hint);
				else {
					m->idata[i].items[j].name = (char *)pentry->name;
					#ifdef PERK_DEVEL
					printf("%s\n",m->idata[i].items[j].name);
					#endif
				}
			}
		}
	}

	/* image */
	m->image.addr = image->addr;
	m->image.size = image->size;

	/* all done */
	*meta = m;
	return 0;
}
