// Author: Quentin Lurkin
// Based on Simple GLFW example by Camilla Löwy <elmindreda@glfw.org>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vmath.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
using namespace vmath;

static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
{
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 0.f, 0.f, 1.f }
};

string loadShader(string path)
{
    ifstream in(path);
    stringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

static void error_callback(int error, const char* description)
{
    cout << "Error: " << description << endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void)
{
    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program, vertex_array;
    GLint mvp_location, vpos_location, vcol_location;

    cout << "Hello OpenGL" << endl;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "OpenGL Lab", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        cout << "Failed to initialize GLEW" << endl;
        return -1;
    }

    glfwSwapInterval(1);

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    const char* srcVert = loadShader("shader.vert").c_str();
    glShaderSource(vertex_shader, 1, &srcVert, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* srcFrag = loadShader("shader.frag").c_str();
    glShaderSource(fragment_shader, 1, &srcFrag, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

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

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        mat4 m, p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        m = rotate((float) glfwGetTime()*10, 0.0f, 0.0f, 1.0f);
        p = ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mvp = p * m;

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
