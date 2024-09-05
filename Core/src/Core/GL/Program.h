#pragma once

#include <unordered_map>

#include <glad/glad.h>

#include "ShaderStage.h"

namespace Core {
	namespace GL {
		class Program {
		public:
			Program() : program(0) {};

			~Program() {
				if (program == 0) return;
				glDeleteProgram(program);
			};

			// Move
			Program(Program&& other) noexcept {
				program = other.program;
				uniform_locations = other.uniform_locations;
				other.program = 0;
			}

			// Move
			Program& operator=(Program&& other) noexcept {
				program = other.program;
				uniform_locations = other.uniform_locations;
				other.program = 0;
				return *this;
			}

			void initialize() { program = glCreateProgram(); }
			void attach(ShaderStage& shader) {
				glAttachShader(program, shader.getId());
			};
			void link() {
				glLinkProgram(program);

				char infoLog[512];
				int success;
				glGetProgramiv(program, GL_LINK_STATUS, &success);
				if (!success) {
					glGetProgramInfoLog(program, 512, NULL, infoLog);
					std::cerr << "ERROR: Shader Program link failure\n" << infoLog << std::endl;
					return;
				}
			}

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