#!/bin/ksh

tirtos=tirtos_full_2_12_00_24
bios=bios_6_41_03_51

mkdir -p $bios
tclone -c $TREES/zumaprod/zumaprod-f24/exports/$tirtos/products/$bios/packages $bios/packages

# patch headers to use qualified String and Void names
../src/bundles/energia/patch.ksh $bios/packages/ti/sysbios

# zip it up
zip -rq $bios.zip $bios/
