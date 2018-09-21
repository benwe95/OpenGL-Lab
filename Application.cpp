#include "Application.h"
#include <iostream>

using namespace std;

static void error_callback(int error, const char* description)
{
    cout << "Error: " << description << endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int Application::run()
{
    GLFWwindow* window;

    cout << "Hello OpenGL" << endl;
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        return EXIT_FAILURE;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "OpenGL Lab", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        cout << "Failed to initialize GLEW" << endl;
        return EXIT_FAILURE;
    }

    glfwSwapInterval(1);

    setup();

    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        update();
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        GLuint error = glGetError();
        if(error != GL_NO_ERROR)
        {
            cout << "OpenGL ERROR: " << error << endl;
        }
    }

    teardown();

    glfwDestroyWindow(window);

    glfwTerminate();

    return EXIT_SUCCESS;
}

int Application::getWidth()
{
    return width;
}

int Application::getHeight()
{
    return height;
}