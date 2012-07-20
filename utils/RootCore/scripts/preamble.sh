test "x$ROOTCOREDIR" = "x" && echo "ROOTCOREDIR not set, please source setup.[c]sh" && exit 1
test "x$ROOTCOREBIN" = "x" && echo "ROOTCOREBIN not set, please source setup.[c]sh" && exit 1

pkgfile=$ROOTCOREBIN/packages

script=`basename $1`
if test \! -f "$pkgfile"
then
    if test "$script" != "find_packages.sh" -a  "$script" != "checkout.sh"
    then
	echo "did not find package list, please run"
	echo "  \$ROOTCOREDIR/scripts/find_packages.sh"
	echo "at the base of your source directory"
	exit 100
    fi
fi
