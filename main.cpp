// Author: Quentin Lurkin

#include "Application.h"
#include "Triangle.h"
#include "Object.h"

#include <iostream>

using namespace std;

class MyApplication : public Application
{
    private:
    Triangle* triangle;
    Object* object;

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

        object = new Object(triangle, vec3(0, 0, 0), mat4::identity());
    }

    void update()
    {
        object-> update((float) glfwGetTime());
    }

    void render()
    {
        mat4 mvp;
        
        // Création de la matrice ModelViewProjection
        float ratio;
        ratio = getWidth() / (float) getHeight();
        mvp = ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);

        object->render(mvp);
    }

    void teardown()
    {
        delete object;
        delete triangle;
    }
};

int main(void)
{
    return MyApplication().run();
}

//TODO: GL Error managment, Object séparation, Log, Exception, Renderable, Updatable, Object Tree, Check comment