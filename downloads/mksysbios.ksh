#!/bin/ksh
mkdir -p bios_6_41_03_49_eng
tclone $TREES/zumaprod/zumaprod-f20/exports/tirtos_full_2_12_00_20_eng/products/bios_6_41_03_49_eng/packages bios_6_41_03_49_eng/packages
../src/bundles/energia/patch.ksh bios_6_41_03_49_eng/packages/ti/sysbios
zip -rq bios_6_41_03_49_eng.zip bios_6_41_03_49_eng/
