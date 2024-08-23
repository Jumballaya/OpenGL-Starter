#include "Model.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <stb/stb_image.h>

namespace Core {
	namespace Render {

		void Model::draw(GL::Shader& shader) {
			for (int i = 0; i < meshes.size(); i++) {
				meshes[i].Draw(shader);
			}
		}
		void Model::loadModel(std::string path) {
			Assimp::Importer import;
			const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
				std::cerr << "Assimp model loading error: " << import.GetErrorString() << std::endl;
				return;
			}

			directory = path.substr(0, path.find_last_of('/'));
			processNode(scene->mRootNode, scene);
		}

		void Model::processNode(aiNode* node, const aiScene* scene) {
			for (int i = 0; i < node->mNumMeshes; i++) {
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.push_back(processMesh(mesh, scene));
			}
			for (int i = 0; i < node->mNumChildren; i++) {
				processNode(node->mChildren[i], scene);
			}
		}

		Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
			std::vector<Vertex> vertices;
			std::vector<GLuint> indices;
			std::vector<Core::GL::Texture> textures;

			for (int i = 0; i < mesh->mNumVertices; i++) {
				Vertex vertex;
				glm::vec3 pos(0.0f);
				glm::vec3 normal(0.0f);
				glm::vec3 uv(0.0f);

				pos.x = mesh->mVertices[i].x;
				pos.y = mesh->mVertices[i].y;
				pos.z = mesh->mVertices[i].z;
				vertex.position = pos;

				normal.x = mesh->mNormals[i].x;
				normal.y = mesh->mNormals[i].y;
				normal.z = mesh->mNormals[i].z;
				vertex.normal = normal;

				if (mesh->mTextureCoords[0]) {
					uv.x = mesh->mTextureCoords[0][i].x;
					uv.y = mesh->mTextureCoords[0][i].y;
				}
				vertex.uv = uv;

				vertices.push_back(vertex);
			}

			// indices
			for (int i = 0; i < mesh->mNumFaces; i++) {
				aiFace face = mesh->mFaces[i];
				for (int j = 0; j < face.mNumIndices; j++) {
					indices.push_back(face.mIndices[j]);
				}
			}

			// material
			if (mesh->mMaterialIndex >= 0) {
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
				std::vector<Core::GL::Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
				std::vector<Core::GL::Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			}

			return Mesh(vertices, indices, textures);
		}

		std::vector<Core::GL::Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
			std::vector<Core::GL::Texture> textures;
			for (int i = 0; i < mat->GetTextureCount(type); i++) {
				aiString str;
				mat->GetTexture(type, i, &str);

				bool skip = false;
				for (unsigned int j = 0; j < textures_loaded.size(); j++) {
					if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
						textures.push_back(textures_loaded[j]);
						skip = true;
						break;
					}
				}
				if (!skip) {		
					Core::GL::Texture tex = loadTextureFromFile(str.C_Str(), directory);
					tex.type = typeName;
					tex.path = std::string(str.C_Str());
					textures.push_back(tex);
				}
			}
			return textures;
		}

		Core::GL::Texture Model::loadTextureFromFile(const char* path, const std::string &directory) {
			std::string filename = std::string(path);
			filename = directory + '/' + filename;

			Core::GL::Texture tex;
			Core::GL::TextureOptions opts;

			int width, height, nrComponents;
			uint8_t* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
			if (data) {
				GLenum format;
				GLenum internalFormat;
				if (nrComponents == 1) {
					format = GL_RED;
					internalFormat = GL_R8;
				} else if (nrComponents == 3) {
					format = GL_RGB;
					internalFormat = GL_RGB8;
				} else if (nrComponents == 4) {
					format = GL_RGBA;
					internalFormat = GL_RGBA8;
				}

				opts.format = format;
				opts.internalFormat = internalFormat;
				opts.minFilter = GL_LINEAR_MIPMAP_LINEAR;

				tex.setup(data, width, height, opts);

				stbi_image_free(data);
			} else {
				std::cout << "Texture failed to load at path: " << path << std::endl;
				stbi_image_free(data);
			}

			return tex;
		}
	}
}