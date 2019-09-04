#include "sphere.h"

#include <iostream>
#include "shader.h"
#include "texture.h"

#define PI 3.14159265
#define RADIUS 1
#define STACKS 18
#define SECTORS 36

using namespace std;

Sphere::Sphere(Program *program) : Mesh(program)
{   

    /* La sphere peut être découpée en STACKS (suivant les parallèles) et en SECTORS (suivant les méridiens)
       ce qui engendre des formes "rectangulaires" qui peuvent être vues comme 
       des combinaisons de triangles 
       (SAUF la première et la dernière stack où les formes sont de simple triangles)*/
    this->radius = RADIUS;
    this->sectors = SECTORS;
    this->stacks = STACKS;
    this->vertexCount = this->sectors * this->stacks * 2;

	// Définition des coordonnées des VertexData
	struct Vertex
	{
        float x, y, z;      // Positions
        float xn, yn, zn;   // Normales aux surfaces (éclairage)
        float u, v;         // Textures
	};
    
    Vertex vertices[vertexCount];

    // l'angle THETA (XOY plan) varie de   0° à 360° (2PI) -> l'arc sous-tendu par un secteur vaut:
    float sectorStep = 2 * PI / this->sectors; 
    // l'angle PHI   (ZOY plan) varie de -90° à +90°       -> l'arc sous-tendu par une stack vaut:
    float stackStep = PI / this->stacks;

    float currentTheta, currentPhi;
    int vertexIndex = 0;

    cout << "vertices size: " << sizeof(vertices)/sizeof(vertices[0]) <<endl;

    // Génération des vertices
    for(int stacksIncrement=0; stacksIncrement <= this->stacks; ++stacksIncrement)
    {
        currentPhi = (PI/2) - stacksIncrement*stackStep; //varation de de  phi de +90° à -90°
        float xyRadiusProjection = radius * cosf(currentPhi);
        float zCoord = radius * sinf(currentPhi);

        /* DEBUG 
        cout << "\n\n============= stacksIncrement: " << stacksIncrement << " =============\n"
        << "Phi: " << currentPhi << endl;
        cout << "\n\txyRadiusProjection: " << xyRadiusProjection << "\n\tzCoord: " << zCoord << endl;*/

        for(int sectorIncrement=0; sectorIncrement <= this->sectors; ++sectorIncrement)
        {
            currentTheta = sectorIncrement * sectorStep;
            float xCoord = xyRadiusProjection * cosf(currentTheta);
            float yCoord = xyRadiusProjection * sinf(currentTheta);

            /* DEBUG
            cout << "\n\t\t----- sectorIncrement: " << sectorIncrement << " -----\n"
            << "\t\t\tTheta: " << currentTheta << endl;*/

            vertices[vertexIndex].x = xCoord;
            vertices[vertexIndex].y = yCoord;
            vertices[vertexIndex].z = zCoord;

            vertices[vertexIndex].xn = xCoord/this->radius;
            vertices[vertexIndex].yn = yCoord/this->radius;
            vertices[vertexIndex].zn = zCoord/this->radius;

            /* DEBUG
            cout << "\n\t\t\t-> Vertex # "<< vertexIndex << "\n"
                 << "\t\t\t  x " << xCoord << "\n"
                 << "\t\t\t  y " << yCoord << "\n"
                 << "\t\t\t  z " << zCoord << endl; */

            vertices[vertexIndex].u = (float)sectorIncrement/this->sectors;
            vertices[vertexIndex].v = (float)stacksIncrement/this->stacks;

            vertexIndex = vertexIndex + 1;

        }
    }


    // Génération des triangles de la sphères en créant un index_buffer
    /* L'index buffer contient la position des vertex dans le buffer 'vertices'. Il permet d'utiliser un même 
       vertex pour dessiner plusieurs triangles, ce qui représente notamment un gain d'espace mémoire. */
    // Ce buffer contiendra donc des groupes de 3 vertex, chaque groupe représente un triangle
    /* (Voir schéma documentation)
        Chaque rectangle contient 2 triangles (donc 6 indices) 
        SAUF les stacks supérieures et inférieures qui ne contiennent que 1 triangle

        -> k1 indice du premier vertex de la stack courante
        -> k2 indice du premier vertex de la stack suivante (en-dessous) 
        
        REM: Les triangles sont dessinés suivant un ordre anti-horlogique de leurs sommets*/
    
    int indicesCount = 6 * this->sectors * (this->stacks-1); // 6 * (this->stacks-2) * this->sectors + 3 * 2 * (this->sectors)
    unsigned int indices[indicesCount];
    int k1, k2; 
    int currentIndex = 0;
    for(int stacksIncrement=0; stacksIncrement<this->stacks; ++stacksIncrement)
    {   
        // Ces indices font référence à la position des vertex dans le vertexBuffer
        k1 = stacksIncrement * (this->sectors + 1);
        k2 = k1 + this->sectors + 1;

        /* DEBUG
        cout << "\n\n============= stacksIncrement: " << stacksIncrement << " =============\n" << endl;
        cout << "\n\tk1: " << k1 << "\tx: " << vertices[k1].x << "\ty: " << vertices[k1].y << "\tz: " << vertices[k1].z
             << "\n\tk2: " << k2 << "\tx: " << vertices[k2].x << "\ty: " << vertices[k2].y << "\tz: " << vertices[k2].z << endl;*/

        /* Par secteur il y a deux triangles 
            ex: triangle de gauche (k1   => k2 => k1+1)
                triangle de droite (k1+1 => k2 => k2+1) */
        for(int sectorIncrement=0; sectorIncrement<this->sectors; ++sectorIncrement)
        {   
            // La premiere et de la derniere stack ne contiennent que 1 seul triangle par secteur

            if(stacksIncrement!=0)
            {   
                // Triangle "de gauche"
                indices[currentIndex] = k1;
                indices[currentIndex+1] = k2;
                indices[currentIndex+2] = k1+1;

                currentIndex = currentIndex + 3;
            }

            if(stacksIncrement != (this->stacks-1))
            {   
                // Triangle "de droite"
                indices[currentIndex] = k1+1;
                indices[currentIndex+1] = k2;
                indices[currentIndex+2] = k2+1;

                currentIndex = currentIndex + 3;
            }
            
            k1 = k1 + 1;
            k2 = k2 + 1;
        }
    }


    /* DEBUG
    for(int i=0; i<indicesCount; ++i)
    {
        cout << "Index #: " << i << "\t" << indices[i] <<endl;
    }*/


	// Creation de la texture
	texture = createTexture("textures/Rock_wall/diffuse.png");

    GLint vpos_location, vtex_location, tex_location, vnor_location;

	// Récupération des "location" des variable "vPos" et "vCol" du pipeline
	vpos_location = glGetAttribLocation(program->getId(), "vPos");
	vtex_location = glGetAttribLocation(program->getId(), "vTex");
	vnor_location = glGetAttribLocation(program->getId(), "vNorm");
	tex_location = glGetUniformLocation(program->getId(), "Tex");

	// Création du VertexArray
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	// Définition du vertexBuffer
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertex_buffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	GLuint index_buffer;
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indicesCount, indices, GL_STATIC_DRAW);

	program->bind();

	// Association des composantes des VertexData à la variable "vPos" du pipeline
	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) 0);

	// Association des composantes des VertexData à la variable "vNorm" du pipeline
	glEnableVertexAttribArray(vnor_location);
	glVertexAttribPointer(vnor_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) (sizeof(float) * 12));

	// Association des composantes suivantes des VertexData à la variable "vTex" du pipeline  
	glEnableVertexAttribArray(vtex_location);
	glVertexAttribPointer(vtex_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) (sizeof(float) * 20));

	// Association de la texture à la variable "Tex"
	texUnit = 1;
	glUniform1i(tex_location, texUnit);

	program->unbind();
}

Sphere::~Sphere()
{
}

void Sphere::update()
{

}

void Sphere::render(mat4 model)
{
	getProgram()->begin(model);

	// Selection du VertexArray
	glBindVertexArray(vertex_array);

	// activer la texture
    glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Affichage des 3 1ers sommets en utilisant des triangles.
    int trianglesCount = 6 * this->sectors * (this->stacks-1);
	glDrawElements(GL_TRIANGLES, trianglesCount, GL_UNSIGNED_INT, (void*) 0);



	getProgram()->end();
}