#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vmath"
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
    void render();

    vec3 getPosition();
    quaternion getRotation();
    Mesh* getMesh();
    
    void setup() = 0;
    void update() = 0;
    void teardown() = 0;
};

//TODO: Objects Tree

#endif