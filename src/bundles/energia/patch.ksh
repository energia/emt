#!/bin/ksh
#
#  usage: patch closure srcDir configPkg
#
if [ $# -ne 3 ]; then
    echo "usage: patch.ksh closure srcDir configPkg"
    exit 1
fi

t=".tmp$$,patch"

# remove files that should have been removed via closure 
echo "  removing build artifacts ..."
find $1 \( -name "*.cfg.dot" -o -name "*.xdc.inc" -o -name "*.xdc.ninc" -o -name "*.dep" -o -name ".xdcenv.mak" -o -name "package.mak" \) -exec rm -f {} \;
for f in `find $1 -name "*.mak" | egrep '/package/|/lib/`; do
    echo "    removing $f ..."
    rm -f $f
done

echo "  restoring key source files ..."
# add back sources from srcDir
for f in `find $2 -name "*.c" -o -name "*.cpp"`; do
    echo "    restoring $f ..."
    cp -f $f $1/$f
done

# add back big.c
for f in `find $3/package/cfg -name "*.c"`; do
    echo "    restoring $f ..."
    cp -f $f $1/$f
done

