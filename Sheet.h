#ifndef Sheet_MESH_H
#define Sheet_MESH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mesh.h"
#include "Computer.h"

class Sheet : public Mesh
{
    private:
    GLuint vertex_array;
    GLuint texture;
    GLuint texUnit;
    GLuint *indices;
    int indicesCount;
    Computer *collide;
    Computer *acceleration;
    Computer *cinematic;

    public:
    Sheet(Program *program);
    ~Sheet();
    void render(mat4 model);
    void update();
};

#endif