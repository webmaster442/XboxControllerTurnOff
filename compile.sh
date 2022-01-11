#!/bin/bash
mkdir bin
windres resource.rc -O coff -o bin/resource.res
gcc -mwindows -static main.c -l:libxinput1_4.a -o bin/XboxControllerTurnOff.exe bin/resource.res
rm bin/resource.res