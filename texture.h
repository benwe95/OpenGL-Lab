#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>


/* Crée une texture à partir d'une image. 
Cette texture sera transformée pour correspondre aux transformation subies par l'objet
L'association entre la texture et l'objet se fait dans pour chaque object de type Mesh */
GLuint createTexture(const char *filename);

#endif