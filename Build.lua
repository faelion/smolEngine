-- premake5.lua
workspace "smol"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "smolEditor"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "smolEngine"
	include "Core/Build-Core.lua"
    include "Dependencies.lua"
group ""

include "App/Build-App.lua"

postbuildcommands {"{COPY} \"%{wks.location}/Binaries/" .. OutputDir .. "/%{prj.name}\" \"%{wks.location}Output\""}
