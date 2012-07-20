#!/bin/sh

source "`dirname $0`/preamble.sh" "$0"

VERSIONFILE="$1"

test "$VERSIONFILE" != "" && test \! -f $VERSIONFILE && echo did not find version file $VERSIONFILE && exit 2

function check_head {
    test "`echo $1 | grep /tags/`" != "" && return 1
    test "`echo $1 | grep /trunk/`" != "" && return 0
    test "`echo $1 | grep /trunk$`" != "" && return 0
    test "`echo $1 | grep /branches/`" != "" && return 0
    exit 3
}

for pkg in $ROOTCOREDIR `cat $pkgfile`
do
    if test \! -d $pkg/.svn
    then
	echo `basename $pkg` not in SVN
    else
	name=`basename $pkg`
	old_tag=`grep svn+ssh $pkg/.svn/entries  | grep /$name | head -n 1`
	new_tag=""
	test "$VERSIONFILE" != "" && new_tag=`grep /$name $VERSIONFILE`
	test "$new_tag" != "" && new_tag=`$ROOTCOREDIR/scripts/svn_get_url.sh $new_tag $old_tag`
	raw_tag=$new_tag
	test "$new_tag" == "" && new_tag=$old_tag

	if test "$old_tag" == ""
	then
	    echo failed to read svn info for $name
	elif check_head "$old_tag"
	then
	    if test "$old_tag" == "$new_tag"
	    then
		echo updating $name from SVN head
		if (cd $pkg && svn update)
		then
		    true
		else
		    echo failed to update $pkg
		    exit 4
		fi
	    else
		echo you currently have the head version of $name checked out
		echo please check in your changes and then switch manually
		echo   cd $pkg
		echo   svn switch $new_tag
	    fi
	    echo
	else
	    if test "$raw_tag" == ""
	    then
		echo $name not in release, keeping at version `echo $new_tag | sed 's/.*\/tags\///'`
	    elif test "$old_tag" == "$new_tag"
	    then
		echo $name already at version `echo $new_tag | sed 's/.*\/tags\///'`
	    else
		echo $old_tag $new_tag
		echo updating $name to version `echo $new_tag | sed 's/.*\/tags\///'`
		if (cd $pkg && svn switch "$new_tag")
		then
		    true
		else
		    echo failed to update $pkg
		    exit 5
		fi
		echo
	    fi
	fi
    fi
done
