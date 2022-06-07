@ECHO OFF 
ECHO Cleaning up SDK code
CD Lib
del *.* /Q

ECHO Cleaning up MyGLWindow Solution...
CD ..\MyGLWindow
rmdir /S /Q .\Release >nul 2>nul
rmdir /S /Q .\Debug >nul 2>nul
rmdir /S /Q .\_Temp >nul 2>nul
rmdir /S /Q .\_Bin >nul 2>nul
rmdir /S /Q .\.vs >nul 2>nul

ECHO Cleaning up MyKitchen Solution...
CD ..\MyKitchen
rmdir /S /Q .\Release >nul 2>nul
rmdir /S /Q .\Debug >nul 2>nul
rmdir /S /Q .\_Temp >nul 2>nul
rmdir /S /Q .\_Bin >nul 2>nul
rmdir /S /Q .\.vs >nul 2>nul

ECHO Cleaning up MyClient Solution...
CD ..\MyClient
rmdir /S /Q .\Release >nul 2>nul
rmdir /S /Q .\Debug >nul 2>nul
rmdir /S /Q .\_Temp >nul 2>nul
rmdir /S /Q .\_Bin >nul 2>nul
rmdir /S /Q .\.vs >nul 2>nul

ECHO Cleaning up MyApp Solution...
CD ..\MyApp
rmdir /S /Q .\TempRelease >nul 2>nul
rmdir /S /Q .\TempDebug >nul 2>nul
rmdir /S /Q .\Release >nul 2>nul
rmdir /S /Q .\Debug >nul 2>nul
rmdir /S /Q .\_Temp >nul 2>nul
rmdir /S /Q .\_Bin >nul 2>nul
rmdir /S /Q .\.vs >nul 2>nul

CD ..
ECHO Atlas SDK code cleanup complete!
pause

