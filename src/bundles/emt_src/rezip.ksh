#!/bin/ksh
#
#  usage: rezip zip-file [op ...]
#    where each op renames a file or directory in the original
#    zip and is of the form <from>:<to>
#  
#    If <to> is empty, <from> is deleted.
#
usage="usage: rezip zip-in zip-out [from:to ...]"

if [ $# -lt 2 ]; then
    echo $usage 
    exit 1
fi

## compute absolute path to input zip file
zdir=`dirname "$1"`
zip="`cd "$zdir";/bin/pwd`/`basename "$1"`"
shift

## compute absolute path to output zip file
odir=`dirname "$1"`
out="`cd "$odir";/bin/pwd`/`basename "$1"`"
shift

## move to a tmp working directory
w=.rezip$$
mkdir $w
cd $w

## unzip $zip
unzip -q $zip

## apply all specified transforms
for op in $@; do
    src=`echo $op | cut -d: -f1`
    dst=`echo $op | cut -d: -f2`
    if [ -z "$src" ]; then
        echo skipping bogus command: \'$op\'
        continue
    fi
    if [ -z "$dst" ]; then
        echo "    rm -rf $src"
        rm -rf "$src"
    else
        echo "    mv -f $src $dst"
        mv -f "$src" "$dst"
    fi
done

## zip up the result
echo "    zip -r tmp.zip `ls`"
zip -rq tmp.zip `ls`
status=$?

## move new zip to output (if successful)
if [ "$status" = "0" ]; then
    echo "    exporting to $out ..."
    mv -f tmp.zip "$out"
    status=$?
fi

## cleanup our mess
cd ..
rm -rf $w
exit $?
