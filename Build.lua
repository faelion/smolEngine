-- premake5.lua
workspace "smol"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "smolEditor"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Dependencies"
    include "Dependencies.lua"
group ""

group "Core"
include "Core/Build-Core.lua"
group ""

group "Editor"
include "App/Build-App.lua"
group ""
