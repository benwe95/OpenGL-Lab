#include "Triangle.h"

#include <iostream>

using namespace std;

Triangle::Triangle(GLuint program) : Mesh(program)
{   
    // Définition des VertexData
    const struct
    {
        float x, y;
        float r, g, b;
    } vertices[3] =
    {
        { -0.6f, -0.4f, 1.f, 0.f, 0.f },
        {  0.6f, -0.4f, 0.f, 1.f, 0.f },
        {   0.f,  0.6f, 0.f, 0.f, 1.f }
    };

    GLint vpos_location, vcol_location;

    // Récupération des "location" des variable "vPos" et "vCol" du pipeline
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    // Création du VertexArray
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    // Définition du vertexBuffer
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glUseProgram(program);

    // Association des 2 1ères composantes des VertexData à la variable "vPos" du pipeline
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) 0);

    // Association des 3 composantes suivantes des VertexData à la variable "vCol" du pipeline  
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) (sizeof(float) * 2));
}

void Triangle::render(float time)
{
    // Selection du VertexArray
    glBindVertexArray(vertex_array);

    // Affichage des 3 1ers sommets en utilisant des triangles.
    glDrawArrays(GL_TRIANGLES, 0, 3);
}