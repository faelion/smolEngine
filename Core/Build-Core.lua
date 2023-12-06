project "smolEngine"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp" }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   includedirs
   {
      "Source",
      "%{IncludeDir.SDL}", 
      "%{IncludeDir.ImGui}", 
      "%{IncludeDir.IconFontCppHeaders}",
      "%{IncludeDir.assimp}",
      "%{IncludeDir.glew}",
      "%{IncludeDir.glm}",
      "%{IncludeDir.spdlog}"
   }

   links
   {
      "ImGui",
      "SDL2", 
      "SDL2main", 
	  "opengl32.lib",
	  "../Vendor/Glew/lib/glew32.lib",
	  "../Vendor/assimp/lib/x64/release/assimp-vc143-mt.lib"
   }

   filter "system:windows"
       systemversion "latest"
       defines { }

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