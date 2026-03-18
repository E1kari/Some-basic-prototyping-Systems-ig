@echo off
cd /d "%~dp0"

echo Cleaning Unreal build folders...

rmdir /s /q "Binaries"
rmdir /s /q "Intermediate"
rmdir /s /q ".vs"

rmdir /s /q "Plugins\SplinePointsMetadata\Binaries"
rmdir /s /q "Plugins\SplinePointsMetadata\Intermediate"

echo Done.
pause