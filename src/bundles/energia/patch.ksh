#!/bin/ksh
#
#  usage: patch closure srcDir
#

t=".tmp$$,patch"

# fixup generated module headers with undecorated String references
for f in `find $1 -name "*.h" -exec egrep "(^static inline String)|(^__extern Void)" {} /dev/null \; | cut -d: -f1`; do
    echo "    patching $f ..."
    sed -e 's/^__extern Void /__extern void /' -e 's/^static inline String /static inline xdc_String /' $f > $t
    mv -f $t $f
done

# add back sources from srcDir
for f in `find $2 -name "*.c" -o -name "*.cpp"`; do
    echo "    restoring $f ..."
    cp -f $f $1/$f
done

