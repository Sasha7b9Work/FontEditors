@echo off

@echo %TIME%

cd ..

rmdir "generated/Win32" /s /q

cd scripts

@echo on
cmake ../CMakeLists.txt -B../generated/Win32 -G "Visual Studio 16 2019" -A Win32

@echo off

@echo %TIME%
