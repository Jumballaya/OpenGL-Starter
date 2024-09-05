#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

#include <glad/glad.h>

#include "ShaderStage.h"
#include "Program.h"

namespace Core {
	namespace GL {
		// Program + ShaderStage[] + Uniforms
		class Shader {
		public:
			Shader() = default;

			void initialize() {
				program.initialize();
			}
			void load(std::string vertexPath, std::string fragmentPath) {
				ShaderStage vertex;
				ShaderStage fragment;
				vertex.initialize();
				fragment.initialize();
				vertex.load(vertexPath);
				fragment.load(fragmentPath);
				program.load(vertex);
				program.load(fragment);
			};

			void bind() { program.bind(); }
			void unbind() { program.unbind(); }

			void uniform_f(std::string name, float value) {
				if (program.getId() == 0) return;
				unsigned int loc = program.getLocation(name);
				glUniform1f(loc, value);
			}

			void uniform_v(std::string name, unsigned int size, float* value) {
				if (program.getId() == 0) return;
				unsigned int loc = program.getLocation(name);
				switch (size) {
				case 1:
					uniform_f(name, value[0]);
					return;
				case 2:
					glUniform2fv(loc, 1, value);
					return;
				case 3:
					glUniform3fv(loc, 1, value);
					return;
				case 4:
					glUniform4fv(loc, 1, value);
				}
			}

			void uniform_m(std::string name, unsigned int size, float* value) {
				if (program.getId() == 0) return;
				unsigned int loc = program.getLocation(name);
				switch (size) {
				case 2:
					glUniformMatrix2fv(loc, 1, false, value);
					return;
				case 3:
					glUniformMatrix3fv(loc, 1, false, value);
					return;
				case 4:
					glUniformMatrix4fv(loc, 1, false, value);
					return;
				}
			}

			void uniform_i(std::string name, int value) {
				if (program.getId() == 0) return;
				unsigned int loc = program.getLocation(name);
				glUniform1i(loc, value);
			}

		private:
			Program program;
		};
	}
}
