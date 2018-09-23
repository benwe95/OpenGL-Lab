#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Triangle
{
    public:
    Triangle();
    void render();

    private:
    GLuint vertex_array;
    GLint vpos_location, vcol_location;

};

#endif