#include "Model.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Core {
	namespace Render {

		void Model::draw(GL::Shader& shader) {
			for (int i = 0; i < meshes.size(); i++) {
				meshes[i]->draw(shader);
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
				processMesh(mesh, scene);
			}
			for (int i = 0; i < node->mNumChildren; i++) {
				processNode(node->mChildren[i], scene);
			}
		}

		void Model::processMesh(aiMesh* mesh, const aiScene* scene) {
			std::vector<Vertex> vertices;
			std::vector<GLuint> indices;
			std::vector<GL::Texture*> textures;

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
				std::vector<GL::Texture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "u_texture_diffuse");
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
				std::vector<GL::Texture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "u_texture_specular");
				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			}

			Mesh* _mesh = new Mesh(vertices, indices, textures);
			meshes.push_back(_mesh);
		}

		std::vector<GL::Texture*> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
			std::vector<GL::Texture*> textures;
			unsigned int typeCount = 0;
			for (int i = 0; i < mat->GetTextureCount(type); i++) {
				aiString str;
				mat->GetTexture(type, i, &str);
				GL::Texture* tex = loadTextureFromFile(str.C_Str(), directory);
				std::string filename = std::string(str.C_Str());
				std::string path = directory + '/' + filename;
				tex->name = typeName + "_" + std::to_string(typeCount++);
				tex->fileName = filename;
				tex->path = path;
				textures.push_back(tex);
			}
			return textures;
		}

		GL::Texture* Model::loadTextureFromFile(const char* path, const std::string& directory) {
			std::string filename = std::string(path);
			filename = directory + '/' + filename;

			GL::Texture* tex = new GL::Texture();
			GL::TextureOptions opts;

			tex->path = filename;
			tex->fileName = std::string(path);

			int width, height, comps;
			uint8_t* data = stbi_load(filename.c_str(), &width, &height, &comps, 0);
			if (data) {
				GLenum format = GL_RGBA;
				GLenum internalFormat = GL_RGBA8;
				if (comps == 1) {
					opts.format = GL_RED;
					opts.internalFormat = GL_R8;
				}
				else if (comps == 3) {
					opts.format = GL_RGB;
					opts.internalFormat = GL_RGB8;
				}
				else if (comps == 4) {
					opts.format = GL_RGBA;
					opts.internalFormat = GL_RGBA8;
				}

				tex->setup();
				tex->load(data, width, height, opts);
				stbi_image_free(data);
			}
			else {
				std::cout << "Texture failed to load at path: " << path << std::endl;
				stbi_image_free(data);
			}

			return tex;
		}
	}
}