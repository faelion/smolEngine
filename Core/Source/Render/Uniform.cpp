#include "smolPCH.h"
#include "Uniform.h"
#include <GL/glew.h>

#include <glm/glm.hpp>

namespace smol {
	//Sampler2D x : OpenGL id y: ResourceId to retrieve the path when saving
	void Uniform::sendToShader(uint16_t shaderProgram, int& textureId)
	{
		if(m_UniformID == NULL)
			m_UniformID = glGetUniformLocation(shaderProgram, name.c_str());

		switch (m_Type)
		{
		case smol::UniformType::Bool:
			glUniform1i(m_UniformID, *static_cast<bool*>(m_Data));
			break;
		case smol::UniformType::Int:
			glUniform1i(m_UniformID, *static_cast<int*>(m_Data));
			break;
		case smol::UniformType::Uint:
			glUniform1ui(m_UniformID, *static_cast<unsigned int*>(m_Data));
			break;
		case smol::UniformType::Float:
			glUniform1f(m_UniformID, *static_cast<float*>(m_Data));
			break;
		case smol::UniformType::fVec2:
			glUniform2fv(m_UniformID, 1, static_cast<float*>(m_Data));
			break;
		case smol::UniformType::fVec3:
			glUniform3fv(m_UniformID, 1, static_cast<float*>(m_Data));
			break;
		case smol::UniformType::fVec4:
			glUniform4fv(m_UniformID, 1, static_cast<float*>(m_Data));
			break;
		case smol::UniformType::Mat2:
			glUniformMatrix2fv(m_UniformID, 1, GL_FALSE, static_cast<float*>(m_Data));
			break;
		case smol::UniformType::Mat3:
			glUniformMatrix3fv(m_UniformID, 1, GL_FALSE, static_cast<float*>(m_Data));
			break;
		case smol::UniformType::Mat4:
			glUniformMatrix4fv(m_UniformID, 1, GL_FALSE, static_cast<float*>(m_Data));
			break;
		case smol::UniformType::Sampler2D:
		{
			glActiveTexture(textureId++);
			glBindTexture(GL_TEXTURE_2D, ((glm::ivec2*)m_Data)->x);
		}break;
		default:
			break;
		}
	}

	void Uniform::setEmptyData()
	{
		switch (m_Type)
		{
		case smol::UniformType::Bool:
			setData(false, m_Type);
			break;
		case smol::UniformType::Int:
			setData(0, m_Type);
			break;
		case smol::UniformType::Uint:
			setData(0, m_Type);
			break;
		case smol::UniformType::Float:
			setData(0.0f, m_Type);
			break;
		case smol::UniformType::fVec2:
			setData(glm::vec2(), m_Type);
			break;
		case smol::UniformType::fVec3:
			setData(glm::vec3(), m_Type);
			break;
		case smol::UniformType::fVec4:
			setData(glm::vec4(), m_Type);
			break;
		case smol::UniformType::Mat2:
			setData(glm::mat2(), m_Type);
			break;
		case smol::UniformType::Mat3:
			setData(glm::mat3(), m_Type);
			break;
		case smol::UniformType::Mat4:
			setData(glm::mat4(), m_Type);
			break;
		case smol::UniformType::Sampler2D:
			setData(glm::ivec2(), m_Type);
			break;
		default:
			break;
		}
	}

}