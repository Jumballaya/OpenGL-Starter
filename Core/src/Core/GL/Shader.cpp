#include <sstream>
#include <fstream>
#include <iostream>

#include <glad/glad.h>

#include "Shader.h"

static std::string get_file_contents(const char* path) {
	std::stringstream ss;
	std::ifstream f(path);
	if (f) {
		ss << f.rdbuf();
		f.close();
	}
	return ss.str();
}
namespace Core {
	namespace GL {
		Shader::Shader(std::string vertexPath, std::string fragmentPath) {
			std::string vertexShaderSourceString = get_file_contents("shaders/uv/vertex.glsl");
			const GLchar* vertexShaderSource = vertexShaderSourceString.data();
			std::string fragmentShaderSourceString = get_file_contents("shaders/uv/fragment.glsl");
			const GLchar* fragmentShaderSource = fragmentShaderSourceString.data();
			int success;
			char infoLog[512];

			program = glCreateProgram();

			unsigned int vertexShader;
			vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
			glCompileShader(vertexShader);

			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				std::cerr << "ERROR: Vertex Shader Compilation failed\n" << infoLog << std::endl;
				return;
			}

			unsigned int fragmentShader;
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
			glCompileShader(fragmentShader);

			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				std::cerr << "ERROR: Fragment Shader Compilation failed\n" << infoLog << std::endl;
				return;
			}

			glAttachShader(program, vertexShader);
			glAttachShader(program, fragmentShader);
			glLinkProgram(program);

			glGetProgramiv(program, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(program, 512, NULL, infoLog);
				std::cerr << "ERROR: Shader Program link failure\n" << infoLog << std::endl;
				return;
			}

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
		}


		Shader::~Shader() {
			glDeleteProgram(program);
		}

		void Shader::bind() {
			glUseProgram(program);
		}

		void Shader::unbind() {
			glUseProgram(-1);
		}

		unsigned int Shader::get_location(std::string name) {
			if (uniform_locations.find(name) != uniform_locations.end()) {
				return uniform_locations.at(name);
			}
			unsigned int loc = glGetUniformLocation(program, name.c_str());
			uniform_locations.insert({ name, loc });
			return loc;
		};

		void Shader::uniform_f(std::string name, float value) {
			unsigned int loc = get_location(name);
			glUniform1f(loc, value);
		}

		void Shader::uniform_v(std::string name, unsigned int size, float* value) {
			unsigned int loc = get_location(name);
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

		void Shader::uniform_m(std::string name, unsigned int size, float* value) {
			unsigned int loc = get_location(name);
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


		void Shader::uniform_i(std::string name, int value) {
			unsigned int loc = get_location(name);
			glUniform1i(loc, value);
		}
	}
}