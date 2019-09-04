/* Gestion de la fenêtre de rendu */
#include "Application.h"

/* */
#include "Program.h"
#include "Object.h"
#include "Sheet.h"
#include "Sphere.h"

#include <iostream>

using namespace std;

/* On instancie la classe application pour créer une nouvelle fenêtre graphique */
class MyApplication : public Application
{
    private:
        Program *program;
        Object *root_sphere, *root_sheet;
        Sheet *sheet;
        Sphere *sphere;
    public:
    
    void setup()
    {
        GLuint vertex_shader, fragment_shader;
        GLint view_location;

        /* Chargement des shaders - 
        REM: (1) la fonction loadShader est définie par le fichier shader.cpp
             (2) les shaders sont définis dans des fichiers à part */
        fragment_shader = loadShader("shader.frag", GL_FRAGMENT_SHADER);
        vertex_shader = loadShader("shader.vert", GL_VERTEX_SHADER);

        /* Création du pipeline : Program(GLuint vertex_shader, GLuint fragment_shader)
        Le PIPELINE correspond à 
        
        | Vertex Data |->| Primitive Processing |->| Vertex Shader |->|Tesselation Control Shader |->| Tesselation |->
        | Tessellation Eval Shader |->| Geometry Shader |->| Primitive Assembly |->| Rasterizer |->| Fragment Shader |->
        | Depth & Stencil |->| Color Buffer Blend |->| Dither |->| Frame Buffer |
        
         */
        program = new Program(vertex_shader, fragment_shader);

        /* Création d'une instance MESH */
        sheet = new Sheet(program);
        sphere = new Sphere(program);

        /* Création d'un objet: Object(Mesh* mesh, vec3 position, mat4 rotation) */
        root_sphere = new Object(sphere, vec3(0, 0, 0), mat4::identity());
        root_sheet = new Object(sheet, vec3(0, 0, 0), mat4::identity());
    }

    void update()
    {
        //sheet->update();
        //root_sphere->setRotation(rotate((float) glfwGetTime()*20, 0.0f, 0.0f, 1.0f));
        //root_sheet->setRotation(rotate((float) glfwGetTime()*20, 0.0f, 0.0f, 1.0f));
        //sheet->update();
    }

    /* Implémentation de la fonction virtuelle Application::render() */
    void render()
    {
        mat4 projection, view, model_sheet, model_sphere;

        /* Le ratio est caclué sur base des dimensions de la fenêtre d'affichage 
        REM: les matrices de vue et de projection sont COMMUNES à tous les objets de l'espace ..... */        
		float ratio;
		ratio = getWidth() / (float) getHeight();
		projection = perspective(60.f, ratio, 0.1f, 10);
		view = lookat(vec3(0, -2, 2), vec3(0, 0, 0), vec3(0, 0, 1));

		/* Application.cpp */
        setProjection(projection);
        setView(view);

        /* ...... tandis que la matrice de model (qui positionne l'objet dans l'espace global) est propre
        à caque objet*/
        model_sphere = mat4::identity();
        /* La matrice de modèle du tissu fait en sorte que celui-ci soit translaté de X unités vers le haut
        par rapport au centre du repère GLOBAL */
        model_sheet = mat4::identity();
        model_sheet[0][3] = 10;

        root_sphere->render(model_sphere);
        root_sheet->render(model_sphere);
    }

    void teardown()
    {
        delete sheet;
        delete root_sphere;
        delete root_sheet;
        delete program;
        delete sphere;
    }
};

int main(void) {
    return MyApplication().run();
}