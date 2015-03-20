#!/bin/ksh
t=".tmp$$,patch"
for f in `find $1 -name "*.h" -exec egrep "(^static inline String)|(^__extern Void)" {} /dev/null \; | cut -d: -f1`; do
    echo "    patching $f ..."
    sed -e 's/^__extern Void /__extern void /' -e 's/^static inline String /static inline xdc_String /' $f > $t
    mv -f $t $f
done
