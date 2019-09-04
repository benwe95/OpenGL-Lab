#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vmath.h"

#include "Program.h"

using namespace vmath;

/* Un mesh (ou "model" ou "geometry") est une collection des vertices, qui définissent une forme globale. */
class Mesh
{
    private:
    Program *program;

    public:
    Mesh(Program *program);
    /* Récupère la référence du programme */
    Program* getProgram();
    /* Cette méthode sera propre à chaque classe de type Mesh */
    virtual void render(mat4 model) = 0;

};

#endif