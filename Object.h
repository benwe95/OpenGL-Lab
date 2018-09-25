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
    quaternion rotation;
    Mesh* mesh;

    public:
    Object(Mesh* mesh, vec3 position, quaternion roration);
    ~Object();
    void render(float time);

    vec3 getPosition();
    quaternion getRotation();
    Mesh* getMesh();
};

//TODO: Objects Tree

#endif