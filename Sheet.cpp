#include "Sheet.h"
#include "texture.h"
#include "shader.h"

#include <iostream>

using namespace std;

#define VERTIC 50
#define HORIZ 50

Sheet::Sheet(Program *program) : Mesh(program)
{   
    /* Définition des attributs d'un vertex 
        - coordonnées dans l'espace local
        - coordonnées normales;
        - coordonnées textures
    */
	struct Vertex
	{
		vec3 position;
		float p1;
		vec3 velocity;
		float p2;
		vec3 acceleration;
		float p3;
		vec3 normal;
		float p4;
		vec3 tangent;
		float p5;
		vec2 texture;
		float p6, p7;
	};

	
	Vertex vertices[VERTIC*HORIZ];

	float x, y = 0;
	int index = 0;
    float texX, texY = 0;
    float texXIncrement = 1/float(VERTIC-1);
    float texYIncrement = 1/float(HORIZ-1);
	/* Génération des vertex, positionnés de part et d'autre de l'origine du
	repère local */
    for(int row=0; row< HORIZ; row++)
	{	
		x = 0;
		y = row - (HORIZ-1)/float(2);
		y = y/float((HORIZ-1)/2);

		for(int column=0; column<VERTIC; column++)
		{	
			/* La coordonée est translatée pour la recentrer et ensuite elle est normalisée */
			x = column - (VERTIC-1)/float(2);
			x = x/float((VERTIC-1)/2);

			vertices[index].position = vec3(x, y, 1.5f);

            vertices[index].velocity = vec3(0.f, 0.0f, 0.f);
			vertices[index].acceleration = vec3(0.f, 0.0f, -0.1f);
			vertices[index].normal = vec3(0.f, 0.f, 1.f);
			vertices[index].tangent = vec3(1.f, 0.f, 0.f);

			//cout << index << " : ( " << x << " : " << y << " )" <<endl;

            /* Pour associer une texture au mesh, il faut dire pour chaque vertex 
            (de chaque triangle) à quelle partie de la texture (l'image png) 
            il correspond -> ensuite, le remplissage de chaque triangle se fait
            automatiquement par interpolation.

            Chaque vertex possède donc deux coordonées texX et texY qui correspondent
            aux positions (x, y) sur l'image de la texture.

            Ces valeurs sont comprises entre 0 et 1 
            -> (0, 0) coin inférieur gauche de l'image texture
            -> (1, 1) coin supérieur droit de l'image texture */
			texX = float(column) * float(texXIncrement);
			texY = float(row) * float(texYIncrement);
			vertices[index].texture = vec2(texX, texY);

            //cout << index << " texCoord: ( " << texX << " : " << texY << " )" <<endl;

			index++;
		}
    }

	indicesCount = (VERTIC-1) * (HORIZ-1) * 6; 
	indices = new GLuint[indicesCount];
	/* Le tissu peut être vu comme une succession de petit rectangle, 
	chacun étant composé de deux triangles, soit 6 vertices que l'on peut 
	décrire comme ceci:

	K2     K2+1
	 _____
	| \   |
	|  \  |
	|___\_|
	K1     K1+1      
	
	Les triangles sont parcourus de la façon suivante:
	K1   => K2 => K1+1
	K1+1 => K2 => K2+1 */
	int currentIndex = 0;
	int k1, k2 = 0;
	for(int row=0; row < HORIZ-1; row++)
	{	
		k1 = row * VERTIC;
		k2 = k1 + VERTIC;

        /*DEBUG
		cout << "\n\n============= xIncrement: " << xIncrement << " =============\n" << endl;
		*/

		/* Pour chaque rectangle, on reporte les indices des vertex (selon la liste 'vertices')
		qui composent les deux triangles */
		for (int column=0; column < VERTIC-1; column ++)
		{
			indices[currentIndex] = k1;
			indices[currentIndex+1] = k2;
			indices[currentIndex+2] = k1+1;
			indices[currentIndex+3] = k1+1;
			indices[currentIndex+4] = k2;
			indices[currentIndex+5] = k2+1; 

			/* DEBUG
            cout << currentIndex
				 << "\tleft: " << k1 << " => " << k2 << " => " << k1+1 
				 << "\n\tright: " << k1+1 << " => " << k2 << " => " << k2+1 << endl;

			cout << "\n\n\t\t------- yIncrement: " << yIncrement << " -------\n" << endl;
        	cout << "\n\t\t\tk1: " << k1 << "\tx: " << vertices[k1].position[0] << "\ty: " << vertices[k1].position[1]
             	 << "\n\t\t\tk2: " << k2 << "\tx: " << vertices[k2].position[0] << "\ty: " << vertices[k2].position[1]
				 << "\n\t\t\tk1+1: " << k1+1 << "\tx: " << vertices[k1+1].position[0] << "\ty: " << vertices[k1+1].position[1]
				 << "\n\t\t\tk2+1: " << k2+1 << "\tx: " << vertices[k2+1].position[0] << "\ty: " << vertices[k2+1].position[1] << endl;
			*/

			k1 ++ ;
			k2 ++ ;

			currentIndex = currentIndex + 6;
		}
	}

	GLint vpos_location, vtex_location, tex_location, vnor_location;

	// Creation de la texture
	texture = createTexture("textures/Knited_Fabric_001_SD/Knited_Fabric_001_COLOR.png");

	// Récupération des "location" des variable "vPos" et "vCol" du pipeline
	vpos_location = glGetAttribLocation(program->getId(), "vPos");
	vtex_location = glGetAttribLocation(program->getId(), "vTex");
	vnor_location = glGetAttribLocation(program->getId(), "vNorm");
	tex_location = glGetUniformLocation(program->getId(), "Tex");

	// Création du VertexArray
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	// Définition du vertexBuffer
	GLuint vertex_buffer;
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

	GLuint collide_shader = loadShader("collide.comp", GL_COMPUTE_SHADER);
	collide = new Computer(collide_shader);
	collide->setData(1, vertex_buffer);

	GLuint cinematic_shader = loadShader("cinematic.comp", GL_COMPUTE_SHADER);
	cinematic = new Computer(cinematic_shader);
	cinematic->setData(1, vertex_buffer);
}

Sheet::~Sheet()
{
	delete[] indices;
	delete collide;
	delete cinematic;
}

void Sheet::update()
{
	collide->compute(79, 1, 1);
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	cinematic->compute(79, 1, 1);
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
}

/* Fonction qui définit ce qui sera affiché à l'écran */
void Sheet::render(mat4 model)
{
	/* On lie la matrice de modèle au vertex shader (en passant par le 'program' associé au mesh) */
	getProgram()->begin(model);

	// Selection du VertexArray
	glBindVertexArray(vertex_array);

	// ACTIVATION de la texture 
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, texture);

	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, (void*) 0);

	getProgram()->end();
}