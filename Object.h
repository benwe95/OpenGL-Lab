#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vmath.h"
#include "Mesh.h"

using namespace vmath;

class Object
{
    private:
    vec3 position;
    mat4 rotation;
    Mesh* mesh;

    public:
    Object(Mesh* mesh, vec3 position, mat4 roration);
    ~Object();
    void render(mat4 mvp);
    void update(float time);

    vec3 getPosition();
    mat4 getRotation();
    Mesh* getMesh();
};

//TODO: Objects Tree

#endif