#include <smolPCH.h>

#include "Model.h"
#include <fstream>
#include <string>

#include <GL/glew.h>

#include <Application.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include <Utils/filesystem/FileSystem.h>
#include <Resources.h>

namespace smol {
	void Model::getMeshFromFile(const char* file, ModelSettings* settings, bool gen_buffers)
	{
		unsigned int flags = aiProcess_Triangulate | aiProcess_FlipUVs;//aiProcessPreset_TargetRealtime_Quality | aiProcess_FlipUVs;//aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs;

		if (settings->preTranslatedVertices) {
			//flags |= aiProcess_PreTransformVertices;
		}

		sbyte* file_data = NULL;
		uint32_t file_buf_size = FileSystem::ReadAll(file, &file_data);

		const aiScene* scene = aiImportFileFromMemory(file_data, file_buf_size, flags, NULL);

		delete[] file_data;

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			SMOL_ERROR("Couldn't load mesh file: {0}", file);
			return;
		}

		SMOL_CORE_INFO("Loading mesh file at: {0} ...", file);
		
		is_root = true;

		m_ModelHierarchy = loadModelHierarchy(scene->mRootNode);
		std::filesystem::path p = file;
		m_ModelHierarchy->m_Name = p.stem().string();

		m_ModelPath = file;

		m_ModelName = scene->mRootNode->mName.C_Str();

		std::filesystem::path path = file;
		path.remove_filename();

		std::filesystem::path curr_path = std::filesystem::current_path();

		// Process materials
		if (scene->HasMaterials()) {
			for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
				aiMaterial* mat = scene->mMaterials[i];

				aiString name;
				aiGetMaterialString(mat, AI_MATKEY_NAME, &name);

				aiString texture_diffuse;
				aiGetMaterialString(mat, AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), &texture_diffuse);

