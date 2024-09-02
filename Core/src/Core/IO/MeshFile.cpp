#include "Meshfile.h"

#include <iostream>
#include <vector>

constexpr uint32_t header_magic_number = 0x12345678;

namespace Core {
	namespace IO {
		Render::Mesh MeshFile::convertAIMesh(const aiMesh* m) {
			const bool hasTexCoords = m->HasTextureCoords(0);
			const uint32_t numIndices = m->mNumFaces * 3;
			const uint32_t streamElementSize = static_cast<uint32_t>(8 * sizeof(float));
			const uint32_t meshSize = static_cast<uint32_t>(m->mNumVertices * streamElementSize + numIndices * sizeof(uint32_t));
			const Render::Mesh result = {
				.lodCount = 1,
				.streamCount = 1,
				.materialID = 0,
				.meshSize = meshSize,
				.vertexCount = m->mNumVertices,
				.lodOffset = { indexOffset * sizeof(uint32_t), (indexOffset + numIndices) * sizeof(uint32_t) },
				.streamOffset = { vertexOffset * streamElementSize },
				.streamElementSize = { streamElementSize }
			};

			for (size_t i = 0; i < m->mNumVertices; i++) {
				const aiVector3D& v = m->mVertices[i];
				const aiVector3D& n = m->mNormals[i];
				const aiVector3D& t = hasTexCoords ? m->mTextureCoords[0][i] : aiVector3D();
				vertexData.push_back(v.x);
				vertexData.push_back(v.y);
				vertexData.push_back(v.z);
				vertexData.push_back(n.x);
				vertexData.push_back(n.y);
				vertexData.push_back(n.z);
				vertexData.push_back(t.x);
				vertexData.push_back(1.0f - t.y);
			}

			for (size_t i = 0; i < m->mNumFaces; i++) {
				aiFace face = m->mFaces[i];
				indexData.push_back(face.mIndices[0] + vertexOffset);
				indexData.push_back(face.mIndices[1] + vertexOffset);
				indexData.push_back(face.mIndices[2] + vertexOffset);
			}

			indexOffset += numIndices;
			vertexOffset += m->mNumVertices;
			return result;
		}

		void MeshFile::loadBinary(std::string path) {
			FILE* f = fopen(path.c_str(), "rb");
			if (f == NULL) {
				std::cerr << "ERROR: unable to open mesh file: " << path << std::endl;
				return;
			}
			fread(&header, 1, sizeof(Render::MeshFileHeader), f);
			if (header.magicValue != header_magic_number) {
				std::cerr << "ERROR: something wrong with binary mesh file: " << path << std::endl;
				return;
			}
			meshes.resize(header.meshCount);
			fread(meshes.data(), header.meshCount, sizeof(Render::Mesh), f);
			indexData.resize(header.indexDataSize);
			vertexData.resize(header.vertexDataSize);
			fread(indexData.data(), 1, header.indexDataSize, f);
			fread(vertexData.data(), 1, header.vertexDataSize, f);
		}

		void MeshFile::load(std::string filename) {
			const unsigned int flags =
				aiProcess_Triangulate
				| aiProcess_FlipUVs
				| aiProcess_PreTransformVertices
				| aiProcess_JoinIdenticalVertices
				| aiProcess_GenSmoothNormals
				| aiProcess_RemoveRedundantMaterials
				| aiProcess_FindDegenerates
				| aiProcess_FindInvalidData
				| aiProcess_FindInstances
				| aiProcess_OptimizeMeshes
				| aiProcess_GenUVCoords
				| aiProcess_ImproveCacheLocality
				| aiProcess_SplitLargeMeshes;


			const aiScene* scene = aiImportFile(filename.c_str(), flags);
			if (!scene || !scene->HasMeshes()) {
				std::cerr << "Unable to load mesh file: " << filename << std::endl;
				return;
			}
			meshes.reserve(scene->mNumMeshes);
			for (size_t i = 0; i < scene->mNumMeshes; i++) {
				meshes.push_back(convertAIMesh(scene->mMeshes[i]));
			}

			header.magicValue = header_magic_number;
			header.meshCount = (uint32_t)meshes.size();
			header.dataBlockStartOffset = (uint32_t)(sizeof(Render::MeshFileHeader) + meshes.size() * sizeof(Render::Mesh));
			header.indexDataSize = indexData.size() * sizeof(uint32_t);
			header.vertexDataSize = vertexData.size() * sizeof(float);
			return;
		}

		void MeshFile::save(std::string path) {
			FILE* f = fopen(path.c_str(), "wb");
			fwrite(&header, 1, sizeof(header), f);
			fwrite(meshes.data(), header.meshCount, sizeof(Render::Mesh), f);
			fwrite(indexData.data(), 1, header.indexDataSize, f);
			fwrite(vertexData.data(), 1, header.vertexDataSize, f);
			fclose(f);
		}
	
	}
}