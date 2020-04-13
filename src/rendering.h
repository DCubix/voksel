#ifndef RENDERING_H
#define RENDERING_H

#include <unordered_map>
#include <vector>
#include <string>

#include "vec_math.hpp"

template <typename T>
using Dict = std::unordered_map<std::string, T>;

#include "glad.h"

class Shader {
public:
	Shader() = default;

	explicit Shader(const std::string& vert, const std::string& frag);

	GLint getUniform(const std::string& name);

	void setInt(const std::string& name, int v);
	void setFloat(const std::string& name, float v);
	void setVector2(const std::string& name, float x, float y);
	void setVector3(const std::string& name, float x, float y, float z);
	void setVector4(const std::string& name, float x, float y, float z, float w);
	void setMatrix4(const std::string& name, float* p);

private:
	Dict<GLint> m_uniforms;
	GLuint m_program;

	GLuint createShader(const std::string& src, GLenum type);
};

class MeshBuilder {
public:
	struct Vertex {
		Vector3 position, normal, tangent;
		Vector2 texCoord;
		Vector4 color;
	};

	enum Axis {
		AxisX = 0,
		AxisY,
		AxisZ
	};

	void addVertex(
		const Vector3& position,
		const Vector3& normal,
		const Vector3& tangent,
		const Vector2& texCoord,
		const Vector4& color
	);
	void addPlane(float offset, Axis axis, const Vector4& color, const Vector4& uv, bool flip = false);
	
	const std::vector<Vertex>& vertices() const { return m_vertices; }
	const std::vector<GLuint>& indices() const { return m_indices; }

private:
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
};

#endif // RENDERING_H