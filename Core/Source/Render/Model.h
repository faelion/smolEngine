#pragma once

#include "glm/glm.hpp"
#include <vector>
#include <Utils/math/Math.h>
#include <Utils/math/AABB.h>
#include <Utils/filesystem/FileSystem.h>

struct aiMesh;
struct aiNode;

namespace smol {
	struct ModelSettings;

	struct ModelHierarchy {
		std::string name;

		// Index in model list
		std::vector<unsigned int> meshIndexes;

		// Model children
		std::vector<ModelHierarchy*> children;

		// Transformation
		glm::vec3 translation;
		glm::vec3 rotation;
		glm::vec3 scale;

		~ModelHierarchy() {
			size_t len = children.size();

			for (size_t i = 0; i < len; i++) {
				delete children[i];
			}

			children.clear();
			meshIndexes.clear();
		}
	};

	class Model
	{
	private:
		std::string m_ModelPath;
	protected:
		bool is_root = false;

		std::string model_name;
		unsigned int model_mat;

		std::vector<float> vbo_data;
		std::vector<int> ebo_data;
		std::vector<int> bbebo_data;
		std::vector<float> bbvbo_data;

		std::vector<Model*> models;
		std::vector<std::string> materials;

		ModelHierarchy* model_hierarchy;

		void generateBuffers();
	private:
		unsigned int vao, vbo, ebo, bbvao, bbvbo, bbebo;

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

		unsigned int getVAO() { return vao; }

		const char* getModelPath() { return m_ModelPath.c_str(); }

		bool IsRoot() const { return is_root; }

		Model* getModelAt(size_t index) { return models[index]; }

		std::string getMaterialAt(size_t index) { return materials[index]; }

		size_t getMaterialCount() { return materials.size(); }

		unsigned int getMaterialIndex() { return model_mat; }

		const ModelHierarchy* getModelHierarchy() { return model_hierarchy; }
		std::string getModelName() { return model_name; }

		void LoadMesh(const char* file, ModelSettings* settings);
		void LoadSmolMesh(const char* file);

		void IsRoot(bool root) { is_root = root; }

		static Model* GetModelFromFile(const char* file, ModelSettings* settings);
		static void SaveModel(Model* model, const char* file);
	public:
		bool showNormals = false;
		Math::AABB boundingBox;
	};
}