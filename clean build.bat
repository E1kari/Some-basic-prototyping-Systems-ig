@echo off
echo Cleaning Unreal build folders...

rmdir /s /q Binaries
rmdir /s /q Intermediate
rmdir /s /q .vs

echo Done.
pause