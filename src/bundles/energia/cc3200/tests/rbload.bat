@echo off

REM
REM Flash an executable to the RedBear board
REM
REM usage: rbload.bat executable [[com_port_id] [energia_install_dir]]
REM
REM where,
REM     executable          - the executable to flash onto the RedBear board
MRE     com_port_id         - the COM port ID for the RedBear board
REM     energia_install_dir - directory which Energia is installed (this
REM                           is needed to find the flash utility)
REM

setlocal

REM define a default path to locate executable to flash
set binpath=V:\dr\emt\src\ti\runtime\wiring\cc3200\tests\zumo

if "%1"=="" (
    echo usage: %~nx0 binary [[com_port_id] [energia_install_dir]]
    exit /b
)

REM set bin to the executable to flash to the RedBear board
set bin=%1
set localbin=%TMP%\%~nx1
if not exist %1 (
    if exist %binpath%\%1 (
        set bin=%binpath%\%1
    ) else (
        echo error: can't find the binary file "%1"
	exit /b 1
    )
)
REM set comid to COM port number for the RedBear board
set comid=%2
if "%2"=="" (
    set comid=9
)

REM set eidir to Energia install directory
if "%3"=="" (
    set eidir=C:\ti
) else (
    set eidir=%3
)

for /f "usebackq delims=" %%f in (`dir /b /o:n %eidir%\energia-*`) do (
    REM cc3200prog may improperly load network files, so we create a local copy
    copy %bin% %localbin%
    
    REM cc3200prog must also be run from it's home directory (!)
    echo chdir %eidir%\%%f\hardware\tools\lm4f\bin
    chdir %eidir%\%%f\hardware\tools\lm4f\bin

    echo cc3200prog %comid% %localbin%
    cc3200prog %comid% %localbin% || exit /b 1
    exit /b 0
)

echo error: energia is not installed in %eidir%
exit /b 1
