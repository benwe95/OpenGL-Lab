// Author: Quentin Lurkin
// Based on Simple GLFW example by Camilla Löwy <elmindreda@glfw.org>

#include "Application.h"

#include <iostream>

using namespace std;

class MyApplication : public Application
{
    private:
    GLint mvp_location;

    public:
    void setup()
    {
        const struct
        {
            float x, y;
            float r, g, b;
        } vertices[3] =
        {
            { -0.6f, -0.4f, 1.f, 0.f, 0.f },
            {  0.6f, -0.4f, 0.f, 1.f, 0.f },
            {   0.f,  0.6f, 0.f, 0.f, 1.f }
        };
        GLuint vertex_buffer, vertex_shader, fragment_shader, program, vertex_array;
        GLint vpos_location, vcol_location;

        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        fragment_shader = loadShader("shader.frag", GL_FRAGMENT_SHADER);
        vertex_shader = loadShader("shader.vert", GL_VERTEX_SHADER);
        
        program = createProgram(vertex_shader, fragment_shader);

        mvp_location = glGetUniformLocation(program, "MVP");
        vpos_location = glGetAttribLocation(program, "vPos");
        vcol_location = glGetAttribLocation(program, "vCol");

        glGenVertexArrays(1, &vertex_array);
        glBindVertexArray(vertex_array);

        glEnableVertexAttribArray(vpos_location);
        glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                            sizeof(vertices[0]), (void*) 0);
        glEnableVertexAttribArray(vcol_location);
        glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                            sizeof(vertices[0]), (void*) (sizeof(float) * 2));

        glUseProgram(program);
    }

    void update()
    {
        mat4 m, p, mvp;
        
        float ratio;
        ratio = getWidth() / (float) getHeight();
        m = rotate((float) glfwGetTime()*10, 0.0f, 0.0f, 1.0f);
        p = ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mvp = p * m;
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
    }

    void render()
    {
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void teardown()
    {

    }
};

int main(void)
{
    return MyApplication().run();
}

//TODO: GL Error managment, Object séparation, Log, Exception