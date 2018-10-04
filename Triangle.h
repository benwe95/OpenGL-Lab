#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mesh.h"
#include "Computer.h"

class Triangle : public Mesh
{
	public:
	Triangle(Program *program);
	void render(mat4 model);
	void update();

	private:
	GLuint vertex_array;
	GLuint vertex_buffer;
	Computer *computer;
};

#endif