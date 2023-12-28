-- You can put your dependencies here
IncludeDir = {}
IncludeDir["ImGui"] = "%{wks.location}/Vendor/imgui/"
IncludeDir["SDL"] = "%{wks.location}/Vendor/SDL/include/"
IncludeDir["IconFontCppHeaders"] = "%{wks.location}/Vendor/IconFontCppHeaders/"
IncludeDir["assimp"] = "%{wks.location}/Vendor/assimp/include/"
IncludeDir["glew"] = "%{wks.location}/Vendor/Glew/include/"
IncludeDir["glm"] = "%{wks.location}/Vendor/glm/glm/"
IncludeDir["DevIL"] = "%{wks.location}/Vendor/DevIL/include/"
IncludeDir["SPDLOG"] = "%{wks.location}/Vendor/spdlog/include"



group "Dependencies"
include "Vendor/ImGui"
include "Vendor/SDL/SDL2.lua"
include "Vendor/SDL/SDL2main.lua"
group ""
