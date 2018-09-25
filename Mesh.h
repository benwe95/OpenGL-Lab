#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Mesh
{
    public:
    Mesh(GLuint program);
    GLuint getProgram();
    virtual void render(float time) = 0;

    private:
    GLuint program;
};

#endif