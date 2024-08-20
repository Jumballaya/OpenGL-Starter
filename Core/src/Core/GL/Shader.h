#pragma once

#include <unordered_map>
#include <string>

namespace Core {
	namespace GL {	
		class Shader {
		public:
			Shader(std::string vertexPath, std::string fragmentPath);
			~Shader();

			void bind();
			void unbind();

			void uniform_f(std::string name, float value);
			void uniform_v(std::string name, unsigned int size, float* value);
			void uniform_i(std::string name, int value);
			void uniform_m(std::string name, unsigned int size, float* value);

		private:
			unsigned int program;

			std::unordered_map<std::string, unsigned int> uniform_locations;

			unsigned int get_location(std::string name);
		};
	}
}
