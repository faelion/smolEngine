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
	  "%{IncludeDir.SPDLOG}",
      "%{IncludeDir.SDL}", 
      "%{IncludeDir.ImGui}", 
      "%{IncludeDir.IconFontCppHeaders}",
      "%{IncludeDir.assimp}",
      "%{IncludeDir.glew}",
      "%{IncludeDir.glm}",
      "%{IncludeDir.DevIL}"
   }

   links
   {
      "ImGui",
      "SDL2", 
      "SDL2main", 
	  "opengl32.lib",
	  "../Vendor/Glew/lib/glew32.lib",
	  "../Vendor/DevIL/lib/DevIL.lib",
	  "../Vendor/DevIL/lib/ILU.lib",
	  "../Vendor/DevIL/lib/ILUT.lib",
	  "../Vendor/assimp/lib/x64/release/assimp-vc143-mt.lib"
   }

   postbuildcommands ("{COPY} %{cfg.buildtarget.relpath} ../Output")

   debugdir "$(SolutionDir)/Output"

   -- Only works in windows actually
   filter "system:windows"
       systemversion "latest"
       defines {
       "SMOL_PLATFORM_WINDOWS"
       }

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