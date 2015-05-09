#ifndef PERK_API_H
#define PERK_API_H

#include <limits.h>

/* perk_export */
#if	defined(__attr_export__)
#define perk_export __attr_export__
#else
#define perk_export
#endif

/* perk_import */
#if	defined(__attr_import__)
#define perk_import __attr_import__
#else
#define perk_import
#endif

/* perk_api */
#ifndef PERK_APP
#if     defined (PERK_BUILD)
#define perk_api perk_export
#elif   defined (PERK_SHARED)
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
