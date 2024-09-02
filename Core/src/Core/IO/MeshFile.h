#pragma once
#include <string>
#include <vector>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include "Core/Render/Mesh.h"

namespace Core {
	namespace IO {
		class MeshFile {
		public:
			MeshFile() = default;
			~MeshFile() = default;

			void load(std::string filepath);
			void loadBinary(std::string filepath);
			void save(std::string filepath);

			Render::MeshFileHeader header;
			std::vector<Render::Mesh> meshes;
			std::vector<uint32_t> indexData;
			std::vector<float> vertexData;

			bool exportTextures = true;
			bool exportNormals = true;
		private:
			Render::Mesh convertAIMesh(const aiMesh* m);

			uint32_t indexOffset = 0;
			uint32_t vertexOffset = 0;
		};
	}
}