// Author: Quentin Lurkin
// Based on Simple GLFW example by Camilla Löwy <elmindreda@glfw.org>

#include "Application.h"
#include "Triangle.h"

#include <iostream>

using namespace std;

class MyApplication : public Application
{
    private:
    GLint mvp_location;
    Triangle* triangle;

    public:
    void setup()
    {
        GLuint vertex_shader, fragment_shader, program;

        // Chargement des shaders
        fragment_shader = loadShader("shader.frag", GL_FRAGMENT_SHADER);
        vertex_shader = loadShader("shader.vert", GL_VERTEX_SHADER);
        
        // Creation du pipeline 
        program = createProgram(vertex_shader, fragment_shader);

        // Récupération de la "location" de la variable "MVP" du pipeline
        mvp_location = glGetUniformLocation(program, "MVP");

        // Sélection du pipeline courant
        glUseProgram(program);

        // Create a Triangle instance
        triangle = new Triangle;
    }

    void update()
    {
        mat4 m, p, mvp;
        
        // Création de la matrice ModelViewProjection
        float ratio;
        ratio = getWidth() / (float) getHeight();
        m = rotate((float) glfwGetTime()*10, 0.0f, 0.0f, 1.0f);
        p = ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mvp = p * m;

        // Association de la matrice ModelViewProjection à la variable "MVP" du pipeline
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
    }

    void render()
    {
        triangle->render();
    }

    void teardown()
    {
        delete triangle;
    }
};

int main(void)
{
    return MyApplication().run();
}

//TODO: GL Error managment, Object séparation, Log, Exception, Renderable, Updatable, Object Tree