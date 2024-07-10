workspace "Tetris-Game"
    architecture "x64"
    startproject "Tetris-game"

    configurations
    {
        "Debug",
        "Release"
    }

    flags
    {
        "MultiProcessorCompile"
    }

    filter "system:windows"
        buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Tetris-game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    files
    {
        "**.h",
        "**.cpp"
    }

    includedirs
    {
        "%{wks.location}/libraries/SDL_image/include",
        "%{wks.location}/libraries/SDL_ttf/include",
        "%{wks.location}/libraries/SDL_mixer/include",
        "%{wks.location}/libraries/SDL2/include"
    }

    libdirs
    {
        "%{wks.location}/libraries/SDL_image/lib/x64",
        "%{wks.location}/libraries/SDL_ttf/lib/x64",
        "%{wks.location}/libraries/SDL2/lib/x64",
        "%{wks.location}/libraries/SDL_mixer/lib/x64"
    }

    links
    {
        "SDL2.lib",
        "SDL2_mixer.lib",
        "SDL2main.lib",
        "SDL2_image.lib",
        "SDL2_ttf.lib",
    }

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "On"