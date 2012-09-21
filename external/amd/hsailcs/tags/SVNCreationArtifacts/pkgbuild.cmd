REM PkgBuild.cmd
REM =========
REM MAKES AN INSTALLSHIELD PACKAGE
REM DEPENDS ON STANDARD AMD INSTALLSHIELD PROJECT STRUCTURE AND IS PATH RELATIVE

REM use clrenv.cmd to debug clean up before starting
call D:\batch\clrenv.cmd

:SETUP_PARAMETERS
REM PACKAGE DEVELOPER SHOULD SET PROJECT_NAME AND SCRIPT_NAME
REM AS PkgBuild.cmd IS MIGRATED TO NEW PROJECTS

SET PKG_SCRIPT_PATH="Installation\Scripts"
SET SCRIPT_NAME=setup.ism
SET PKGCHECK=TRUE

:PICK RELEASETYPE
if %theBType%=="alpha" GOTO ALPHASET
if %theBType%=="beta" GOTO BETASET
if %theBType%=="test" GOTO TESTSET
if %theBType%=="release" GOTO RELEASESET

:ALPHASET
SET PROJECT_NAME=ProductName Alpha
GOTO CONTINUE

:BETASET
SET PROJECT_NAME=ProductName Beta
GOTO CONTINUE

:TESTSET
SET PROJECT_NAME=ProductName NOT FOR DISTRIBUTION
GOTO CONTINUE

:RELEASESET
SET PROJECT_NAME=ProductName
GOTO CONTINUE

:CONTINUE

REM AMDVersion.cmd must be dropped by Version.pl
call AMDVersion.cmd
SET PKG_VERSION

mkdir BOM
mkdir BOM\Internal
mkdir BOM\NDA
mkdir BOM\Public

REM Don't change these! 
REM ==================
SET PKG_SCRIPT_PATH="Installation\Scripts"
pushd %PKG_SCRIPT_PATH%

:Update_Version_Number
Echo PkgBuild.cmd: Updating Version info
Update_Version.vbs
if %ERRORLEVEL%==0 ECHO. > %DevRoot%\pkgsuccess

:Build_Package
Echo PkgBuild.cmd: Building %PKG_SCRIPT_PATH%\%SCRIPT_NAME% Internal
c:\bin\StandaloneBuild2009sp2\System\IsCmdBld.exe -p %SCRIPT_NAME% -b "%DEVROOT%\Temp\Internal" -r "Internal_Compressed"
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\pkgsuccess

:Build_Package
Echo PkgBuild.cmd: Building %PKG_SCRIPT_PATH%\%SCRIPT_NAME% NDA
c:\bin\StandaloneBuild2009sp2\System\IsCmdBld.exe -p %SCRIPT_NAME% -b "%DEVROOT%\Temp\NDA" -r "NDA_Compressed"
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\pkgsuccess

:Build_Package
Echo PkgBuild.cmd: Building %PKG_SCRIPT_PATH%\%SCRIPT_NAME% Public
c:\bin\StandaloneBuild2009sp2\System\IsCmdBld.exe -p %SCRIPT_NAME% -b "%DEVROOT%\Temp\Public" -r "Public_Compressed"
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\pkgsuccess

:POST_PROCESS
REM Go back where we started
popd
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\pkgsuccess

Echo PkgBuild.cmd: Generate Manifest.
perl %DevRoot%\Installation\Scripts\generate_manifests.pl
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\pkgsuccess

Echo PkgBuild.cmd: Copy Internal release into BOM folder
copy /y "%DEVROOT%\Temp\Internal\Setup.exe" "%DEVROOT%\Bom\Internal\Setup.exe"
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\pkgsuccess

copy /y "%DEVROOT%\Documentation\ReadMe\ReadMe_Internal.rtf" "%DEVROOT%\Bom\Internal\ReadMe.rtf"
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\pkgsuccess

Echo PkgBuild.cmd: Copy Internal Manifest into BOM folder
copy /y "%DEVROOT%\Installation\Files\manifest_Internal.xml" "%DEVROOT%\Bom\Internal\manifest.xml"
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\pkgsuccess

Echo PkgBuild.cmd: Copy NDA release into BOM folder
copy /y "%DEVROOT%\Temp\NDA\Setup.exe" %DEVROOT%\Bom\NDA\Setup.exe
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\pkgsuccess

copy /y "%DEVROOT%\Documentation\ReadMe\ReadMe_NDA.rtf" "%DEVROOT%\Bom\NDA\ReadMe.rtf"
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\pkgsuccess

Echo PkgBuild.cmd: Copy NDA Manifest into BOM folder
copy /y "%DEVROOT%\Installation\Files\manifest_nda.xml" "%DEVROOT%\Bom\NDA\manifest.xml"
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\pkgsuccess

Echo PkgBuild.cmd: Copy Public release into BOM folder
copy /y "%DEVROOT%\Temp\Public\Setup.exe" %DEVROOT%\Bom\Public\Setup.exe
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\pkgsuccess

copy /y "%DEVROOT%\Documentation\ReadMe\ReadMe_Public.rtf" "%DEVROOT%\Bom\Public\ReadMe.rtf"
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\pkgsuccess

Echo PkgBuild.cmd: Copy NDA Manifest into BOM folder
copy /y "%DEVROOT%\Installation\Files\manifest_Public.xml" "%DEVROOT%\Bom\Public\manifest.xml"
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\pkgsuccess