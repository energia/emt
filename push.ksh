#!/bin/ksh

echo "pushing to energia.nu ..."
base=./src/bundles/energia
$base/push.exp $base/msp432/closure.zip msp432-closure.zip \
               $base/cc3200/closure.zip cc3200-closure.zip \
               $base/cc26xx/closure.zip cc26xx-closure.zip \
               ./exports/emt_src.zip    emt_src.zip

