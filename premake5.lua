local tryExcept = function(f, catch)
    -- returns true if f() doesn't throw an error
    local status, exception = pcall(f)
    if not status then
        catch(exception)
    end
    return status
end
local function captureUname()
    local function call()
        local f = assert(io.popen("uname", 'r'))
        local s = assert(f:read('*a'))

        f:close()

        s = string.gsub(s, '^%s+', '')
        s = string.gsub(s, '%s+$', '')
        s = string.gsub(s, '[\n\r]+', ' ')

        return s
    end
    local ok = tryExcept(
        call,
        function(exception)
            print("Error: " .. exception)
        end
    )

    if not ok then return "Windows" end -- Most likely windows

    local uname = call()
    if string.find(uname, "Linux") then
        return "Linux"
    elseif string.find(uname, "Darwin") then
        return "Mac"
    else
        return "Windows"
    end
end

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

    -- only make _WIN32 define on windows
    local uname = captureUname()
    if uname == "Windows" then
        defines { "_WIN32"}
    else
        defines { "LINUX" }
    end

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

