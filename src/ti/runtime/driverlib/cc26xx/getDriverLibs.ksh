#!/bin/ksh
SRC=/db/vtree/dr/drivers-dr/packages/ti/drivers/lib
if [ $# -ge 1 ]; then
    SRC=$1
fi

mkdir -p ti_drivers_lib
cp -fp $SRC/drivers_cc13xxware*.am3g ti_drivers_lib/
cp -fp $SRC/drivers_cc26xxware*.am3g ti_drivers_lib/
