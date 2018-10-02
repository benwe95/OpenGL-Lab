#include "Mesh.h"

Mesh::Mesh(GLuint program)
{
	this->program = program;
}

GLuint Mesh::getProgram()
{
	return program;
}