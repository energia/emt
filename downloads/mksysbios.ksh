#!/bin/ksh

tirtos=tirtos_full_2_12_00_20_eng
bios=bios_6_41_03_49_eng

mkdir -p $bios
tclone $TREES/zumaprod/zumaprod-f20/exports/$tirtos/products/$bios/packages $bios/packages

# patch headers to use qualified String and Void names
../src/bundles/energia/patch.ksh $bios/packages/ti/sysbios

# zip it up
zip -rq $bios.zip $bios/
