SRCTREE    = .
CFE        = gcc
CC         = $(CROSS_COMPILE)$(CFE)
CFLAGS     = -O0 -g2 -I$(SRCTREE)/include -I$(SRCTREE)/src/internal -D_XOPEN_SOURCE=900
CFLAGS_APP = -DPERK_APP
CFLAGS_OBJ = -DPERK_PRE_ALPHA
CFLAGS_LIB = -fPIC # -DPERK_PRE_ALPHA

OBJS   = perk.o \
	pe_map_raw_image.o \
	pe_get_image_meta.o \
	pe_read_dos_header.o \
	pe_read_coff_header.o \
	pe_read_optional_header.o \
	pe_read_section_header.o \
	pe_read_export_header.o \
	pe_read_import_header.o \
	pe_output_export_symbols.o

all:	app static

app:
	$(CC) $(CFLAGS_APP) $(CFLAGS) -c $(SRCTREE)/src/main/perk.c
	$(CC) $(CFLAGS_APP) $(CFLAGS) -c $(SRCTREE)/src/main/pe_map_raw_image.c
	$(CC) $(CFLAGS_APP) $(CFLAGS) -c $(SRCTREE)/src/main/pe_get_image_meta.c
	$(CC) $(CFLAGS_APP) $(CFLAGS) -c $(SRCTREE)/src/reader/pe_read_dos_header.c
	$(CC) $(CFLAGS_APP) $(CFLAGS) -c $(SRCTREE)/src/reader/pe_read_coff_header.c
	$(CC) $(CFLAGS_APP) $(CFLAGS) -c $(SRCTREE)/src/reader/pe_read_optional_header.c
	$(CC) $(CFLAGS_APP) $(CFLAGS) -c $(SRCTREE)/src/reader/pe_read_section_header.c
	$(CC) $(CFLAGS_APP) $(CFLAGS) -c $(SRCTREE)/src/reader/pe_read_export_header.c
	$(CC) $(CFLAGS_APP) $(CFLAGS) -c $(SRCTREE)/src/reader/pe_read_import_header.c
	$(CC) $(CFLAGS_APP) $(CFLAGS) -c $(SRCTREE)/src/output/pe_output_export_symbols.c
	$(CC) -static -o perk $(OBJS)

static:
	$(CC) $(CFLAGS_OBJ) $(CFLAGS) -c $(SRCTREE)/src/main/pe_map_raw_image.c
	$(CC) $(CFLAGS_OBJ) $(CFLAGS) -c $(SRCTREE)/src/main/pe_get_image_meta.c
	$(CC) $(CFLAGS_OBJ) $(CFLAGS) -c $(SRCTREE)/src/reader/pe_read_dos_header.c
	$(CC) $(CFLAGS_OBJ) $(CFLAGS) -c $(SRCTREE)/src/reader/pe_read_coff_header.c
	$(CC) $(CFLAGS_OBJ) $(CFLAGS) -c $(SRCTREE)/src/reader/pe_read_optional_header.c
	$(CC) $(CFLAGS_OBJ) $(CFLAGS) -c $(SRCTREE)/src/reader/pe_read_section_header.c
	$(CC) $(CFLAGS_OBJ) $(CFLAGS) -c $(SRCTREE)/src/reader/pe_read_export_header.c
	$(CC) $(CFLAGS_OBJ) $(CFLAGS) -c $(SRCTREE)/src/reader/pe_read_import_header.c
	$(CC) $(CFLAGS_OBJ) $(CFLAGS) -c $(SRCTREE)/src/output/pe_output_export_symbols.c
	$(CROSS_COMPILE)ar -rc libperk.a $(OBJS)
	$(CROSS_COMPILE)ranlib libperk.a

shared:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c $(SRCTREE)/src/main/pe_map_raw_image.c
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c $(SRCTREE)/src/main/pe_get_image_meta.c
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c $(SRCTREE)/src/reader/pe_read_dos_header.c
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c $(SRCTREE)/src/reader/pe_read_coff_header.c
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c $(SRCTREE)/src/reader/pe_read_optional_header.c
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c $(SRCTREE)/src/reader/pe_read_section_header.c
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c $(SRCTREE)/src/reader/pe_read_export_header.c
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c $(SRCTREE)/src/reader/pe_read_import_header.c
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c $(SRCTREE)/src/output/pe_output_export_symbols.c
	$(CC) -shared -o libperk.so $(OBJS)

clean:
	rm -f *~
	rm -f *.o
	rm -f *.a
	rm -f *.so
	rm -f *.gch
	rm -f perk
	rm -f perk.exe
