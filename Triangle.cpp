#include "Triangle.h"

#include <iostream>
#include "shader.h"

using namespace std;

Triangle::Triangle(Program *program) : Mesh(program)
{   
	// Définition des VertexData
	const struct
	{
		vec3 position;
		float p1;
		vec3 velocity;
		float p2;
		vec3 normal;
		float p3;
		vec3 tangent;
		float p4;
		vec2 texture;
		float p5, p6;
	} vertices[3] =
	{
		{vec3(-0.6f, -0.4f, 0.f), 1.f, vec3(0.f, 0.f, 0.f), 1.f, vec3(0.f, 0.f, 1.f), 1.f, vec3(1.f, 0.f, 0.f), 1.f, vec2(0.f, 0.f), 1.f, 1.f },
		{vec3(0.6f, -0.4f, 0.f), 1.f, vec3(0.f, 0.f, 0.f), 1.f, vec3(0.f, 0.f, 1.f), 1.f, vec3(1.f, 0.f, 0.f), 1.f, vec2(1.f, 0.f), 1.f, 1.f },
		{vec3(0.f, 0.6f, 0.f), 1.f, vec3(0.f, 0.f, 0.f), 1.f, vec3(0.f, 0.f, 1.f), 1.f, vec3(1.f, 0.f, 0.f), 1.f, vec2(0.5f, 1.f), 1.f, 1.f }
	};

	GLint vpos_location, vtex_location, tex_location;
	GLuint texture, sampler, texUnit;

	// Récupération des "location" des variable "vPos" et "vCol" du pipeline
	vpos_location = glGetAttribLocation(program->getId(), "vPos");
	vtex_location = glGetAttribLocation(program->getId(), "vTex");

	// Création du VertexArray
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	// Définition du vertexBuffer
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertex_buffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	program->bind();

	// Association des 2 1ères composantes des VertexData à la variable "vPos" du pipeline
	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) 0);

	// Association des 3 composantes suivantes des VertexData à la variable "vCol" du pipeline  
	glEnableVertexAttribArray(vtex_location);
	glVertexAttribPointer(vtex_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) (sizeof(float) * 16));

	program->unbind();

	GLuint compute_shader = loadShader("shader.comp", GL_COMPUTE_SHADER);
	computer = new Computer(compute_shader);
	computer->setData(1, vertex_buffer);
}

void Triangle::update()
{
	computer->compute(1, 1, 1);
}

void Triangle::render(mat4 model)
{
	getProgram()->begin(model);

	// Selection du VertexArray
	glBindVertexArray(vertex_array);

	// Affichage des 3 1ers sommets en utilisant des triangles.
	glDrawArrays(GL_TRIANGLES, 0, 3);

	getProgram()->end();
}