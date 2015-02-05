#!/bin/ksh

vers=1_97_00_47b
drvlib=/db/vendors/ti/msp43xware/$vers/msp430ware/driverlib

# TI-RTOS prefix for Driverlib product
prefix=MSP430Ware

if [ -d $drvlib ]; then
    rm -f ${prefix}_$vers.zip
    mkdir -p ${prefix}_$vers/msp430ware
    ln -s $drvlib ${prefix}_$vers/msp430ware/driverlib
    zip -rq ${prefix}_$vers.zip ${prefix}_$vers
    rm -rf ${prefix}_$vers
else
    echo "$drvlib does not exist"
    exit 1
fi
