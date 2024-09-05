#pragma once

#include <string>
#include <fstream>
#include <sstream>

#include <glad/glad.h>

static std::string get_file_contents(const char* path) {
	std::stringstream ss;
	std::ifstream f(path);
	if (f) {
		ss << f.rdbuf();
		f.close();
	}
	return ss.str();
}

static void glsl_inject_includes(std::string& code, std::string directory, int depth = 0) {
	if (depth > 30) return;
	while (code.find("#include ") != code.npos) {
		const auto pos = code.find("#include ");
		const auto p1 = code.find('<', pos);
		const auto p2 = code.find('>', pos);
		if (p1 == code.npos || p2 == code.npos || p2 <= p1) {
			std::cerr << "Error while loading shader program:\n" << code << std::endl;
			return;
		}
		const std::string name = directory + "/" + code.substr(p1 + 1, p2 - p1 - 1);
		std::string include = get_file_contents(name.c_str());
		std::string dir = name.substr(0, name.find_last_of('/'));
		glsl_inject_includes(include, dir, depth + 1);
		code.replace(pos, p2 - pos + 1, include.c_str());
	}
}

namespace Core {
	namespace GL {
		class ShaderStage {
		public:
			ShaderStage() = default;
			~ShaderStage() {
				if (shader == 0) return;
				glDeleteShader(shader);
			}

			// Move
			ShaderStage(ShaderStage&& other) noexcept {
				shader = other.shader;
				shaderType = other.shaderType;
				other.shader = 0;
			}

			// Move
			ShaderStage& operator=(ShaderStage&& other) noexcept {
				shader = other.shader;
				shaderType = other.shaderType;
				other.shader = 0;
				return *this;
			}


			void initialize(GLenum type) {
				shader = glCreateShader(type);
				shaderType = type;
			}
			void load(std::string path) {
				std::string sourceString = get_file_contents(path.c_str());
				std::string directory = path.substr(0, path.find_last_of('/'));
				glsl_inject_includes(sourceString, directory);
				const GLchar* source = sourceString.data();
				glShaderSource(shader, 1, &source, NULL);
				glCompileShader(shader);

				int success;
				char infoLog[512];
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				if (!success) {
					glGetShaderInfoLog(shader, 512, NULL, infoLog);
					std::cerr << "ERROR: Shader Compilation failed\n" << infoLog << std::endl;
					return;
				}
			};

			GLuint getId() { return shader; }
			GLenum getType() { return shaderType; }

		private:
			GLuint shader;
			GLenum shaderType;
		};
	}
}