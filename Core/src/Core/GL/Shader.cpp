#include <sstream>
#include <fstream>
#include <iostream>

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

		Shader::Shader() {
			program = 0;
		}

		void Shader::setup() {
			program = glCreateProgram();
		}

		void Shader::load(std::string vertexPath, std::string fragmentPath) {
			std::string vertexShaderSourceString = get_file_contents(vertexPath.c_str());
			const GLchar* vertexShaderSource = vertexShaderSourceString.data();
			std::string fragmentShaderSourceString = get_file_contents(fragmentPath.c_str());
			const GLchar* fragmentShaderSource = fragmentShaderSourceString.data();
			int success;
			char infoLog[512];

			GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexPath);
			GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentPath);

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
			compiled = true;
		}

		GLuint Shader::compileShader(GLenum type, std::string path) {
			std::string sourceString = get_file_contents(path.c_str());
			std::string directory = path.substr(0, path.find_last_of('/'));
			glsl_inject_includes(sourceString, directory);
			const GLchar* source = sourceString.data();
			GLuint shader = glCreateShader(type);
			glShaderSource(shader, 1, &source, NULL);
			glCompileShader(shader);

			int success;
			char infoLog[512];
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 512, NULL, infoLog);
				std::cerr << "ERROR: Shader Compilation failed\n" << infoLog << std::endl;
				return 0;
			}
			return shader;
		}


		void Shader::destroy() {
			glDeleteProgram(program);
		}

		void Shader::bind() {
			glUseProgram(program);
		}

		void Shader::unbind() {
			glUseProgram(0);
		}

		GLuint Shader::get_location(std::string name) {
			if (uniform_locations.find(name) != uniform_locations.end()) {
				return uniform_locations.at(name);
			}
			GLuint loc = glGetUniformLocation(program, name.c_str());
			uniform_locations.insert({ name, loc });
			return loc;
		};

		void Shader::uniform_f(std::string name, float value) {
			if (program == 0) return;
			unsigned int loc = get_location(name);
			glUniform1f(loc, value);
		}

		void Shader::uniform_v(std::string name, unsigned int size, float* value) {
			if (program == 0) return;
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
			if (program == 0) return;
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
			if (program == 0) return;
			unsigned int loc = get_location(name);
			glUniform1i(loc, value);
		}
	}
}