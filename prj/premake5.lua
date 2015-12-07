-- [ref] http://industriousone.com/scripting-reference

newoption { trigger = "no-audio", description = "Disable use of OpenAL/Ogg/Vorbis APIs" }
newoption { trigger = "no-tests", description = "Disable generation of test projects" }
newoption { trigger = "no-examples", description = "Disable generation of examples" }
newoption { trigger = "gen-licenses", description = "Generate LICENSES and update README.md" }

solution "ee"
location ("..")
configurations { "Debug", "DebugOpt", "Release", "Shipping" }
platforms {"native", "x64", "x32"}
language "C"
targetdir ("../.obj")
debugdir  ("../.obj")
objdir    ("../.obj")
startproject "eePlayer"

filter { "system:unix" }
    buildoptions "-std=c++11 -pthread"

filter { "system:macosx and (**.cpp or **.cc or **.cxx)" }
    buildoptions { "-std=c++11" }
filter { "system:macosx and (**.c)" }
    buildoptions { "-std=c99" }
filter { "system:macosx" }
    linkoptions "-stdlib=libc++"

filter "action:vs*"
    defines { "_SCL_SECURE_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "NOMINMAX" }
    flags {  "NoEditAndContinue", "Maps" } -- "StaticRuntime" }  --NoMinimalRebuild
--      vectorextensions "SSE" -- "SSE2" -- vs2013 pro only
--      flags { "ExtraWarnings", "FatalWarnings", "MultiProcessorCompile", }
--      flags { "NoRTTI" }
--      flags( "NoExceptions" )
--      defines { "_SECURE_SCL=0", "_SECURE_SCL_THROWS=0" } -- @master?
--      defines { "_WIN32_WINNT=0x0501"} -- needed for boost
    buildoptions {
        "/wd4018", -- '<' : signed/unsigned mismatch
        "/wd4099", -- PDB 'xxx.pdb' was not found with ...\yyy.pdb'; linking object as if no debug info (issue with 3rdparty libs)
        "/wd4100", -- unreferenced formal parameter
        "/wd4127", -- conditional expression is constant
        "/wd4244", -- 'initializing' : conversion from 'X' to 'Y', possible loss of data
        "/wd4267", -- conversion from 'T' to 'U', possible loss of data
        "/wd4309", -- truncation of constant value
        "/wd4503", -- decorated name length exceeded, name was truncated
        "/wd4530", -- C++ exception handler used, but unwind semantics are not enabled.
        "/wd4800", -- forcing value to bool 'true' or 'false' (performance warning)
        "/wd4996", -- this function or variable may be unsafe. Consider using asctime_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS.
        "/wd4577", -- 'noexcept' used with no exception handling mode specified; termination on exception is not guaranteed. Specify /EHsc
    }

filter "Debug"
    defines { "DEBUG","_DEBUG", "HAS_DEBUGINFO=3" }
    flags { --[["NoRTTI",]] "NoExceptions", --[["StaticRuntime",]] "Symbols" }
    floatingpoint "Fast" -- "Strict" raises errors on stb :(
    optimize "Off"
    targetsuffix "-d"

filter "DebugOpt"
    defines { "DEBUG","_DEBUG","DEBUGOPT", "HAS_DEBUGINFO=2", "HAS_OPTIMIZATIONS=1", }
    flags { --[["NoRTTI",]] "NoExceptions", "StaticRuntime", "Symbols", "NoBufferSecurityCheck", "NoRuntimeChecks" }
    floatingpoint "Fast"
    optimize "Debug"
    targetsuffix "-do"

filter "Release"
    defines { "NDEBUG","_NDEBUG", "HAS_DEBUGINFO=1", "HAS_OPTIMIZATIONS=2" }
    flags { --[["NoRTTI",]] "NoExceptions", --[["StaticRuntime",]] "Symbols", "NoBufferSecurityCheck", "NoRuntimeChecks" }
    floatingpoint "Fast"
    optimize "On"
    targetsuffix "-r"

filter "Shipping"
    defines { "NDEBUG","_NDEBUG","RETAIL","SHIPPING","MASTER","GOLD", "HAS_OPTIMIZATIONS=3" }
    flags { --[["NoRTTI",]] "NoExceptions", "StaticRuntime", --[["Symbols",]] "NoBufferSecurityCheck", "NoRuntimeChecks", "NoFramePointer", "LinkTimeOptimization" }
    floatingpoint "Fast"
    optimize "Full"
    targetsuffix "-sh"

-- EDGE ENGINE

project "eeCore"
    kind "StaticLib" --"SharedLib"
    language "C++"
    includedirs {
        "../",
        "../3rd/",
        "../src/",
        "../src/*/",
    }
    files {
        "../3rd/frodo/*.*pp",
        "../3rd/hertz/*.hpp",
        "../3rd/apathy/*.hpp",
        "../3rd/journey/*.hpp",
        "../3rd/whereami/*.?",
        "../src/**.h",
        "../src/**.c",
        "../src/**.*pp",
        "../src/**.inl",
        "../src/**.hh",
        --"../src/**.cc",
        --"../src/**.*xx",
        "../src/**.mm",
    }
    defines {
        os.is("windows") and "WIN32" or "",
    }
    links {
    }

project "eePlayer"
    kind "ConsoleApp"
    language "C++"
    targetdir ("../")
    files {
        "../src/**.h",
        "../src/**.hh",
        "../src/**.hpp",
        "../src/**.hxx",

        "../src/**.c",
        --"../src/**.cc",
        --"../src/**.cxx",
        "../src/**.cpp",

        "../src/**.inl",
        "../src/**.ipp",

        "../src/**.mm",
    }

    if os.is("windows") then
    os.copyfile("../3rd/SDL2/lib/x64/SDL2.dll", "../SDL2.dll")
    end

    includedirs {
        "../",
        "../3rd/",
        "../3rd/*/",
        "../3rd/*/include/",
        "../src/",
        "../src/*/",

        "../addons/**/",
        "../ext/**/",
    }

    libdirs {
        "../.obj/",
        "../3rd/SDL2/lib/x64/",
        "../3rd/SFML2/extlibs/libs-msvc/x64/",
    }

    links { 
        "eeCore"
    }
