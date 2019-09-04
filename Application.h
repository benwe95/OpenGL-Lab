#ifndef APPLICATION_H
#define APPLICATION_H

/* glew.h est une libraire qui permet d'accéder aux fonctions impélmentées par le hardware graphique.
   Ces fonctions sont définies par les drivers de notre carte graphique, glew ne fait que POINTER/REFERENCER */
#include <GL/glew.h>

/* glf3.h est une bibliothèque qui permet de générer des fenêtres cross-platform (pas de lien avec OpenGL)
   -> on va l'utiliser pour générer un CONTEXTE OPENGL et ainsi afficher le RENDU graphique de notre projet */
#include <GLFW/glfw3.h>

/* vmath.h est une librairie qui implémente toues sortes de fonctions mathématiques liées aux vecteurs/matrices 
   et qui sont utiles pour le rendement graphique */
#include "vmath.h"

/* permet de charger un shader REM: les shaders ne sont pas explicitement utilisés ici 
-> ils sont appelés dans le fichier main.cpp par la méthode setup()*/
#include "shader.h"

using namespace vmath;


/* Cette classe est utilisée pour la gestion de la fenêtre graphique et de son rendu, la création du contexte OpenGL, ....
avec une gestion des erreurs
--> pas d'OpenGL interessant ici */
class Application
{
    private:

        int width = 0;
        int height = 0;
        /*  RAPPEL THEORIQUE:
            - Les vertices VISIBLES dans la fenêtre sont exprimés dans leurs coordonnées Normalisées 
            (NDC-> Normalized Device Coordinate) aka x, y, z comprises entre -1.0 et 1.0
            - Pour arriver à ces coords, les verctices subissent des changements/transformations pour passer
            d'un espace/repère de rendu à un autre. Ces transfos se font aux moyen des MATRICES DE TRANSFORMATIONS
            parmi lesquelles: Model, View, Projection

            - LOCAL space --- MODEL matrix ---> WORLD space
            - WORLD space --- VIEW matrix  ---> VIEW space
            - VIEW space  --- PROJECTION matrix ---> CLIP space (espace de clipping) */
        static mat4 projection;
        static mat4 view;

    public:
        /* REM: les fonctions virtuelles ne sont pas implémentées par la classe Application elle-même 
        Elles sont implémentées dans le fichier main, qui fait lui-même appel aux fonctions update(), render() des 
        différents objets */
        virtual void setup() = 0;
        virtual void update() = 0;
        virtual void render() = 0;
        virtual void teardown() = 0;
        int run();
        int getWidth();
        int getHeight();
        static void setProjection(mat4 matrix);
        static mat4 getProjection();
        static void setView(mat4 matrix);
        static mat4 getView();
};

#endif