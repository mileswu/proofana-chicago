if test "x$ROOTCOREBIN" != "x"
then
    replace=`echo $ROOTCOREBIN | sed 's/\\//\\\\\\//g'`
    PATH=`echo $PATH | sed "s/^$replace\/bin://" | sed "s/:$replace\/bin://"`
    LD_LIBRARY_PATH=`echo $LD_LIBRARY_PATH | sed "s/^$replace\/lib://" | sed "s/:$replace\/lib://"`
    DYLD_LIBRARY_PATH=`echo $DYLD_LIBRARY_PATH | sed "s/^$replace\/lib://" | sed "s/:$replace\/lib://"`
    PYTHONPATH=`echo $PYTHONPATH | sed "s/^$replace\/python://" | sed "s/:$replace\/python://"`
    unset ROOTCOREDIR
    unset ROOTCOREBIN
fi
