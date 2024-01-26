#include "smolPCH.h"
#include "Material.h"
#include "Resources.h"

#include "Utils/json/JSONDocument.h"
#include "Shader.h"
#include "Uniform.h"

#include "rapidjson.h"
#include "GL/glew.h"

namespace smol {

    Material::Material(Shader* shader)
    {
		if (!shader)
		{
			size_t id =	smol::Resources::Load<Shader>("resources/shaders/model_color");
			setShader(Resources::GetResourceById<Shader>(id), "resources/shaders/model_color");
		}
		else
			setShader(shader, shader->getPath());
		
    }

    Material::Material(const char* file)
    {
        Load(file);
    }

    Material::~Material()
    {
		if (m_Shader)
			m_Shader->DeleteRef(this);
    }

    void Material::Load(const char* path)
    {
        JSONDocument matFile(path);

		if(matFile.HasMember("shader"))
			m_ShaderPath = matFile["shader"].get<const char*>();
		
		size_t shaderId = Resources::Load<Shader>(m_ShaderPath.c_str());
		Shader* shader = Resources::GetResourceById<Shader>(shaderId);


		setShader(shader, m_ShaderPath.c_str());

		if (matFile.HasMember("uniforms"))
		{
			JSONValue uniforms = matFile["uniforms"];
			rapidjson::Value* val = uniforms.getValue();
			for (rapidjson::Value::MemberIterator p = val->MemberBegin(); p != val->MemberEnd(); ++p)
			{
				Uniform* uniform = getUniform(p->name.GetString());
				if (!uniform)
				{
					return;
				}
				const char* name = uniform->name.c_str();
				switch (uniform->getType())
				{
				case smol::UniformType::Bool:
					uniform->setData(p->value.GetBool(), uniform->getType());
					break;
				case smol::UniformType::Int:
					uniform->setData(p->value.GetInt(), uniform->getType());
					break;
				case smol::UniformType::Uint:
					uniform->setData(p->value.GetUint(), uniform->getType());
					break;
				case smol::UniformType::Float:
					uniform->setData(p->value.GetFloat(), uniform->getType());
					break;
				case smol::UniformType::fVec2:
				{
					glm::vec2 data;
					data.x = p->value["x"].GetFloat();
					data.y = p->value["y"].GetFloat();
					uniform->setData(data, uniform->getType());
				}break;
				case smol::UniformType::fVec3:
				{
					glm::vec3 data;
					data.x = p->value["x"].GetFloat();
					data.y = p->value["y"].GetFloat();
					data.z = p->value["z"].GetFloat();
					uniform->setData(data, uniform->getType());
				}break;
				case smol::UniformType::fVec4:
				{
					glm::vec4 data;
					data.x = p->value["r"].GetFloat();
					data.y = p->value["g"].GetFloat();
					data.z = p->value["b"].GetFloat();
					data.w = p->value["a"].GetFloat();
					uniform->setData(data, uniform->getType());
				}break;
				case smol::UniformType::Sampler2D:
				{
					const char* texPath = p->value.GetString();
					Resources::Import<Texture2D>(texPath);
					size_t id = Resources::Load<Texture2D>(texPath);
					Texture2D* img = Resources::GetResourceById<Texture2D>(id);
					uniform->setData(img->GetTextureId(), uniform->getType());
				}break;
				default:
					break;
				}
				}
			}
        m_MaterialPath = path;
    }

