#pragma once
#include <string>
#include <vector>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include "MeshData.h"

namespace Core {
	namespace IO {
		class MeshFile {
		public:
			MeshFile() = default;
			~MeshFile() = default;

			void load(std::string filepath);
			void loadBinary(std::string filepath);
			void save(std::string filepath);

			MeshFileHeader header;
			std::vector<MeshData> meshes;
			std::vector<uint32_t> indexData;
			std::vector<float> vertexData;

			bool exportTextures = true;
			bool exportNormals = true;
		private:
			MeshData convertAIMesh(const aiMesh* m);

			uint32_t indexOffset = 0;
			uint32_t vertexOffset = 0;
		};
	}
}