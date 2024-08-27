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
			Model() = default;
			~Model() = default;

			void setup();
			void load(std::string path);
			void destroy();

			void draw(GL::Shader& shader);

		private:
			std::string directory;
			std::vector<Mesh> meshes;

			Core::GL::Texture loadTextureFromFile(const char* path, const std::string& directory);
			std::vector<Core::GL::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
			Mesh processMesh(aiMesh* mesh, const aiScene* scene);
			void processNode(aiNode* node, const aiScene* scene);

		};
	}
}