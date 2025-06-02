API_HEADERS = \
	$(SOURCE_DIR)/include/$(PACKAGE)/perk.h \
	$(SOURCE_DIR)/include/$(PACKAGE)/perk_api.h \
	$(SOURCE_DIR)/include/$(PACKAGE)/perk_arbits.h \
	$(SOURCE_DIR)/include/$(PACKAGE)/perk_consts.h \
	$(SOURCE_DIR)/include/$(PACKAGE)/perk_meta.h \
	$(SOURCE_DIR)/include/$(PACKAGE)/perk_output.h \
	$(SOURCE_DIR)/include/$(PACKAGE)/perk_structs.h

INTERNAL_HEADERS = \
	$(SOURCE_DIR)/src/internal/argv/argv.h \
	$(SOURCE_DIR)/src/internal/$(PACKAGE)_ar_impl.h \
	$(SOURCE_DIR)/src/internal/$(PACKAGE)_dprintf_impl.h \
	$(SOURCE_DIR)/src/internal/$(PACKAGE)_driver_impl.h \
	$(SOURCE_DIR)/src/internal/$(PACKAGE)_endian_impl.h \
	$(SOURCE_DIR)/src/internal/$(PACKAGE)_errinfo_impl.h \
	$(SOURCE_DIR)/src/internal/$(PACKAGE)_hexdump_impl.h \
	$(SOURCE_DIR)/src/internal/$(PACKAGE)_reader_impl.h \
	$(SOURCE_DIR)/src/internal/$(PACKAGE)_synopsis_impl.h \
	$(SOURCE_DIR)/src/internal/$(PACKAGE)_visibility_impl.h \

ALL_HEADERS = $(API_HEADERS) $(INTERNAL_HEADERS)
