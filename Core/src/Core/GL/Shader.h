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
			Shader();
			~Shader() = default;

			void setup();
			void load(std::string vertexPath, std::string fragmentPath);
			void destroy();

			void bind();
			void unbind();

			void uniform_f(std::string name, float value);
			void uniform_v(std::string name, unsigned int size, float* value);
			void uniform_i(std::string name, int value);
			void uniform_m(std::string name, unsigned int size, float* value);

		private:
			Program program;
			std::unordered_map<std::string, GLuint> uniform_locations;
			GLuint get_location(std::string name);

			GLuint compileShader(GLenum type, std::string path);

			bool compiled = false;
		};
	}
}
