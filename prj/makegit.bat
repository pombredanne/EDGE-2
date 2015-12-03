:: generate git info for C++ inclusion
:: - rlyeh, public domain

pushd %~dp0

git rev-list --count --first-parent HEAD > git.hpp
set /p REVISION=<git.hpp

git rev-parse --abbrev-ref HEAD > git.hpp
set /p BRANCH=<git.hpp

echo #pragma once > git.hpp
echo #define GIT_BRANCH "%BRANCH%" >> git.hpp
echo #define GIT_REVISION "%REVISION%" >> git.hpp
echo #define GIT_REVISION_NUM %REVISION% >> git.hpp

popd
