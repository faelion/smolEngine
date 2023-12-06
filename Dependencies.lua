-- You can put your dependencies here
IncludeDir = {}
IncludeDir["ImGui"] = "../Vendor/imgui/"
IncludeDir["SDL"] = "../Vendor/SDL/include/"
IncludeDir["IconFontCppHeaders"] = "../Vendor/IconFontCppHeaders/"
IncludeDir["assimp"] = "../Vendor/assimp/include/"
IncludeDir["glew"] = "../Vendor/Glew/include/"
IncludeDir["glm"] = "../Vendor/glm/glm/"
IncludeDir["spdlog"] = "../Vendor/spdlog/include/"



group "Dependencies"
include "Vendor/ImGui"
include "Vendor/SDL/SDL2.lua"
include "Vendor/SDL/SDL2main.lua"
group ""
