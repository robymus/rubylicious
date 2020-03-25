#!/bin/sh
cp arch/i386-mingw32/dll/*.dll build/win32
wine build/win32/rubylicious.exe
