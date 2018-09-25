#include "Object.h"

#include <iostream>

using namespace std;

Object::Object(Mesh* mesh, vec3 position, mat4 rotation)
{
    this->mesh = mesh;
    this->position = position;
    this->rotation = rotation;
}

Object::~Object()
{

}

void Object::update(float time)
{
    rotation = rotate(time*10, 0.0f, 0.0f, 1.0f);
}

void Object::render(mat4 mvp)
{
    mvp = mvp * translate(position) * rotation;
    mesh->render(mvp);
}

vec3 Object::getPosition() { return position; }
mat4 Object::getRotation() { return rotation; }
Mesh* Object::getMesh() { return mesh; }
