#include "Object.h"

#include <iostream>

using namespace std;

Object::Object(Mesh* mesh, vec3 position, quaternion roration)
{
    this->mesh = mesh;
    this->position = position;
    this->rotation = rotation;
}

Object::~Object()
{

}

void Object::render(float time)
{
    //TODO: apply translation and rotation
    GLuint program = mesh->getProgram();
    GLint model_location = glGetUniformLocation(program, "Model");
    mat4 model = rotate(time*10, 0.0f, 0.0f, 1.0f);
    glUseProgram(program);
    glUniformMatrix4fv(model_location, 1, GL_FALSE, (const GLfloat*) model);
    mesh->render(time);
}

vec3 Object::getPosition() { return position; }
quaternion Object::getRotation() { return rotation; }
Mesh* Object::getMesh() { return mesh; }
