@echo off

del *.dll
del *.exe

cl test.c  /EHsc /nologo /D_USRDLL /D_WINDLL /MD /link /DLL /OUT:test.dll 
cl main.cc /EHsc /nologo
main

cl test.cc /EHsc /nologo /D_USRDLL /D_WINDLL /MD /link /DLL /OUT:test.dll 
cl main.cc /EHsc /nologo
main

