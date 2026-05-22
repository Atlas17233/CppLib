mkdir Build\ifc
taskkill /f /im cpptools-srv.exe
cd Build\ifc &&^
del /q * &&^
copy ..\%1.dir\%2\*.ixx.ifc .\ &&^
ren *.ifc *. &&^
ren *.ixx *.ifc &&^
cl /std:c++latest /EHsc /nologo /W4 /c "%VCToolsInstallDir%modules\std.ixx" &&^
del std.obj