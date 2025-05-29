#ifndef PERK_VISIBILITY_IMPL_H
#define PERK_VISIBILITY_IMPL_H

/**********************************************************************/
/* PE targets: __dllexport suffices for the purpose of exporting only */
/* the desired subset of global symbols; this makes the visibility    */
/* attribute not only redundant, but also tricky if not properly      */
/* supported by the toolchain.                                        */
/*                                                                    */
/* When targeting Midipix, __PE__, __dllexport and __dllimport are    */
/* always defined by the toolchain. Otherwise, the absnece of these   */
/* macros has been detected by sofort's ccenv.sh during ./configure,  */
/* and they have accordingly been added to CFLAGS_OS.                 */
/**********************************************************************/

#ifdef __PE__
#define perk_hidden
#else
#ifdef _ATTR_VISIBILITY_HIDDEN
#define perk_hidden _ATTR_VISIBILITY_HIDDEN
#else
#define perk_hidden
#endif
#endif

#endif