				aiColor4D diffuse(1.0f, 1.0f, 1.0f, 1.0f);
				aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse);

				aiColor4D specular(1.0f, 1.0f, 1.0f, 1.0f);
				aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &specular);

				float shininess = 0.1f;
				aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &shininess);
				
				std::filesystem::path mat_path = path;
				mat_path += name.C_Str();
				mat_path += ".smolmaterial";

				ResourceId matID = Resources::Load<Material>(mat_path.string().c_str());
				if (matID == -1)
				{
					Material material; // Default settings
					size_t id;

					if (texture_diffuse.length > 0)
					{

						std::filesystem::current_path(path);

						std::filesystem::path texture_path = texture_diffuse.C_Str();
						texture_path = std::filesystem::absolute(texture_path);

						std::filesystem::current_path(curr_path);

						texture_path = std::filesystem::relative(texture_path);

						id = Resources::Load<Shader>("resources/shaders/model_texture");
						material.setShader(Resources::GetResourceById<Shader>(id), "resources/shaders/model_texture");
						Resources::Import<Texture2D>(texture_path.string().c_str());
						uint32_t imgId = Resources::Load<Texture2D>(texture_path.string().c_str());
						Texture2D* img = Resources::GetResourceById<Texture2D>(imgId);
						material.SetUniformData("u_Tex0", glm::ivec2(img->GetTextureId(), imgId));
					}
					else
					{
						//Set the color of the material
						id = Resources::Load<Shader>("resources/shaders/model_color");
						material.setShader(Resources::GetResourceById<Shader>(id), "resources/shaders/model_color");
						material.SetUniformData("u_Color", glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a));
					}


					Material::SaveMaterial(mat_path.string().c_str(), &material);
				}
				m_Materials.push_back(mat_path.string());
			}
		}

		// Load mesh list
		if (scene->HasMeshes())
		{
			for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
				Model* model = loadmesh(scene->mMeshes[i]);

				if (gen_buffers) { 
					model->generateBuffers();
				}

				m_ChildModels.push_back(model);
			}
		}

		aiReleaseImport(scene);
	}

	void Model::getSmolMeshFromFile(const char* file)
	{
		is_root = true;

		File f = FileSystem::OpenIB(file);

		if (f.IsOpen()) {
			m_ModelPath = file;

			size_t model_size;

			// Model size
			f.Read(&model_size, sizeof(size_t));

			m_ChildModels.reserve(model_size);

			// Material size
			size_t mat_size;

			f.Read(&mat_size, sizeof(size_t));

			m_Materials.resize(mat_size);

			for (size_t i = 0; i < mat_size; i++) {
				size_t name_size;

				f.Read(&name_size, sizeof(size_t));
				m_Materials[i].resize(name_size);

				f.Read(&m_Materials[i][0], name_size);
			}

			// Children models
			for (size_t i = 0; i < model_size; i++) {
				Model* model = new Model(NULL);

				// Read material index
				f.Read(&model->m_ModelMat, sizeof(size_t));

				// Read vbo
				size_t vbo_size;
				f.Read(&vbo_size, sizeof(size_t));
				model->m_MeshVBO_data.resize(vbo_size);
				f.Read(&model->m_MeshVBO_data[0], vbo_size * sizeof(float));

				// Read ebo
				size_t ebo_size;
				f.Read(&ebo_size, sizeof(size_t));
				model->m_MeshIBO_data.resize(ebo_size);
				f.Read(&model->m_MeshIBO_data[0], ebo_size * sizeof(int));

				model->generateBuffers();
				m_ChildModels.push_back(model);
			}

			m_ModelHierarchy = LoadModelHierarchy(f);
		}

		f.Close();
	}

	Model* Model::loadmesh(const aiMesh* mesh)
	{
		Model* model = new Model(NULL);

		model->m_ModelMat = mesh->mMaterialIndex;

		model->m_ModelName = mesh->mName.C_Str();
		for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
			// Vertices
			model->m_MeshVBO_data.push_back(mesh->mVertices[j].x);
			model->m_MeshVBO_data.push_back(mesh->mVertices[j].y);
			model->m_MeshVBO_data.push_back(mesh->mVertices[j].z);
			// Normals
			model->m_MeshVBO_data.push_back(mesh->mNormals[j].x);
			model->m_MeshVBO_data.push_back(mesh->mNormals[j].y);
			model->m_MeshVBO_data.push_back(mesh->mNormals[j].z);
			// Texture coordinates
			if (mesh->mTextureCoords[0])
			{
				model->m_MeshVBO_data.push_back(mesh->mTextureCoords[0][j].x);
				model->m_MeshVBO_data.push_back(mesh->mTextureCoords[0][j].y);
			}
			else {
				model->m_MeshVBO_data.push_back(0.0f);
				model->m_MeshVBO_data.push_back(0.0f);
			}
		}
		if (model->m_MeshVBO_data.empty())
		{
			SMOL_CORE_ERROR("Error while loading mesh vertex buffer");
		}
		else
		{
			SMOL_CORE_INFO("Vertex buffer generated correctly");
		}
		// Indices
		for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
			aiFace& face = mesh->mFaces[j];
			for (unsigned int k = 0; k < face.mNumIndices; k++) {
				model->m_MeshIBO_data.push_back(face.mIndices[k]);
			}
		}
		if (model->m_MeshIBO_data.empty())
		{
			SMOL_CORE_ERROR("Error while loading mesh index buffer");
		}
		else
		{
			SMOL_CORE_INFO("Index buffer generated correctly");
		}
		
		return model;
	}

	ModelHierarchy* Model::loadModelHierarchy(const aiNode* node)
	{
		ModelHierarchy* h = new ModelHierarchy();

		h->m_Name = node->mName.C_Str();

		
		// Node transform
		aiVector3D translate, scale, rot;

		node->mTransformation.Decompose(scale, rot, translate);

		h->translation = { translate.x, translate.y, translate.z };
		h->rotation = { rot.x * 180.0f / PI_F, rot.y * 180.0f / PI_F, rot.z * 180.0f / PI_F };
		h->scale = { scale.x, scale.y, scale.z };

		// Node meshes
		for (size_t i = 0; i < node->mNumMeshes; i++) {
			h->m_MeshIndexes.push_back(node->mMeshes[i]);
		}

		// Node children
		for (size_t i = 0; i < node->mNumChildren; i++) {
			h->m_Children.push_back(loadModelHierarchy(node->mChildren[i]));
		}

		return h;
	}

	void Model::CreateCube()
	{
		m_MeshVBO_data = {
			1,1,1, 0,0,1, 0.625,0.5,
			-1,1,1, 0,0,1, 0.875,0.5,
			-1,-1,1, 0,0,1, 0.875,0.25,
			1,-1,1, 0,0,1, 0.625,0.25,
			1,-1,-1, 0,-1,0, 0.375,0.25,
			1,-1,1, 0,-1,0, 0.625,0.25,
			-1,-1,1, 0,-1,0, 0.625,0,
			-1,-1,-1, 0,-1,0, 0.375,0,
			-1,-1,-1, -1,0,0, 0.375,1,
			-1,-1,1, -1,0,0, 0.625,1,
			-1,1,1, -1,0,0, 0.625,0.75,
			-1,1,-1, -1,0,0, 0.375,0.75,
			-1,1,-1, 0,0,-1, 0.125,0.5,
			1,1,-1, 0,0,-1, 0.375,0.5,
			1,-1,-1, 0,0,-1, 0.375,0.25,
			-1,-1,-1, 0,0,-1, 0.125,0.25,
			1,1,-1, 1,0,0, 0.375,0.5,
			1,1,1, 1,0,0, 0.625,0.5,
			1,-1,1, 1,0,0, 0.625,0.25,
			1,-1,-1, 1,0,0, 0.375,0.25,
			-1,1,-1, 0,1,0, 0.375,0.75,
			-1,1,1, 0,1,0, 0.625,0.75,
			1,1,1,	0,1,0, 0.625,0.5,
			1,1,-1, 0,1,0, 0.375,0.5
		};

		m_MeshIBO_data = {
			0,1,2,0,2,3,
			4,5,6,4,6,7,
			8,9,10,8,10,11,
			12,13,14,12,14,15,
			16,17,18,16,18,19,
			20,21,22,20,22,23
		};
		is_root = false;
		generateBuffers();
	}

	void Model::CreatePlane()
	{
		m_MeshVBO_data = {
			-0.50,-0.50,0.00,0.00,0.00,1.00,0.00,1.00
			,0.50,-0.50,0.00,0.00,0.00,1.00,1.00,1.00,
			0.50,0.50,0.00,0.00,0.00,1.00,1.00,0.00,
			-0.50,0.50,0.00,0.00,0.00,1.00,0.00,
			- 0.50,-0.50,0.00,0.00,0.00,1.00,0.00,
			1.00,0.50,-0.50,0.00,0.00,0.00,1.00,1.00
			,1.00,0.50,0.50,0.00,0.00,0.00,1.00,1.00,
			0.00,-0.50,0.50,0.00,0.00,0.00,1.00,0.00,0.00
		};

		m_MeshIBO_data = {
			0,1,2,0,2,3
		};

		is_root = false;
		generateBuffers();
	}

	void Model::CreatePyramid()
	{
		m_MeshVBO_data = {
			0.00f,1.00f,-1.00f,0.67f,0.67f,0.33f,0.25f,0.51f,0.00f,0.00f,1.00f,0.67,0.67,0.33,0.25,0.75,1.00,-0.00,-1.00,0.67,0.67,0.33,0.49,0.75,1.00,-0.00,-1.00,0.67,-0.67,0.33,0.49,0.75,0.00,0.00,1.00,0.67,-0.67,0.33,0.25,0.75,-0.00,-1.00,-1.00,0.67,-0.67,0.33,0.25,0.99,0.00,1.00,-1.00,0.00,0.00,-1.00,0.75,0.51,1.00,-0.00,-1.00,0.00,0.00,-1.00,0.99,0.75,-0.00,-1.00,-1.00,0.00,0.00,-1.00,0.75,0.99,-1.00,0.00,-1.00,0.00,0.00,-1.00,0.51,0.75,-0.00,-1.00,-1.00,-0.67,-0.67,0.33,0.25,0.99,0.00,0.00,1.00,-0.67,-0.67,0.33,0.25,0.75,-1.00,0.00,-1.00,-0.67,-0.67,0.33,0.01,0.75,-1.00,0.00,-1.00,-0.67,0.67,0.33,0.01,0.75,0.00,0.00,1.00,-0.67,0.67,0.33,0.25,0.75,0.00,1.00,-1.00,-0.67,0.67,0.33,0.25,0.51
		};

		m_MeshIBO_data = {
			0,1,2,3,4,5,6,7,8,6,8,9,10,11,12,13,14,15
		};
		is_root = false;
		generateBuffers();
	}

	void Model::CreateSphere()
	{
		int stack_count = 18;
		int sector_count = 36;
		int radius = 1;

		float x, y, z, xy;
		float sectorStep = 2 * PI_F / sector_count;
		float stackStep = PI_F / stack_count;
		float sectorAngle, stackAngle;

		// Generate vertices
		for (int i = 0; i <= stack_count; ++i)
		{
			stackAngle = (float)PI / 2 - i * stackStep;
			xy = radius * cosf(stackAngle);
			z = radius * sinf(stackAngle);

			for (int j = 0; j <= sector_count; ++j)
			{
				sectorAngle = j * sectorStep;

				x = xy * cosf(sectorAngle);
				y = xy * sinf(sectorAngle);
				m_MeshVBO_data.push_back(x);
				m_MeshVBO_data.push_back(y);
				m_MeshVBO_data.push_back(z);
			}
		}

		// Generate indices
		int k1, k2;
		for (int i = 0; i < stack_count; ++i)
		{
			k1 = i * (sector_count + 1);     // beginning of current stack
			k2 = k1 + sector_count + 1;      // beginning of next stack

			for (int j = 0; j < sector_count; ++j, ++k1, ++k2)
			{
				if (i != 0)
				{
					m_MeshIBO_data.push_back(k1);
					m_MeshIBO_data.push_back(k2);
					m_MeshIBO_data.push_back(k1 + 1);
				}

				if (i != (stack_count - 1))
				{
					m_MeshIBO_data.push_back(k1 + 1);
					m_MeshIBO_data.push_back(k2);
					m_MeshIBO_data.push_back(k2 + 1);
				}
			}
		}
		
		is_root = false;
		generateBuffers();
	}

	void Model::SaveModelHierarchy(File file, ModelHierarchy* h)
	{
		size_t name_len = h->m_Name.size();

		file.Write(&name_len, sizeof(size_t));
		file.Write(h->m_Name.c_str(), name_len);

		file.Write(&h->translation, sizeof(glm::vec3));
		file.Write(&h->rotation, sizeof(glm::vec3));
		file.Write(&h->scale, sizeof(glm::vec3));

		size_t mesh_ind_size = h->m_MeshIndexes.size();

		file.Write(&mesh_ind_size, sizeof(size_t));

		if (mesh_ind_size > 0) {
			file.Write(h->m_MeshIndexes.data(), mesh_ind_size);
		}

		size_t child_size = h->m_Children.size();

		file.Write(&child_size, sizeof(size_t));

		if (child_size > 0) {
			for (size_t i = 0; i < child_size; i++) {
				SaveModelHierarchy(file, h->m_Children[i]);
			}
		}
	}

	ModelHierarchy* Model::LoadModelHierarchy(File file)
	{
		ModelHierarchy* h = new ModelHierarchy();

		size_t name_len;

		file.Read(&name_len, sizeof(size_t));
		h->m_Name.resize(name_len);
		file.Read(&h->m_Name[0], name_len);

		file.Read(&h->translation, sizeof(glm::vec3));
		file.Read(&h->rotation, sizeof(glm::vec3));
		file.Read(&h->scale, sizeof(glm::vec3));

		size_t mesh_ind_size;

		file.Read(&mesh_ind_size, sizeof(size_t));

		if (mesh_ind_size > 0) {
			h->m_MeshIndexes.resize(mesh_ind_size);
			file.Read(&h->m_MeshIndexes[0], mesh_ind_size);
		}

		size_t child_size;

		file.Read(&child_size, sizeof(size_t));

		if (child_size > 0) {
			h->m_Children.reserve(child_size);

			for (size_t i = 0; i < child_size; i++) {
				h->m_Children.push_back(LoadModelHierarchy(file));
			}
		}

		return h;
	}

	void Model::generateBuffers()
	{
		if (is_root) return;

		/////////////////
		// MESH BUFFERS//
		/////////////////

		SMOL_CORE_INFO("Generating buffers...");
		m_MeshVAO = std::make_shared<VertexArray>();

		/*glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		glGenVertexArrays(1, &vao);*/
		
		if (glGetError() != 0)
		{
			SMOL_CORE_ERROR("Check error {0}", glewGetErrorString(glGetError()));
		}

		SMOL_CORE_INFO("Binding the vertex buffer ...");
		m_MeshVBO = std::make_shared<VertexBuffer>(m_MeshVBO_data.data(), m_MeshVBO_data.size() * sizeof(float));

		/*glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vbo_data.size() * sizeof(float), vbo_data.data(), GL_STATIC_DRAW);*/

		if (glGetError() != 0)
		{
			SMOL_CORE_ERROR("Check error {0}", glewGetErrorString(glGetError()));
		}

		m_MeshVBO->SetLayout({
			{ ShaderDataType::Float3, "aPos"           },
			{ ShaderDataType::Float3, "aColor"         }, // TODO: no se si debería ser vec3 o vec4
			{ ShaderDataType::Float2, "aTex"           }/*,
			{ShaderDataType::Int,    "a_EntityID"}*/
			});
		m_MeshVAO->AddVertexBuffer(m_MeshVBO);

		SMOL_CORE_INFO("Binding the vertex buffer DONE");

		SMOL_CORE_INFO("Binding the index buffer ...");
		std::shared_ptr<IndexBuffer> meshIB = std::make_shared<IndexBuffer>(m_MeshIBO_data.data(), m_MeshIBO_data.size());

		/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_data.size() * sizeof(int), ebo_data.data(), GL_STATIC_DRAW);*/

		if (glGetError() != 0)
		{
			SMOL_CORE_ERROR("Check error {0}", glewGetErrorString(glGetError()));
		}

		m_MeshVAO->SetIndexBuffer(meshIB);

		/*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);*/

		if (glGetError() != 0)
		{
			SMOL_CORE_ERROR("Check error {0}", glewGetErrorString(glGetError()));
		}

		SMOL_CORE_INFO("Generating buffers DONE");

		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		if (glGetError() != 0)
		{
			SMOL_CORE_ERROR("Check error {0}", glewGetErrorString(glGetError()));
		}



		/////////////////////////
		// BOUNDING BOX BUFFERS//
		/////////////////////////

		for (int i = 0; i < m_MeshVBO_data.size(); i += 8)
		{
			glm::vec3 vec = { m_MeshVBO_data[i], m_MeshVBO_data[i + 1], m_MeshVBO_data[i + 2] };
			m_BoundingBox.extend(vec);
		}
		m_BoundingBoxIBO_data = {
			0, 1, 1, 2, 2, 3, 3, 0, // Front
			4, 5, 5, 6, 6, 7, 7, 4, // Back
			0, 4, 1, 5, 2, 6, 3, 7
		};
		m_BoundingBoxVBO_data = {
			m_BoundingBox.getMax().x, m_BoundingBox.getMax().y, m_BoundingBox.getMax().z, //TOP
			m_BoundingBox.getMin().x, m_BoundingBox.getMax().y, m_BoundingBox.getMax().z,
			m_BoundingBox.getMin().x, m_BoundingBox.getMax().y, m_BoundingBox.getMin().z,
			m_BoundingBox.getMax().x, m_BoundingBox.getMax().y, m_BoundingBox.getMin().z,

			m_BoundingBox.getMax().x, m_BoundingBox.getMin().y, m_BoundingBox.getMax().z, //Bottom
			m_BoundingBox.getMin().x, m_BoundingBox.getMin().y, m_BoundingBox.getMax().z,
			m_BoundingBox.getMin().x, m_BoundingBox.getMin().y, m_BoundingBox.getMin().z,
			m_BoundingBox.getMax().x, m_BoundingBox.getMin().y, m_BoundingBox.getMin().z,
		};


		m_BoundingBoxVAO = std::make_shared<VertexArray>();

		m_BoundingBoxVBO = std::make_shared<VertexBuffer>(m_BoundingBoxVBO_data.data(), m_BoundingBoxVBO_data.size() * sizeof(float));
		m_BoundingBoxVBO->SetLayout({
			{ ShaderDataType::Float3, "aPos"           },/*,
			{ShaderDataType::Int,    "a_EntityID"}*/
			});
		m_BoundingBoxVAO->AddVertexBuffer(m_BoundingBoxVBO);

		std::shared_ptr<IndexBuffer> BBoxIB = std::make_shared<IndexBuffer>(m_BoundingBoxIBO_data.data(), m_BoundingBoxIBO_data.size());
		m_BoundingBoxVAO->SetIndexBuffer(BBoxIB);

		/*glGenBuffers(1, &bbvbo);
		glGenBuffers(1, &bbebo);
		glGenVertexArrays(1, &bbvao);

		glBindVertexArray(bbvao);
		glBindBuffer(GL_ARRAY_BUFFER, bbvbo);
		glBufferData(GL_ARRAY_BUFFER, bbvbo_data.size() * sizeof(float), bbvbo_data.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bbebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, bbebo_data.size() * sizeof(int), bbebo_data.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);*/

		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}



	Model::Model(const char* file)
	{
		is_root = true;
		m_ModelHierarchy = NULL;

		if (file) {
			if (strcmp(file, "cube") == 0)
			{
				CreateCube();
			}
			else if (strcmp(file, "plane") == 0)
			{
				CreatePlane();
			}
			else if (strcmp(file, "pyramid") == 0)
			{
				CreatePyramid();
			}
			else if (strcmp(file, "sphere") == 0)
			{
				CreateSphere();
			}
			else
			{
				ModelSettings settings;
				getMeshFromFile(file, &settings);
			}
		}
		else {
			is_root = false;
		}
	}

	Model::~Model()
	{
		size_t m_size = m_ChildModels.size();

		for (size_t i = 0; i < m_size; i++) {
			delete m_ChildModels[i];
		}

		if (m_ModelHierarchy) {
			delete m_ModelHierarchy;
		}
	}

	void Model::Render()
	{
		if(is_root){
			size_t meshCount = m_ChildModels.size();

			for (size_t i = 0; i < meshCount; i++) {
				m_ChildModels[i]->Render();
			}
		}
		else {
			m_MeshVAO->Bind();
			glDrawElements(GL_TRIANGLES, (GLsizei)m_MeshIBO_data.size(), GL_UNSIGNED_INT, 0);
			m_MeshVAO->Unbind();
		
		}
	}

	void Model::DrawBoudingBox()
	{
		if (is_root) {
			size_t meshCount = m_ChildModels.size();

			for (size_t i = 0; i < meshCount; i++) {
				m_ChildModels[i]->DrawBoudingBox();
			}
		}
		else {
			m_BoundingBoxVAO->Bind();
			glDrawElements(GL_LINES, (GLsizei)m_BoundingBoxIBO_data.size(), GL_UNSIGNED_INT, 0);
			m_BoundingBoxVAO->Unbind();
		}
	}

	void Model::LoadMesh(const char* file, ModelSettings* settings)
	{
		getMeshFromFile(file, settings);
	}

	void Model::LoadSmolMesh(const char* file)
	{
		getSmolMeshFromFile(file);
	}

	Model* Model::GetModelFromFile(const char* file, ModelSettings* settings)
	{
		Model* model = new Model(NULL);

		model->getMeshFromFile(file, settings, false);

		return model;
	}

	void Model::SaveModel(Model* model, const char* file)
	{
		File f = FileSystem::OpenOB(file);

		if (f.IsOpen()) {
			// Model size
			size_t model_size = model->m_ChildModels.size();
			f.Write(&model_size, sizeof(size_t));

			// Material size
			size_t mat_size = model->m_Materials.size();

			f.Write(&mat_size, sizeof(size_t));
			for (size_t i = 0; i < mat_size; i++) {
				size_t name_size = model->m_Materials[i].size();

				f.Write(&name_size, sizeof(size_t));
				f.Write(model->m_Materials[i].c_str(), name_size);
			}

			// Model list
			for (size_t i = 0; i < model_size; i++) {
				Model* c_model = model->m_ChildModels[i];

				// Material index
				size_t mat_ind = c_model->getMaterialIndex();
				f.Write(&mat_ind, sizeof(size_t));

				// Model vbo
				size_t vbo_size = c_model->m_MeshVBO_data.size();
				f.Write(&vbo_size, sizeof(size_t));
				f.Write(c_model->m_MeshVBO_data.data(), vbo_size * sizeof(float));

				// Model ebo
				size_t ebo_size = c_model->m_MeshIBO_data.size();
				f.Write(&ebo_size, sizeof(size_t));
				f.Write(c_model->m_MeshIBO_data.data(), ebo_size * sizeof(int));
			}

			// Model hierarchy
			SaveModelHierarchy(f, model->m_ModelHierarchy);
		}

		f.Close();
	}
}