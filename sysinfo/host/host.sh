#!/bin/sh

error_msg()
{
	echo $@ >&2
}

host_test()
{
	mb_hdrdir=$(pwd)/build
	mkdir -p $mb_hdrdir || exit 2

	if [ -z "$mb_compiler" ]; then
                echo "config error: compiler not set."
		exit 2
	fi

	$mb_compiler -dM -E - < /dev/null > /dev/null && return 0

	error_msg "config error: invalid compiler."
	exit 2
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


# all done
exit 0
