-- You can put your dependencies here
IncludeDir = {}
IncludeDir["ImGui"] = "../Vendor/ImGui/"
IncludeDir["SDL"] = "../Vendor/SDL/include/"
IncludeDir["IconFontCppHeaders"] = "../Vendor/IconFontCppHeaders/"
IncludeDir["assimp"] = "../Vendor/assimp/include"
IncludeDir["Glew"] = "../Vendor/Glew/include"
IncludeDir["GLM"] = "../Vendor/glm/glm"
IncludeDir["DevIL"] = "../Vendor/DevIL/include"
IncludeDir["SPDLOG"] = "../Vendor/spdlog/include"



group "Dependencies"
include "Vendor/ImGui"
include "Vendor/SDL/SDL2"
include "Vendor/SDL/SDL2main"
group ""
