pushd %~dp0\prj
call makegit
premake5 vs2013
cd ..
msbuild ee.sln
popd
