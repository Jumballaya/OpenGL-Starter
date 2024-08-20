project "App"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"
  targetdir "bin/%{cfg.buildcfg}"
  staticruntime "off"

  files { "src/**.h", "src/**.cpp" }

  includedirs
  {
    "src",
    "../Core/src"
  }

  links
  {
    "Core"
  }

  targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
  objdir ("../bin/intermediates/" ..OutputDir .."/%{prj.name}")

  filter "system:windows"
    systemversion "latest"
    defines { "WINDOWS" }
  
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