#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mesh.h"

class Triangle : public Mesh
{
	public:
	Triangle(GLuint program);
	void render(mat4 mvp);

	private:
	GLuint vertex_array;
};

#endif