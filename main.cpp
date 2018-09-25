// Author: Quentin Lurkin
// Based on Simple GLFW example by Camilla Löwy <elmindreda@glfw.org>

#include "Application.h"
#include "Triangle.h"
#include "Object.h"

#include <iostream>

using namespace std;

class MyApplication : public Application
{
    private:
    GLint proj_location;
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

        // Récupération de la "location" de la variable "MVP" du pipeline
        proj_location = glGetUniformLocation(program, "Projection");
        view_location = glGetUniformLocation(program, "View");

        // Sélection du pipeline courant
        glUseProgram(program);
        
        mat4 view = mat4::identity();
        glUniformMatrix4fv(view_location, 1, GL_FALSE, (const GLfloat*) view);

        // Create a Triangle instance
        triangle = new Triangle(program);

        object = new Object(triangle, vec3(0, 0, 0), quaternion(0, 0, 0, 0));
        cout << "main.setup() OK" << endl;
    }

    void update()
    {
        mat4 proj;
        
        // Création de la matrice ModelViewProjection
        float ratio;
        ratio = getWidth() / (float) getHeight();
        proj = ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);

        // Association de la matrice ModelViewProjection à la variable "MVP" du pipeline
        glUniformMatrix4fv(proj_location, 1, GL_FALSE, (const GLfloat*) proj);
    }

    void render()
    {
        object->render((float) glfwGetTime());
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