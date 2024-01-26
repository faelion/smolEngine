project "smolEngine"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp" }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   pchheader "smolPCH.h"
   pchsource "Source/smolPCH.cpp"

   includedirs
   {
      "Source",
      "%{IncludeDir.SDL}", 
	  "%{IncludeDir.SPDLOG}",
      "%{IncludeDir.ImGui}", 
      "%{IncludeDir.GLM}",
      "%{IncludeDir.backends}", 
      "%{IncludeDir.IconFontCppHeaders}",
      "%{IncludeDir.assimp}",
      "%{IncludeDir.Glew}",
	  "%{IncludeDir.JSON}",
	  "%{IncludeDir.STB}",
	  "%{IncludeDir.GLI}"
   }

   links
   {
      "SDL2", 
      "SDL2main",
      "ImGui",
	  "opengl32.lib",
	  "../Vendor/Glew/lib/glew32.lib",
	  "../Vendor/assimp/lib/x64/release/assimp-vc143-mt.lib"
   }
   
   postbuildcommands {"{COPY} \"%{wks.location}/Binaries/" .. OutputDir .. "/%{prj.name}\" \"%{wks.location}Output\""}

   debugdir "$(SolutionDir)/Output"

   -- Only works in windows actually
   filter "system:windows"
       systemversion "latest"
       defines {
       "SMOL_PLATFORM_WINDOWS"
       }

   filter "configurations:Debug"
       defines { 
           "DEBUG" ,
           "SMOL_ENABLE_ASSERTS"
       }
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