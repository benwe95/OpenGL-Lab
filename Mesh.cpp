#include "Mesh.h"

Mesh::Mesh(GLuint program)
{
	this->program = program;
}

GLuint Mesh::getProgram()
{
	return program;
}

void Mesh::render(mat4 projection, mat4 view, mat4 model)
{
	glUseProgram(program);

	// Récupération de la "location" des matrices du pipeline
	GLint proj_location = glGetUniformLocation(program, "Projection");
	GLint view_location = glGetUniformLocation(program, "View");
	GLint model_location = glGetUniformLocation(program, "Model");

	// Association des matrices
	glUniformMatrix4fv(proj_location, 1, GL_FALSE, (const GLfloat*) projection);
	glUniformMatrix4fv(view_location, 1, GL_FALSE, (const GLfloat*) view);
	glUniformMatrix4fv(model_location, 1, GL_FALSE, (const GLfloat*) model);
}