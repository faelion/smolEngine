#pragma once

#include "glm/glm.hpp"

namespace smol {

	enum class ImageFormat
	{
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	class Texture2D
	{
	public:
		Texture2D();
		Texture2D(const std::string& path, bool compressed = false);
		~Texture2D();

		uint32_t GetWidth() const { return m_Width;  }
		uint32_t GetHeight() const { return m_Height; }
		uint32_t GetTextureId() const { return m_TextureID; }
		glm::ivec2 GetSize() { return m_ImageSize; }

		const std::string& GetPath() const { return m_Path; }
		
		void SetData(void* data, uint32_t size);

		void Bind(uint32_t slot = 0) const;

		bool IsLoaded() const { return m_IsLoaded; }

		bool operator==(const Texture2D& other) const
		{
			return m_TextureID == other.GetTextureId();
		}

		//utils
		static void raw_to_dds_file(const char* filename, const unsigned char* pData, int width, int height, int bpp);

	private:

		void Init(const std::string& path);
		void InitDDS(const std::string& path);

		std::string m_Path;
		bool m_IsLoaded = false;
		uint32_t m_Width, m_Height;
		uint32_t m_TextureID;
		unsigned int m_InternalFormat, m_DataFormat;
		glm::ivec2 m_ImageSize;
	};

}
