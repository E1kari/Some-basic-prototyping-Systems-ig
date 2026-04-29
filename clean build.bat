@echo off
echo Cleaning Unreal build folders...

rmdir /s /q "Binaries" 2>nul
rmdir /s /q "Intermediate" 2>nul
rmdir /s /q ".vs" 2>nul

rmdir /s /q "Plugins\FlightSystem\Binaries" 2>nul
rmdir /s /q "Plugins\FlightSystem\Intermediate" 2>nul

echo Done.
