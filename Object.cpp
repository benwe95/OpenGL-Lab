#include "Object.h"

Object::Object(Mesh* mesh, vec3 position, quaternion roration)
{
    this->mesh = mesh;
    this->position = position;
    this->rotation = rotation;

    setup();
}

Object::~Object()
{
    teardown();
}

void Object::render()
{
    //TODO: apply translation and rotation
    mesh->render();
}

vec3 Object::getPosition() { return position; }
quaternion Object::getRotation() { return rotation; }
Mesh* Object::getMesh() { return mesh; }
