// Author: Quentin Lurkin

#include "Application.h"
#include "Triangle.h"
#include "Object.h"

#include <iostream>

using namespace std;

class MyApplication : public Application
{
    private:
    Triangle *triangle;
    Object *root, *o, *o2;

    public:
    void setup()
    {
        GLuint vertex_shader, fragment_shader, program;
        GLint view_location;

        // Chargement des shaders
        fragment_shader = loadShader("shader.frag", GL_FRAGMENT_SHADER);
        vertex_shader = loadShader("shader.vert", GL_VERTEX_SHADER);

        // Creation du pipeline 
        program = createProgram(vertex_shader, fragment_shader);

        // Create a Triangle instance
        triangle = new Triangle(program);

        root = new Object(triangle, vec3(0, 0, 0), mat4::identity());
        o = new Object(triangle, vec3(3, 0, 0), mat4::identity());
        o2 = new Object(triangle, vec3(1.5, 0, 0), mat4::identity());

        o->setParent(root);
        o2->setParent(o);
    }

    void update()
    {
        //object->update((float) glfwGetTime());
        root->setRotation(rotate((float) glfwGetTime()*20, 0.0f, 0.0f, 1.0f));
        o->setRotation(rotate((float) glfwGetTime()*20, 0.0f, 0.0f, 1.0f));
        o2->setRotation(rotate((float) glfwGetTime()*20, 0.0f, 0.0f, 1.0f));
    }

    void render()
    {
        mat4 mvp;
        
        // Création de la matrice ModelViewProjection
        float ratio;
        ratio = getWidth() / (float) getHeight();
        mvp = ortho(-ratio*5, ratio*5, -5.f, 5.f, 1.f, -1.f);

        root->render(mvp);
    }

    void teardown()
    {
        delete root;
        delete triangle;
    }
};

int main(void)
{
    return MyApplication().run();
}

//TODO: GL Error managment, Object séparation, Log, Exception, Renderable, Updatable, Object Tree, Check comment