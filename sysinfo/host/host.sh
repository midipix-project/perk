#!/bin/sh

error_msg()
{
	echo $@ >&2
}

host_test()
{
	mb_hdrdir=$(pwd)/build
	mkdir -p $mb_hdrdir || exit 2

	if [ x"$mb_compiler" = x ]; then
                echo "config error: compiler not set."
		exit 2
	fi

	$mb_compiler -dM -E - < /dev/null > /dev/null && return 0

	error_msg "config error: invalid compiler."
	exit 2
}

host_endian_h()
{
	mb_header='endian.h'
	rm -f "$mb_hdrdir"/$mb_header

	# portable
	printf "#include <$mb_header>" | $mb_compiler $mb_cflags \
		-E - > /dev/null 2>/dev/null \
		&& return 0

	# non-portable
	mb_hosthdr=

	[ -z $mb_hosthdr ] && printf "#include <sys/$mb_header>" | $mb_compiler $mb_cflags \
		-E - > /dev/null 2>/dev/null \
		&& mb_hosthdr='sys/'$mb_header

	[ -z $mb_hosthdr ] && printf "#include <machine/$mb_header>" | $mb_compiler $mb_cflags \
		-E - > /dev/null 2>/dev/null \
		&& mb_hosthdr='machine/'$mb_header

	if [ x"$mb_hosthdr" = x ]; then
		error_msg "config error: could not find an alternate <$mb_header>."
		exit 2
	fi

	printf "#include <%s>\\n" $mb_hosthdr > "$mb_hdrdir"/$mb_header || exit 2
}


# one: args
for arg ; do
	case "$arg" in
		--help)	usage
			;;
		--compiler=*)
			mb_compiler=${arg#*=}
			;;
		--cflags=*)
			mb_cflags=${arg#*=}
			;;
		*)
			error_msg ${arg#}: "unsupported config argument."
			exit 2
			;;
	esac
done


# two: test
host_test


# three: headers
host_endian_h


# all done
exit 0
