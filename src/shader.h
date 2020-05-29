#pragma once
	
#include<glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader {
public:
	GLuint programID;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
		const char* vertexCode = getShaderCode(vertexPath).c_str();
		const char* fragCode = getShaderCode(fragmentPath).c_str();
		GLuint vertexShader = compileShader(GL_VERTEX_SHADER, &vertexCode);
		GLuint fragShader = compileShader(GL_FRAGMENT_SHADER, &fragCode);
		linkShader(vertexShader, fragShader);
	}
	~Shader() {
		glDeleteProgram(programID);
	}

	void use() {
		glUseProgram(programID);
	}


	//void setBool(const string &name, bool value) const {
	//	glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
	//}

	//void setInt(const string &name, int value) const {
	//	glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
	//}

	//void setFloat(const string &name, float value) const {

	//}

private:
	string getShaderCode(const GLchar* shaderPath) {
		ifstream shaderFile;
		string result;
		shaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		try {
			shaderFile.open(shaderPath);
			stringstream shaderStream;
			shaderStream << shaderFile.rdbuf();
			shaderFile.close();
			result = shaderStream.str();
		}
		catch (ifstream::failure e) {
			cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
		}
		return result;
	}

	GLuint compileShader(GLenum shaderType, const GLchar *const* shaderSource) {
		GLuint shader;
		int success;
		char infoLog[512];
		shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, shaderSource, NULL);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << endl;
		}
		return shader;
	}

	void linkShader(GLuint vertexShader, GLuint fragShader) {
		int success;
		char infoLog[512];
		programID = glCreateProgram();
		glAttachShader(programID, vertexShader);
		glAttachShader(programID, fragShader);
		glLinkProgram(programID);
		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(programID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragShader);
	}

};
