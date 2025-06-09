#ifndef PERK_EDEFS_H
#define PERK_EDEFS_H

/***************************************************/
/* perk_edefs.h: PE backend extended definitions.  */
/***************************************************/

/* symbol attributes (tulips) */
#define PE_SYM_ATTR_TYPE_NOTYPE             (0)
#define PE_SYM_ATTR_TYPE_FILE               (1 << 0)
#define PE_SYM_ATTR_TYPE_OBJECT             (1 << 1)
#define PE_SYM_ATTR_TYPE_SECTION            (1 << 2)
#define PE_SYM_ATTR_TYPE_FUNCTION           (1 << 3)
#define PE_SYM_ATTR_TYPE_STRING             (1 << 4)
#define PE_SYM_ATTR_TYPE_COMMON             (1 << 5)
#define PE_SYM_ATTR_TYPE_STACK              (1 << 6)
#define PE_SYM_ATTR_TYPE_RDATA              (1 << 7)
#define PE_SYM_ATTR_TYPE_BSS                (1 << 8)
#define PE_SYM_ATTR_TYPE_TLS                (1 << 9)
#define PE_SYM_ATTR_TYPE_GOT                (1 << 10)
#define PE_SYM_ATTR_TYPE_PLT                (1 << 11)
#define PE_SYM_ATTR_TYPE_DSO                (1 << 12)
#define PE_SYM_ATTR_TYPE_REF                (1 << 13)

#define PE_SYM_ATTR_TYPE_IFUNC              (1 << 14)
#define PE_SYM_ATTR_TYPE_DEBUG              (1 << 15)
#define PE_SYM_ATTR_TYPE_PDATA              (1 << 16)
#define PE_SYM_ATTR_TYPE_XDATA              (1 << 17)
#define PE_SYM_ATTR_TYPE_XATTR              (1 << 18)
#define PE_SYM_ATTR_TYPE_RELOC              (1 << 19)
#define PE_SYM_ATTR_TYPE_RSRC               (1 << 20)

#define PE_SYM_ATTR_BIND_LOCAL              (1 << 21)
#define PE_SYM_ATTR_BIND_GLOBAL             (1 << 22)
#define PE_SYM_ATTR_BIND_UNIQUE             (1 << 23)
#define PE_SYM_ATTR_BIND_PENDING            (1 << 24)
#define PE_SYM_ATTR_BIND_WEAK_ALIAS         (1 << 25)
#define PE_SYM_ATTR_BIND_WEAK_EXTERN        (1 << 26)
#define PE_SYM_ATTR_BIND_WEAK_SYMBOL        (1 << 27)

#define PE_SYM_ATTR_VISIBILITY_DEFAULT      (1 << 28)
#define PE_SYM_ATTR_VISIBILITY_PROTECTED    (1 << 29)
#define PE_SYM_ATTR_VISIBILITY_HIDDEN       (1 << 30)
#define PE_SYM_ATTR_VISIBILITY_INTERNAL     (1 << 31)

#endif
