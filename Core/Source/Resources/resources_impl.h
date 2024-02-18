#pragma once
#pragma warning(disable : 4251)

// Resources
#include <Render/Shader.h>
#include <Render/Texture.h>
#include <Render/Model.h>
#include <Render/Material.h>
#include <Utils/json/JSONDocument.h>
#include <Utils/json/JSONValue.h>

#include <Log.h>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <time.h>

#define RES_NOT_LOADED -1

typedef size_t ResourceId;

namespace smol {
	enum class CompressionType {
		NONE = 0,
		RGB_DXT1,
		RGBA_DXT1,
		RGBA_DXT3,
		RGBA_DXT5
	};
	struct ImageSettings {
		CompressionType Compression = CompressionType::RGBA_DXT5;
		bool Interlaced = false;
		int32_t OffsetX = 0;
		int32_t OffsetY = 0;
		bool MipMapping = true;
		bool Anystropy = true;
		bool MaxAnystropy = true;
		bool BlurGausian = false;
		bool BlurAverage = false;
		bool Contrast = false;
		int32_t AmountOfContrast = 0;
		bool Alienify = false;
		bool GammaCorrection = false;
		bool Noise = false;
		bool Equialize = false;
		bool Negative = false;
		bool Pixelize = false;
		bool Sharpen = false;
		int32_t SharpenFactor = 0;
		int32_t SharpenIterations = 0;
		bool Scale = 0;
		ImageSettings() = default;
	};

	struct ModelSettings {
		bool preTranslatedVertices = false;
		ModelSettings() = default;
	};
	class Resources
	{
	public:
		// Enum that indicates all the resources that can be loaded by the engine
		enum ResourceType {
			SRT_IMAGE,
			SRT_SHADER,
			SRT_MODEL,
			SRT_MATERIAL,
			SRT_LAST
		};
		enum MetaResult
		{
			NOTFOUND,
			DELETED,
			UPDATED,
			TOUPDATE
		};
		struct Resource {
			// Path to resource
			std::string filePath;
			bool isNative;
			// Any type
			void* resource;

			Resource() = default;
		};

	private:
		Resources();

		static std::vector<Resource*> m_Resources[SRT_LAST];

		static void PushResource(ResourceType rt, const char* file, void* rsc, bool isNative = false);
		static ResourceId getResourcePosition(ResourceType rt, const char* file);

		// Resource path for importing
		inline static const char* getPathById(ResourceType type, size_t id)
		{
			if (id >= m_Resources[type].size())
				return "";
			
			return m_Resources[type][id]->filePath.c_str();
		}
		
		
		// Implementations
		static void _import_image_impl(const char* origin, const char* destination);
		static void _import_model_impl(const char* origin, const char* destination, ModelSettings* settings);
		static bool _check_import_impl(const char* file, const char* extension);
		
	public:
		static bool _preparePath(std::string path);
		static std::string _assetToLibPath(std::string path);
		static std::filesystem::path _import_path_impl(const std::filesystem::path& path, const char* extension);
		template <typename TP>
		static inline std::time_t to_time_t(TP tp)
		{
			using namespace std::chrono;
			auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
				+ system_clock::now());
			return system_clock::to_time_t(sctp);
		}
		static inline void standarizePath(std::string& file_path)
		{
			size_t index = 0;

			while ((index = file_path.find('\\', index)) != std::string::npos) {
				file_path.replace(index, 1, "/");
				index++;
			}
			_toLower(file_path);
		}


		static void _toLower(std::string& path);
		inline static std::vector<Resource*>& GetResourcesOf(ResourceType rt) { return m_Resources[rt]; }
		static void UnloadSceneResources();

		// Returns the state of the meta file
		// Deleted: the meta file is deleted due to the reference file not existing
		// Updated: the meta files are up to date
		// ToUpdate: the meta last updated doesnt equal the file reference
		// Not found: there's no meta file for the reference
		static MetaResult CheckMeta(const char* filename);
		static void UpdateMeta(const char* filename);
		template<class T> static const char* getResourcePathById(size_t id);

		template<class T> static ResourceId Load(const char* file);
		template<class T> static ResourceId LoadNative(const char* file);
		template<class T> static T* GetResourceById(ResourceId id);
		template<class T> static bool CheckImport(const char* file);
		template<class T, class... T2> static void Import(const char* file, T2... settings);
		template<class T, class... T2> static void CreateMeta(const char* file, T2... settings);
		template<class T, class... T2> static void LoadMeta(const char* file, T2... settings);

		inline static void SaveFile(const char* file, std::string& shaderFile)
		{
			std::string path = SetLibraryPath(file);

			std::ofstream outFile(path.c_str(), std::ios::out | std::ios::binary);
			outFile.write(shaderFile.c_str(), shaderFile.size());
			outFile.close();
		}

		inline static std::string SetLibraryPath(const char* file)
		{
			std::string path = "library/";
			path += file;
			size_t index = path.rfind('/');
			path = path.substr(0, index);
			if (!std::filesystem::exists(path))
			{
				if (std::filesystem::create_directories(path))
					SMOL_CORE_ERROR("Can't create a folder at {0}", path.c_str());
			}
			path = "library/";
			path += file;
			path += ".smolasset";

			return path;
		}
		static void SetAssetPath(std::string& path);
		inline static std::string* getFileData(const char* file)
		{
			std::fstream shaderFile;

			shaderFile.open(file, std::ios::in);

			if (!shaderFile.is_open()) return NULL;

			std::stringstream buffer;

			buffer << shaderFile.rdbuf();

			shaderFile.close();

			return new std::string(buffer.str());
		}
		static void Clear();
		
	};
}