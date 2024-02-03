#pragma once

#include "glm/glm.hpp"
#include <vector>
#include <Render/VertexArray.h>
#include <Utils/math/Math.h>
#include <Utils/math/AABB.h>
#include <Utils/filesystem/FileSystem.h>

struct aiMesh;
struct aiNode;

namespace smol {
	struct ModelSettings;

	struct ModelHierarchy {
		std::string m_Name;

		// Index in model list
		std::vector<unsigned int> m_MeshIndexes;

		// Model children
		std::vector<ModelHierarchy*> m_Children;

		// Transformation
		glm::vec3 translation;
		glm::vec3 rotation;
		glm::vec3 scale;

		~ModelHierarchy() {
			size_t len = m_Children.size();

			for (size_t i = 0; i < len; i++) {
				delete m_Children[i];
			}

			m_Children.clear();
			m_MeshIndexes.clear();
		}
	};

	class Model
	{
	private:
		std::string m_ModelPath;
	protected:
		bool is_root = false;

		std::string m_ModelName;
		unsigned int m_ModelMat;

		std::vector<float> m_MeshVBO_data;
		std::vector<uint32_t> m_MeshIBO_data;
		std::vector<float> m_BoundingBoxVBO_data;
		std::vector<uint32_t> m_BoundingBoxIBO_data;

		std::vector<Model*> m_ChildModels;
		std::vector<std::string> m_Materials;

		ModelHierarchy* m_ModelHierarchy;

		void generateBuffers();
	private:
		std::shared_ptr<VertexBuffer> m_MeshVBO;
		std::shared_ptr<VertexBuffer> m_BoundingBoxVBO;

		std::shared_ptr<VertexArray> m_MeshVAO;
		std::shared_ptr<VertexArray> m_BoundingBoxVAO;

		void getMeshFromFile(const char* file, ModelSettings* settings, bool gen_buffers=true);
		void getSmolMeshFromFile(const char* file);

		Model* loadmesh(const aiMesh* mesh);
		ModelHierarchy* loadModelHierarchy(const aiNode* node);
		
		void CreateCube();
		void CreatePlane();
		void CreatePyramid();
		void CreateSphere();

		static void SaveModelHierarchy(File file, ModelHierarchy* h);
		static ModelHierarchy* LoadModelHierarchy(File file);
	public:
		Model(const char* file);
		~Model();

		void Render();

		void DrawBoudingBox();

		VertexArray* getVAO() { return m_MeshVAO.get(); }

		const char* getModelPath() { return m_ModelPath.c_str(); }

		bool IsRoot() const { return is_root; }

		Model* getModelAt(size_t index) { return m_ChildModels[index]; }

		std::string getMaterialAt(size_t index) { return m_Materials[index]; }

		size_t getMaterialCount() { return m_Materials.size(); }

		unsigned int getMaterialIndex() { return m_ModelMat; }

		const ModelHierarchy* getModelHierarchy() { return m_ModelHierarchy; }
		std::string getModelName() { return m_ModelName; }

		void LoadMesh(const char* file, ModelSettings* settings);
		void LoadSmolMesh(const char* file);

		void IsRoot(bool root) { is_root = root; }

		static Model* GetModelFromFile(const char* file, ModelSettings* settings);
		static void SaveModel(Model* model, const char* file);
	public:
		bool m_ShowNormals = false;
		Math::AABB m_BoundingBox;
	};
}