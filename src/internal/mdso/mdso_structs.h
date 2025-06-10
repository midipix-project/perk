#ifndef MDSO_STRUCTS_H
#define MDSO_STRUCTS_H

#ifdef __cplusplus
extern "C" {
#endif

struct mdso_raw_arg_m32 {
	unsigned char	marg_utf8	[0x04];		/* 0x00 */
	unsigned char	marg_utf16	[0x04];		/* 0x04 */
};

struct mdso_raw_arg_m64 {
	unsigned char	marg_utf8	[0x08];		/* 0x00 */
	unsigned char	marg_utf16	[0x08];		/* 0x08 */
};

union mdso_raw_arg {
	struct mdso_raw_arg_m32		marg_m32;
	struct mdso_raw_arg_m64		marg_m64;
};


struct mdso_raw_sym_entry_m32 {
	unsigned char	msym_string	[0x04];		/* 0x00 */
	unsigned char	msym_meta	[0x04];		/* 0x04 */
};

struct mdso_raw_sym_entry_m64 {
	unsigned char	msym_string	[0x08];		/* 0x00 */
	unsigned char	msym_meta	[0x08];		/* 0x08 */
};

union mdso_raw_sym_entry {
	struct mdso_raw_sym_entry_m32	msym_m32;
	struct mdso_raw_sym_entry_m64	msym_m64;
};


struct mdso_raw_meta_record_m32 {
	unsigned char	mrec_base	[0x04];		/* 0x00 */
	unsigned char	mrec_name	[0x04];		/* 0x04 */
	unsigned char	mrec_flags	[0x04];		/* 0x08 */
	unsigned char	mrec_priority	[0x04];		/* 0x0c */
	unsigned char	mrec_nsyms	[0x04];		/* 0x10 */
	unsigned char	mrec_padding	[0x04];		/* 0x14 */
	unsigned char	mrec_hashtbl	[0x04];		/* 0x18 */
	unsigned char	mrec_symtbl	[0x04];		/* 0x1c */
	unsigned char	mrec_fncarg	[0x04];		/* 0x20 */
	unsigned char	mrec_fncargarr	[0x04];		/* 0x24 */
	unsigned char	mrec_fnr1	[0x04];		/* 0x28 */
	unsigned char	mrec_fnr2	[0x04];		/* 0x2c */
};

struct mdso_raw_meta_record_m64 {
	unsigned char	mrec_base	[0x08];		/* 0x00 */
	unsigned char	mrec_name	[0x08];		/* 0x08 */
	unsigned char	mrec_flags	[0x04];		/* 0x10 */
	unsigned char	mrec_priority	[0x04];		/* 0x14 */
	unsigned char	mrec_nsyms	[0x04];		/* 0x18 */
	unsigned char	mrec_padding	[0x04];		/* 0x1c */
	unsigned char	mrec_hashtbl	[0x08];		/* 0x20 */
	unsigned char	mrec_symtbl	[0x08];		/* 0x28 */
	unsigned char	mrec_fncarg	[0x08];		/* 0x30 */
	unsigned char	mrec_fncargarr	[0x08];		/* 0x38 */
	unsigned char	mrec_fnr1	[0x08];		/* 0x40 */
	unsigned char	mrec_fnr2	[0x08];		/* 0x48 */
};

union mdso_raw_meta_record {
	struct mdso_raw_meta_record_m32	mrec_m32;
	struct mdso_raw_meta_record_m64	mrec_m64;
};


#ifdef __cplusplus
}
#endif

#endif
