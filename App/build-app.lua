project "App"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"
  targetdir "bin/%{cfg.buildcfg}"
  staticruntime "off"

  files { "src/**.h", "src/**.cpp", "../vendor/src/imgui/**.cpp", "../vendor/src/imgui/**.h" }

  includedirs
  {
    "src",
    "../Core/src",
    "../vendor/inc",
    "../vendor/src"
  }

  libdirs
  {
    "../vendor/lib"
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
    defines { "DEBUG" }
    runtime "Debug"
    symbols "On"

  filter "configurations:Release"
    defines { "RELEASE" }
    runtime "Release"
    optimize "On"
    symbols "On"

  filter "configurations:Dist"
    defines { "DIST" }
    runtime "Release"
    optimize "On"
    symbols "Off"