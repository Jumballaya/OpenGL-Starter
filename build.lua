-- premake5.lua
workspace "New Project"
  architecture "x64"
  configurations { "Debug", "Release", "Dist" }
  startproject "App"

  -- Workspace-wide build options for MSVC
  filter "system:windows"
    buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }
    links { "glfw3", "opengl32", "user32", "gdi32", "shell32" }
    libdirs { "vendor/lib"}

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Core"
  include "Core/build-core.lua"
group ""

include "App/build-app.lua"