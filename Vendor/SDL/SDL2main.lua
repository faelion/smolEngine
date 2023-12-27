project "SDL2main"
	kind          "StaticLib"
	language      "C"

	systemversion "latest"

	flags {
		          "NoRuntimeChecks",      -- Only used on Visual Studio.
		          "NoBufferSecurityCheck"
	}

	vectorextensions "SSE"                -- Necessary to run x32.

	location         "Intermediate/ProjectFiles/%{_ACTION}"
	
	targetdir        "Binaries/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	objdir           "Intermediate/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"

	includedirs     {"include"}

	files           {"src/main/windows/*.c"}	

	systemversion    "latest"
	defines {
					 "_WINDOWS",
					 "WIN32"
	}

   filter "configurations:Debug"
		defines {
			         "_DEBUG"
		}
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
		defines {
			         "NDEBUG"
		}
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
		defines {
			         "NDEBUG"
		}
       runtime "Release"
       optimize "On"
       symbols "Off"
