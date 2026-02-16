@echo Copying resources files

mkdir ..\generated\Win32\Editor\icons
mkdir ..\generated\Win32\Editor\Debug\icons

xcopy ..\Editor\icons ..\generated\Win32\Editor\Debug\icons /q /y
xcopy ..\Editor\icons ..\generated\Win32\Editor\icons /q /y

