Set DevRoot="%CD%"

ECHO.> %DevRoot%\success

echo Creating release directories ...
md  Release
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\success
md  Release\Common
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\success
md  Release\Internal
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\success
md  Release\NDA
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\success
md  Release\Public
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\success
del /Q /S /F Release
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\success

REM Set the build environments 
if "%VS90COMNTOOLS%"=="" GOTO error_no_VSCOMNTOOLS
call "%VS90COMNTOOLS%\vsvars32.bat"

if "%COMPUTERNAME%"=="CTRL1" call d:\batch\QT333.cmd
set BUILDCOMMAND="devenv.com"
if "%COMPUTERNAME%"=="CTRL1" set BUILDCOMMAND=cov-build -c %DevRoot%\Documentation\Coverity\VS_2005.xml --dir %DevRoot%\CoverityTemp devenv.com
if "%COMPUTERNAME%"=="CTRL1" set path=%path%;d:\coverity\prevent-mingw-4.0.1\bin;
set QMAKESPEC=win32-msvc

pushd Source

%BUILDCOMMAND% BIOSTestSuite.sln /rebuild "Release|Win32"
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\success

%BUILDCOMMAND% BIOSTestSuite.sln /rebuild "Release_NDA|Win32"
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\success

popd

@echo Copying support files ...
copy/Y "%DevRoot%\Source\BTS DB\BIOS_TS.inp" %DevRoot%\Release\Common\*.*
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\success

copy/Y "%DevRoot%\Source\BTS DB\BIOS_TS_ALL.inp" %DevRoot%\Release\Common\*.*
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\success

copy/Y "%DevRoot%\Source\BTS DB\*.db" %DevRoot%\Release\Common\*.*
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\success

copy/Y %DevRoot%\ExternalPackages\QT\*.* %DevRoot%\Release\Common\*.*
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\success

copy/Y %DevRoot%\ExternalPackages\JTagSDK\*.* %DevRoot%\Release\Common\*.*
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\success

copy/Y %DevRoot%\ExternalPackages\TapSDK\*.* %DevRoot%\Release\Common\*.*
if %ERRORLEVEL% NEQ 0 del /Q /F %DevRoot%\success

GOTO END

:error_no_VSCOMNTOOLS
ECHO VSCOMNTOOLS Environment Variable not set, aborting!
del /Q /F %DevRoot%\success

:END
