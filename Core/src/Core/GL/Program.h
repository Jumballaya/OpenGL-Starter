#pragma once

#include <unordered_map>

#include <glad/glad.h>

#include "ShaderStage.h"

namespace Core {
	namespace GL {
		class Program {
		public:
			Program() = default;
			~Program() { if (program > 0) glDeleteProgram(program); };

			void initialize() { program = glCreateProgram(); }
			void load(ShaderStage shader) {
				glAttachShader(program, shader.getId());
				glLinkProgram(program);

				char infoLog[512];
				int success;
				glGetProgramiv(program, GL_LINK_STATUS, &success);
				if (!success) {
					glGetProgramInfoLog(program, 512, NULL, infoLog);
					std::cerr << "ERROR: Shader Program link failure\n" << infoLog << std::endl;
					return;
				}
			};

			void bind() { glUseProgram(program); }
			void unbind() { glUseProgram(0); }

			GLuint getId() { return program; }

			GLuint getLocation(std::string name) {
				if (uniform_locations.find(name) != uniform_locations.end()) {
					return uniform_locations.at(name);
				}

				GLuint loc = glGetUniformLocation(program, name.c_str());
				uniform_locations.insert({ name, loc });
				return loc;
			};

		private:
			GLuint program;
			std::unordered_map<std::string, GLuint> uniform_locations;

		};
	}
}