	void Material::Save(const char* path)
	{
		std::string file = path;
		Resources::SetAssetPath(file);
        smol::JSONDocument doc;
		smol::Resources::standarizePath(m_ShaderPath);
		doc.AddMember("shader", m_ShaderPath.c_str());
        smol::JSONValue uniforms = doc.AddMemberObject("uniforms");
		for (smol::Uniform uniform : m_Uniforms)
		{
			const char* name = uniform.name.c_str();
			switch (uniform.getType())
			{
			case smol::UniformType::Bool:
				uniforms.AddMember(name, uniform.getData<bool>());
				break;
			case smol::UniformType::Int:
				uniforms.AddMember(name, uniform.getData<int>());
				break;
			case smol::UniformType::Uint:
				uniforms.AddMember(name, uniform.getData<unsigned int>());
				break;
			case smol::UniformType::Float:
				uniforms.AddMember(name, uniform.getData<float>());
				break;
			case smol::UniformType::fVec2:
			{
				smol::JSONValue vec = uniforms.AddMemberObject(name);
				vec.AddMember("x", uniform.getData<glm::vec2>().x);
				vec.AddMember("y", uniform.getData<glm::vec2>().y);
			}break;
			case smol::UniformType::fVec3:
			{
				smol::JSONValue vec = uniforms.AddMemberObject(name);
				vec.AddMember("x", uniform.getData<glm::vec3>().x);
				vec.AddMember("y", uniform.getData<glm::vec3>().y);
				vec.AddMember("z", uniform.getData<glm::vec3>().z);
			}break;
			case smol::UniformType::fVec4:
			{
				smol::JSONValue vec = uniforms.AddMemberObject(name);
				vec.AddMember("r", uniform.getData<glm::vec4>().r);
				vec.AddMember("g", uniform.getData<glm::vec4>().g);
				vec.AddMember("b", uniform.getData<glm::vec4>().b);
				vec.AddMember("a", uniform.getData<glm::vec4>().a);
			}break;
			case smol::UniformType::Mat2:
			{
				smol::JSONValue mat = uniforms.AddMemberObject(name);

				smol::JSONValue vec1 = mat.AddMemberObject("vec1");
				vec1.AddMember("x", uniform.getData<glm::mat2>()[0].x);
				vec1.AddMember("y", uniform.getData<glm::mat2>()[0].y);

				smol::JSONValue vec2 = mat.AddMemberObject("vec2");
				vec2.AddMember("x", uniform.getData<glm::mat2>()[1].x);
				vec2.AddMember("y", uniform.getData<glm::mat2>()[1].y);
			}break;
			case smol::UniformType::Mat3:
			{
				smol::JSONValue mat = uniforms.AddMemberObject(name);

				smol::JSONValue vec1 = mat.AddMemberObject("vec1");
				vec1.AddMember("x", uniform.getData<glm::mat3>()[0].x);
				vec1.AddMember("y", uniform.getData<glm::mat3>()[0].y);
				vec1.AddMember("z", uniform.getData<glm::mat3>()[0].z);

				smol::JSONValue vec2 = mat.AddMemberObject("vec2");
				vec2.AddMember("x", uniform.getData<glm::mat3>()[1].x);
				vec2.AddMember("y", uniform.getData<glm::mat3>()[1].y);
				vec2.AddMember("z", uniform.getData<glm::mat3>()[1].z);

				smol::JSONValue vec3 = mat.AddMemberObject("vec3");
				vec3.AddMember("x", uniform.getData<glm::mat3>()[2].x);
				vec3.AddMember("y", uniform.getData<glm::mat3>()[2].y);
				vec3.AddMember("z", uniform.getData<glm::mat3>()[2].z);
			}break;
			case smol::UniformType::Mat4:
			{
				smol::JSONValue mat = uniforms.AddMemberObject(name);

				smol::JSONValue vec1 = mat.AddMemberObject("vec1");
				vec1.AddMember("x", uniform.getData<glm::mat4>()[0].x);
				vec1.AddMember("y", uniform.getData<glm::mat4>()[0].y);
				vec1.AddMember("z", uniform.getData<glm::mat4>()[0].z);
				vec1.AddMember("w", uniform.getData<glm::mat4>()[0].w);

				smol::JSONValue vec2 = mat.AddMemberObject("vec2");
				vec2.AddMember("x", uniform.getData<glm::mat4>()[1].x);
				vec2.AddMember("y", uniform.getData<glm::mat4>()[1].y);
				vec2.AddMember("z", uniform.getData<glm::mat4>()[1].z);
				vec2.AddMember("w", uniform.getData<glm::mat4>()[1].w);

				smol::JSONValue vec3 = mat.AddMemberObject("vec3");
				vec3.AddMember("x", uniform.getData<glm::mat4>()[2].x);
				vec3.AddMember("y", uniform.getData<glm::mat4>()[2].y);
				vec3.AddMember("z", uniform.getData<glm::mat4>()[2].z);
				vec3.AddMember("w", uniform.getData<glm::mat4>()[2].w);

				smol::JSONValue vec4 = mat.AddMemberObject("vec4");
				vec4.AddMember("x", uniform.getData<glm::mat4>()[2].x);
				vec4.AddMember("y", uniform.getData<glm::mat4>()[2].y);
				vec4.AddMember("z", uniform.getData<glm::mat4>()[2].z);
				vec4.AddMember("w", uniform.getData<glm::mat4>()[2].w);
			}break;
			case smol::UniformType::Sampler2D:
			{
				size_t id = uniform.getData<glm::ivec2>().y;
				std::string string = Resources::getResourcePathById<Texture2D>(id);
				smol::Resources::standarizePath(string);
				uniforms.AddMember(name, string.c_str());
			}break;
			default:
				break;
			}
		}
        doc.save_file(file.c_str());
		m_MaterialPath = file;
		Resources::Import<Material>(m_MaterialPath.c_str());
	}

	void Material::SaveMaterial(const char* path, Material* mat)
    {
        mat->Save(path);
    }

    Material* Material::LoadMaterial(const char* path)
    {
        return new Material(path);
    }
    void Material::Refresh()
    {
		std::vector<UniformField>& fields = m_Shader->getUniforms();
		for (UniformField& field : fields)
		{
			
			Uniform* uniform = getUniform(field.name.c_str());
			if (uniform)
			{
				if (uniform->getType() != field.type)
				{
					m_Uniforms.erase(m_Uniforms.begin() + getUniformIndex(uniform->name.c_str()));
					m_Uniforms.emplace_back(field.name, field.type);
				}
			}
			else
			{
				m_Uniforms.emplace_back(field.name, field.type);
			}
		}

		for (size_t i = 0; i < m_Uniforms.size(); i++)
		{
			size_t j = 0;
			for (; j < fields.size(); j++)
			{
				if (m_Uniforms[i].name == fields[j].name
					&& m_Uniforms[i].getType() == fields[j].type)	
					break;
			}
			if (j == fields.size())
			{
				m_Uniforms.erase(m_Uniforms.begin() + i);
				i--;
			}
		}
    }

	void Material::setShader(Shader* shader, const char* path)
	{
		m_Uniforms.clear();
		if (m_Shader)
			m_Shader->DeleteRef(this);

		m_Shader = shader;
		m_Shader->AddRef(this);
		m_ShaderPath = path;
		Refresh();
	}

	smol::Uniform* Material::getUniform(const char* name)
	{
		for (size_t i = 0; i < m_Uniforms.size(); i++)
		{
			if (m_Uniforms[i].name == name)
			{
				return &m_Uniforms[i];
			}
		}
		return nullptr;
	}

	void Material::Bind()
	{
		int texureId = GL_TEXTURE0;
		for(size_t i = 0; i < m_Uniforms.size(); i++)
		{
			m_Uniforms[i].sendToShader(m_Shader->getID(), texureId);
		}
	}

	void Material::UnBind()
	{
		m_Shader->UnBind();
	}

}