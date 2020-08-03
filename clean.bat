@echo off
rmdir /s /q ".vs" >nul 2>nul

rmdir /s /q "Debug" >nul 2>nul
rmdir /s /q "Release" >nul 2>nul
rmdir /s /q "x64" >nul 2>nul

rmdir /s /q "src\.deps" >nul 2>nul
rmdir /s /q "autom4te.cache" >nul 2>nul
rmdir /s /q "config-aux" >nul 2>nul

del aclocal.m4
del avarice.spec
del config.log
del config.status
del configure
del Makefile
del Makefile.in
del doc\Makefile
del doc\Makefile.in
del scripts\ice-gdb
del scripts\Makefile
del scripts\Makefile.in
del src\Makefile
del src\Makefile.in
del src\avarice
del src\autoconf.h
del src\autoconf.hin
del src\stamp-h1
del src\*.o
