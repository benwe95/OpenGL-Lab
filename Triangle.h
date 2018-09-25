#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mesh.h"

class TriangleMesh : public Mesh
{
    public:
    Triangle(GLuint program);
    void render();

    private:
    GLuint program;
    GLuint vertex_array;
};

#endif