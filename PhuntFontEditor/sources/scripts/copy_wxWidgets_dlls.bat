@echo %TIME%   Copying files *.dll wxWidgtes

xcopy ..\ThirdParty\wxWidgets\generated\lib\vc_dll\*.dll ..\generated\Win32\Editor\Debug /Y /Q
