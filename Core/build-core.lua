project "Core"
  kind "StaticLib"
  language "C++"
  cppdialect "C++20"
  targetdir "bin/%{cfg.buildcfg}"
  staticruntime "off"

  files { "src/**.h", "src/**.cpp" }

  includedirs
  {
    "src"
  }

  targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
  objdir ("../bin/intermediates/" ..OutputDir .."/%{prj.name}")

  filter "system:windows"
    systemversion "latest"
    defines { }
  
  filter "configurations:Debug"
    define { "DEBUG" }
    runtime "Debug"
    symbols "On"

  filter "configurations:Release"
    define { "RELEASE" }
    runtime "Release"
    optimize "On"
    symbols "On"

  filter "configurations:Dist"
    define { "DIST" }
    runtime "Release"
    optimize "On"
    symbols "Off"