#pragma once

#include <vector>
#include <string>

#include <assimp/scene.h>

#include "Core/GL/gl.h"
#include "Mesh.h"

namespace Core {
	namespace Render {
		
		class Model {
		public:
			Model(std::string path) {
				loadModel(path);
			}

			void draw(GL::Shader& shader);

			std::vector<Mesh*> meshes;
		private:
			std::string directory;

			void loadModel(std::string path);
			void processNode(aiNode* node, const aiScene* scene);
			void processMesh(aiMesh* mesh, const aiScene* scene);
			std::vector<Core::GL::Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
			Core::GL::Texture* loadTextureFromFile(const char* path, const std::string& directory);
		};
	}
}