@echo on
del c:\Users\dr\%1
copy V:\dr\emt\src\ti\runtime\wiring\cc3200\tests\zumo\%1 c:\Users\dr\%1
chdir C:\ti\energia-0101E0016b4\hardware\tools\lm4f\bin
C:\ti\energia-0101E0016b4\hardware\tools\lm4f\bin\cc3200prog 9 c:\Users\dr\%1
chdir c:\Users\dr
