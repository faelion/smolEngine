project "smolEditor"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
      "Source",
      "%{IncludeDir.SDL}", 
	  "%{IncludeDir.SPDLOG}",
	  "%{IncludeDir.ImGui}",
      "%{IncludeDir.IconFontCppHeaders}",

	  -- Include Core
	  "../Core/Source"
   }

   links
   {
      "smolEngine",
      "ImGui",
      "SDL2main"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")
   
   postbuildcommands {"{COPY} \"%{wks.location}/Binaries/" .. OutputDir .. "/%{prj.name}\" \"%{wks.location}Output\""}

   debugdir "$(SolutionDir)/Output"

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