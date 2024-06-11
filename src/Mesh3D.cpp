#include <iostream>
#include "Mesh3D.h"
#include <glad/glad.h>
#include <GL/GL.h>

using std::vector;
using glm::mat4;
using glm::vec4;

Mesh3D::Mesh3D(const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& faces) 
	: m_vertexCount(vertices.size()), m_faceCount(faces.size()) {

	// Generate a vertex array object on the GPU.
	glGenVertexArrays(1, &m_vao);
	// "Bind" the newly-generated vao, which makes future functions operate on that specific object.
	glBindVertexArray(m_vao);

	// Generate a vertex buffer object on the GPU.
	uint32_t vbo;
	glGenBuffers(1, &vbo);

	// "Bind" the newly-generated vbo, which makes future functions operate on that specific object.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// This vbo is now associated with m_vao.
	// Copy the contents of the vertices list to the buffer that lives on the GPU.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex3D), &vertices[0], GL_STATIC_DRAW);
	// Inform OpenGL how to interpret the buffer: each vertex is 3 contiguous floats (4 bytes each)
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// Generate a second buffer, to store the indices of each triangle in the mesh.
	uint32_t ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(uint32_t), &faces[0], GL_STATIC_DRAW);

	// Unbind the vertex array, so no one else can accidentally mess with it.
	glBindVertexArray(0);
}

void Mesh3D::render() {
	// Activate the mesh's vertex array.
	glBindVertexArray(m_vao);
	// Draw the vertex array, using is "element buffer" to identify the faces.
	glDrawElements(GL_TRIANGLES, m_faceCount, GL_UNSIGNED_INT, nullptr);
	// Deactivate the mesh's vertex array.
	glBindVertexArray(0);
}

Mesh3D Mesh3D::square() {
	return Mesh3D(
		{ 
		  { 0.5, 0.5, 0 },    // TR
		  { 0.5, -0.5, 0 },   // BR
		  { -0.5, -0.5, 0 },  // BL
		  { -0.5, 0.5, 0 },   // TL
		}, 
		{ 
			3, 1, 2,
			3, 1, 0,
		}
	);
}

Mesh3D Mesh3D::triangle() {
	return Mesh3D(
		{ { -0.5, -0.5, 0 },
		  { -0.5, 0.5, 0 },
		  { 0.5, 0.5, 0 } },
		{ 2, 1, 0 }
	);
}

Mesh3D Mesh3D::cube() {
	std::vector<Vertex3D> verts = {
		{ 0.5, 0.5, -0.5 },
		{ -0.5, 0.5, -0.5 },
		{ -0.5, -0.5, -0.5 },
		{ 0.5, -0.5, -0.5 },
		{ 0.5, 0.5, 0.5 },
		{ -0.5, 0.5, 0.5 },
		{ -0.5, -0.5, 0.5 },
		{ 0.5, -0.5, 0.5 }
	};
	std::vector<uint32_t> tris = {
		0, 1, 2,
		0, 2, 3,
		4, 0, 3,
		4, 3, 7,
		5, 4, 7,
		5, 7, 6,
		1, 5, 6,
		1, 6, 2,
		4, 5, 1,
		4, 1, 0,
		2, 6, 7,
		2, 7, 3
	};

	return Mesh3D(verts, tris);
}
