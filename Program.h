#ifndef PROGRAM_H
#define PROGRAM_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vmath.h"

using namespace vmath;

/* Un program permet de faire exécuter des shaders par la garte graphique.
En effet, un shader lui-même n'est pas exécutable par le CPU, il faut donc le passer à un
program (qui lui est éxécutable) pour l'envoyer vers le GPU */
class Program
{
    private:
    GLuint program;
    GLint proj_location, view_location, model_location;

    public:
    Program(GLuint vertex_shader, GLuint fragment_shader);
    void setModelMatrix(mat4 matrix);
    void begin();
    void begin(mat4 model_matrix);
    void end();
    void bind();
    void unbind();
    GLuint getId();
};

#endif