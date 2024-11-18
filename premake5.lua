workspace "Engine"
   configurations { "Debug", "Release" }

function includeGLFW()
	includedirs { "lib/GLFW/include", "lib/glad", "lib/KHR", "lib/stb" }
end

function linkGLFW()
	libdirs "lib/GLFW/lib"
    -- mingw32 libs
    links { "glfw3dll", "opengl32", "gdi32" }
end

project "Engine"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    architecture "x86_64"

    files { "src/**.hpp", "src/**.cpp" }

    -- Copy over dll files

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

        filter {}
        includeGLFW()

        filter {}
        linkGLFW()
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

