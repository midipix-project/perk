#!/bin/sh

# prefix, exec_prefix
if [ "$PKGCONF_PREFIX" = "$PKGCONF_EXEC_PREFIX" ]; then
	echo 'prefix='${PKGCONF_PREFIX}
	echo 'exec_prefix=${prefix}'
else
	echo 'prefix='${PKGCONF_PREFIX}
	echo 'exec_prefix='${PKGCONF_EXEC_PREFIX}
fi


# (relative) includedir
prefix=`dirname "$PKGCONF_INCLUDEDIR"`
base=`basename "$PKGCONF_INCLUDEDIR"`

if [ "$prefix/$base" = "$PKGCONF_PREFIX/$base" ]; then
	echo 'includedir=${prefix}/'${base}
else
	echo 'includedir='${PKGCONF_INCLUDEDIR}
fi

if [ "$prefix/$base" = "$PKGCONF_PREFIX/include" ]; then
	CFLAGS=
else
	CFLAGS='-I{includedir}'
fi


# (relative) libdir
prefix=`dirname "$PKGCONF_LIBDIR"`
base=`basename "$PKGCONF_LIBDIR"`

if [ "$prefix/$base" = "$PKGCONF_EXEC_PREFIX/$base" ]; then
	echo 'libdir=${prefix}/'${base}
else
	echo 'libdir='${PKGCONF_LIBDIR}
fi

if [ "$prefix/$base" = "$PKGCONF_EXEC_PREFIX/lib" ]; then
	LDFLAGS='-l'${PKGCONF_NAME}
elif [ "$prefix/$base" = "$PKGCONF_EXEC_PREFIX/lib64" ]; then
	LDFLAGS='-l'${PKGCONF_NAME}
else
	LDFLAGS='-L'${PKGCONF_LIBDIR} '-l'${PKGCONF_NAME}
fi


# name, description, url, version
echo
echo 'Name:        '${PKGCONF_NAME}
echo 'Description: '${PKGCONF_DESC}
echo 'URL:         '${PKGCONF_USRC}
echo 'Version:     '${PKGCONF_VERSION}


# repo (optional)
if ! [ -z "$PKGCONF_REPO" ]; then
	echo 'Repo:        '${PKGCONF_REPO}
fi


# patches (optional)
if ! [ -z "$PKGCONF_PSRC" ]; then
	echo 'Patches:      '${PKGCONF_PSRC}
fi


# disto (optional)
if ! [ -z "$PKGCONF_DURL" ]; then
	echo 'Distro:       '${PKGCONF_DURL}
fi


# Cflags
if [ -z "$PKGCONF_DEFS" ] && [ -z "$CFLAGS" ]; then
	echo 'Cflags:'
elif [ -z "$PKGCONF_DEFS" ]; then
	echo 'Cflags:      '${CFLAGS}
elif [ -z "$CFLAGS" ]; then
	echo 'Cflags:      '${PKGCONF_DEFS}
else
	echo 'Cflags:      '${PKGCONF_DEFS} ${CFLAGS}
fi


# Libs
if [ -z "$PKGCONF_LIBS" ]; then
	echo 'Libs:        '${LDFLAGS}
else
	echo 'Libs:        '${PKGCONF_LIBS} ${LDFLAGS}
fi
