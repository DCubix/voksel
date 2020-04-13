#include "rendering.h"

#include <iostream>

Shader::Shader(const std::string& vert, const std::string& frag) {
	GLint vs = createShader(vert, GL_VERTEX_SHADER);
	GLint fs = createShader(frag, GL_FRAGMENT_SHADER);

	if (vs != 0 && fs != 0) {
		m_program = glCreateProgram();
		glAttachShader(m_program, vs);
		glAttachShader(m_program, fs);
		glLinkProgram(m_program);

		GLint status = 0;
		glGetProgramiv(m_program, GL_LINK_STATUS, &status);
		if (status != GL_TRUE) {
			glDeleteProgram(m_program);
			glDeleteShader(vs);
			glDeleteShader(fs);
			return;
		}
		
		glDetachShader(m_program, vs);
		glDetachShader(m_program, fs);
		glDeleteShader(vs);
		glDeleteShader(fs);
	}
}

GLuint Shader::createShader(const std::string& src, GLenum type) {
	GLuint s = glCreateShader(type);
	
	const char* srcPtr = src.c_str();
	glShaderSource(s, 1, &srcPtr, nullptr);
	glCompileShader(s);

	GLint status = 0;
	glGetShaderiv(s, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		char log[1024];
		glGetShaderInfoLog(s, 1024, nullptr, log);
		std::cerr << log << std::endl;
		glDeleteShader(s);
		return 0;
	}
	return s;
}

GLint Shader::getUniform(const std::string& name) {
	auto pos = m_uniforms.find(name);
	if (pos == m_uniforms.end()) {
		GLint loc = glGetUniformLocation(m_program, name.c_str());
		if (loc != -1) {
			m_uniforms[name] = loc;
			return loc;
		}
	}
	return pos->second;
}

void Shader::setInt(const std::string& name, int v) {
	glUniform1i(getUniform(name), v);
}

void Shader::setFloat(const std::string& name, float v) {
	glUniform1f(getUniform(name), v);
}

void Shader::setVector2(const std::string& name, float x, float y) {
	glUniform2f(getUniform(name), x, y);
}

void Shader::setVector3(const std::string& name, float x, float y, float z) {
	glUniform3f(getUniform(name), x, y, z);
}

void Shader::setVector4(const std::string& name, float x, float y, float z, float w) {
	glUniform4f(getUniform(name), x, y, z, w);
}

void Shader::setMatrix4(const std::string& name, float* p) {
	glUniformMatrix4fv(getUniform(name), 1, true, p);
}

void MeshBuilder::addVertex(
	const Vector3& position,
	const Vector3& normal,
	const Vector3& tangent,
	const Vector2& texCoord,
	const Vector4& color
) {
	Vertex vert;
	vert.position = position;
	vert.normal = normal;
	vert.tangent = tangent;
	vert.texCoord = texCoord;
	vert.color = color;
	m_vertices.push_back(vert);
}

void MeshBuilder::addPlane(float offset, Axis axis, const Vector4& color, const Vector4& uv, bool flip) {
	float n = flip ? -1.0f : 1.0f;
	switch (axis) {
		case AxisX:
			addVertex(Vector3(offset, 0.0f, 0.0f), Vector3(n, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector2(), color);
			addVertex(Vector3(offset, 0.0f, 1.0f), Vector3(n, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector2(), color);
			addVertex(Vector3(offset, 1.0f, 1.0f), Vector3(n, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector2(), color);
			addVertex(Vector3(offset, 1.0f, 0.0f), Vector3(n, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector2(), color);
		break;
		case AxisY:
			addVertex(Vector3(0.0f, offset, 0.0f), Vector3(0.0f, n, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(), color);
			addVertex(Vector3(1.0f, offset, 0.0f), Vector3(0.0f, n, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(), color);
			addVertex(Vector3(1.0f, offset, 1.0f), Vector3(0.0f, n, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(), color);
			addVertex(Vector3(0.0f, offset, 1.0f), Vector3(0.0f, n, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(), color);
		break;
		case AxisZ:
			addVertex(Vector3(0.0f, 0.0f, offset), Vector3(0.0f, 0.0f, n), Vector3(0.0f, 1.0f, 0.0f), Vector2(), color);
			addVertex(Vector3(1.0f, 0.0f, offset), Vector3(0.0f, 0.0f, n), Vector3(0.0f, 1.0f, 0.0f), Vector2(), color);
			addVertex(Vector3(1.0f, 1.0f, offset), Vector3(0.0f, 0.0f, n), Vector3(0.0f, 1.0f, 0.0f), Vector2(), color);
			addVertex(Vector3(0.0f, 1.0f, offset), Vector3(0.0f, 0.0f, n), Vector3(0.0f, 1.0f, 0.0f), Vector2(), color);
		break;
	}

	if (!flip) {
		m_indices.insert(m_indices.end(), { 0, 1, 2, 2, 3, 0 });
	} else {
		m_indices.insert(m_indices.end(), { 0, 3, 2, 2, 1, 0 });
	}
}