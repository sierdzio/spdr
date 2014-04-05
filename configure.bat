@echo off
echo Oh, hi. Sorry to hear we are compiling on Windows...
echo spdr is not tested on Windows as much as it is being tested on Linux, sorry
echo .
echo For now, only -t (--translate) flag is implemented in this script
echo .
echo Copying QM files to destination directories:
mkdir bin\locale
echo Spdr library
move libspdr\locale\*.qm bin\locale
echo Spdr Command Line Tool
move cli\locale\*.qm bin\locale
echo Spdr Widget GUI
move widgetgui\locale\*.qm bin\locale
echo Done
