#!/bin/sh

usage()
{
cat << EOF >&2

Usage:
  -h            show this HELP message
  -s  SRCDIR    set source directory
  -o  OUTPUT    set output header
  -p  PREFIX    set macro prefix

EOF
exit 1
}


# one
workdir=$(pwd)
srcdir=
output=
prefix=


while getopts "hs:o:p:" opt; do
	case $opt in
	h)
  		usage
  		;;
	s)
    		srcdir="$OPTARG"
    		;;
	o)
    		output="$OPTARG"
    		;;
	p)
    		prefix="$OPTARG"
    		;;
	\?)
    		printf "Invalid option: -%s" "$OPTARG" >&2
    		usage
    		;;
	esac
done


# two
if [ -z "$srcdir" ] || [ -z "$output" ] || [ -z "$prefix" ]; then
	usage
fi

cd "$srcdir" || exit 2

gitver=`git rev-parse --verify HEAD` || gitver="unknown"
macro=`echo "$prefix"_GIT_VERSION | tr '[:lower:]' '[:upper:]'`

cd "$workdir" || exit 2
printf "#define $macro\t\"$gitver\"\n" > "$output"

# all done
exit 0
