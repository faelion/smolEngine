-- You can put your dependencies here
IncludeDir = {}
IncludeDir["ImGui"] = "%{wks.location}/Vendor/ImGui/"
IncludeDir["backends"] = "%{wks.location}/Vendor/ImGui/backends"
IncludeDir["SDL"] = "%{wks.location}/Vendor/SDL/include/"
IncludeDir["IconFontCppHeaders"] = "%{wks.location}/Vendor/IconFontCppHeaders/"
IncludeDir["assimp"] = "%{wks.location}/Vendor/assimp/include"
IncludeDir["Glew"] = "%{wks.location}/Vendor/Glew/include"
IncludeDir["GLM"] = "%{wks.location}/Vendor/glm/glm"
IncludeDir["DevIL"] = "%{wks.location}/Vendor/DevIL/include"
IncludeDir["SPDLOG"] = "%{wks.location}/Vendor/spdlog/include"



group "Dependencies"
include "Vendor/ImGui"
include "Vendor/SDL/SDL2"
include "Vendor/SDL/SDL2main"
group ""
