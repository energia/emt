#!/bin/ksh
/db/toolsrc/library/tools/vendors/ti/arm/5.2.4/Linux/bin/armlnk -w -u _c_int00 -fs package/cfg/./  -q -o test.xem3 package/cfg/test_pem3.oem3  package/cfg/test/test.oem3  package/cfg/test_pem3.xdl --silicon_version=7M3 --strict_compatibility=on -c -m package/cfg//test.xem3.map -i /db/toolsrc/library/tools/vendors/ti/arm/5.2.4/Linux/lib/ -l libc.a
