workspace "Engine"
   configurations { "Debug", "Release" }

function includeLibraries()
	includedirs { 
        "lib/GLEW/include",
        "lib/SDL2/include",
    }
end

function linkLibraries()
	libdirs { 
        "lib/GLEW/lib",
        "lib/SDL2/lib"
    }
    -- mingw32 libs
    ---lglu32 -lopengl32 -lGL -lGLU -lglew
    links { "opengl32", "gdi32", "glew32", "SDL2", "SDL2main" }
end

function getDlls()
    local dlls = {
        "lib/GLEW/lib/glew32.dll",
    }

end

project "Engine"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    architecture "x86_64"

    files { "src/**.hpp", "src/**.cpp" }

    -- Copy over dll files

    defines { "_WIN32"}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

        filter {}
        includeLibraries()

        filter {}
        linkLibraries()
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

