@echo off

@echo %TIME% Compile

MSBuild.exe ..\generated\Win32\FontEditor.sln -clp:ErrorsOnly;WarningsOnly -nologo /m
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Succsess

:Failed
@echo %TIME%   !!!!!!!!!!!!!!! Error !!!!!!!!!!!!!!! Build Failed !!!!!!!!!!!!!
goto Exit

:Succsess
@echo %TIME% Completed

:Exit

call copy_wxWidgets_dlls.bat
call copy_resources.bat
