#ifndef PERK_API_H
#define PERK_API_H

#include <limits.h>

/* perk_export */
#if	defined(__dllexport)
#define perk_export __dllexport
#else
#define perk_export
#endif

/* perk_import */
#if	defined(__dllimport)
#define perk_import __dllimport
#else
#define perk_import
#endif

/* perk_api */
#ifndef PERK_APP
#if     defined (PERK_EXPORT)
#define perk_api perk_export
#elif   defined (PERK_IMPORT)
#define perk_api perk_import
#elif   defined (PERK_STATIC)
#define perk_api
#else
#define perk_api
#endif
#else
#define perk_api
#endif

#endif
