-- You can put your dependencies here
IncludeDir = {}
IncludeDir["ImGui"] = "%{wks.location}/Vendor/ImGui/"
IncludeDir["backends"] = "%{wks.location}/Vendor/ImGui/backends"
IncludeDir["SDL"] = "%{wks.location}/Vendor/SDL/include/"
IncludeDir["IconFontCppHeaders"] = "%{wks.location}/Vendor/IconFontCppHeaders/"
IncludeDir["assimp"] = "%{wks.location}/Vendor/assimp/include"
IncludeDir["Glew"] = "%{wks.location}/Vendor/Glew/include"
IncludeDir["GLM"] = "%{wks.location}/Vendor/glm"
IncludeDir["SPDLOG"] = "%{wks.location}/Vendor/spdlog/include"
IncludeDir["JSON"] = "%{wks.location}/Vendor/rapidjson"
IncludeDir["STB"] = "%{wks.location}/Vendor/stb"
IncludeDir["GLI"] = "%{wks.location}/Vendor/gli/gli"
IncludeDir["ENTT"] = "%{wks.location}/Vendor/entt/include"


group "Dependencies"
include "Vendor/ImGui"
include "Vendor/SDL/SDL2"
include "Vendor/SDL/SDL2main"
group ""
