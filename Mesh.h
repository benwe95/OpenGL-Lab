#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vmath.h"

using namespace vmath;

class Mesh
{
	public:
	Mesh(GLuint program);
	GLuint getProgram();
	virtual void render(mat4 mvp) = 0;

	private:
	GLuint program;
};

#endif