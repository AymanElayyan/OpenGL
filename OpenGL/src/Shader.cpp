#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "Shader.h"

Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath), m_RendererID(0)
{
	ShaderProgramSouece source = ParseShader(filepath);
	m_RendererID = CreateShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));

}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUnformLocation(name), value));

}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(GetUnformLocation(name), value));

}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUnformLocation(name), v0, v0, v2, v3));

}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(GetUnformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUnformLocation(const std::string& name) const
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[name];
	}

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' dosn't exist!" << endl;

	m_UniformLocationCache[name] = location;
	return location;
}

unsigned int Shader::CreateShader(const string& vertexShader, const string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	/*
	glLinkProgram: Links the attached shaders into a complete shader program.
	glValidateProgram: Validates the program to check if it can execute in the current OpenGL state.
	*/
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

unsigned int Shader::CompileShader(unsigned int type, const string& source)
{
	int result;
	unsigned int id = glCreateShader(type); // Shader Type
	const char* src = source.c_str();

	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		cout << "Faild to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n";
		cout << message << endl;

		glDeleteShader(id);
		return 0;
	}

	return id;
}

ShaderProgramSouece Shader::ParseShader(const std::string& filepath)
{
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	ifstream stream(filepath);
	string line;
	stringstream ss[2];
	ShaderType type = ShaderType::NONE;


	while (getline(stream, line))
	{
		if (line.find("#shader") != string::npos)
		{
			if (line.find("vertex") != string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	return {
		ss[0].str(),
		ss[1].str()
	};
}