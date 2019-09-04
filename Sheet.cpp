#include "Sheet.h"
#include "texture.h"
#include "shader.h"

#include <iostream>

using namespace std;

#define X_VERTIC 5
#define Y_HORIZ 5

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
        vec3 normal;
        vec2 texture;
        vec3 velocity;
        vec3 acceleration;
		vec3 tangent;
    };

	
	Vertex vertices[X_VERTIC * Y_HORIZ];

	float x, y = 0;
	int index = 0;
	/* Génération des vertex, positionnés de part et d'autre de l'origine du
	repère local */
	for(int xIncrement=0; xIncrement<X_VERTIC; xIncrement++)
	{

		for(int jIncrement=0; jIncrement<Y_HORIZ; jIncrement++)
		{
			x = (xIncrement - (X_VERTIC/2)) / float(X_VERTIC/2);
			y = (jIncrement - (Y_HORIZ/2)) / float(Y_HORIZ/2);
			vertices[index].position = vec3(x, y, 1.5);

			vertices[index].velocity = vec3(0.f, 0.0f, 0.f);
			vertices[index].acceleration = vec3(0.f, 0.0f, 0.f);
			vertices[index].normal = vec3(0.f, 0.f, 1.f);
			vertices[index].tangent = vec3(1.f, 0.f, 0.f);

			float nx = float(jIncrement) / float(50);
			float ny = float(xIncrement) / float(50);
			vertices[index].texture = vec2(nx, ny);

			/* DEBUG
			cout << index << " (" << x << " : " << y << ")\t";*/

			index++;
		}

		cout << "\n";
	}

	int indicesCount = (X_VERTIC-1)*(Y_HORIZ-1)*6;
	indices = new GLuint[indicesCount];
	/* Le tissu peut être vu comme une succession de petit rectangle, 
	chacun étant composé de deux triangles, soit 6 vertices que l'on peut 
	décrire comme ceci:

	K1     K1+1
	 _____
	|   / |
	|  /  |
	|_/___|
	K2     K2+1      
	
	Les triangles sont parcourus de la façon suivante:
	K1   => K2 => K1+1
	K1+1 => K2 => K2+1 */
	int k1, k2;
	int currentIndex = 0;
	for (int xIncrement = 0; xIncrement<X_VERTIC-1; xIncrement++)
	{
		k1 = xIncrement * (X_VERTIC);
		k2 = k1 + X_VERTIC;

        /*DEBUG
		cout << "\n\n============= xIncrement: " << xIncrement << " =============\n" << endl;
		*/

		/* Pour chaque rectangle, on reporte les indices des vertex (selon la liste 'vertices')
		qui composent les deux triangles */
		for(int yIncrement = 0; yIncrement<Y_HORIZ-1; yIncrement++)
		{
			indices[currentIndex] = k1;
			indices[currentIndex+1] = k2;
			indices[currentIndex+2] = k1+1;
			indices[currentIndex+3] = k1+1;
			indices[currentIndex+4] = k2;
			indices[currentIndex+5] = k2+1;

			/* DEBUG
			cout << "\n\n\t\t------- yIncrement: " << yIncrement << " -------\n" << endl;
        	cout << "\n\t\t\tk1: " << k1 << "\tx: " << vertices[k1].position[0] << "\ty: " << vertices[k1].position[1]
             	 << "\n\t\t\tk2: " << k2 << "\tx: " << vertices[k2].position[0] << "\ty: " << vertices[k2].position[1]
				 << "\n\t\t\tk1+1: " << k1+1 << "\tx: " << vertices[k1+1].position[0] << "\ty: " << vertices[k1+1].position[1]
				 << "\n\t\t\tk2+1: " << k2+1 << "\tx: " << vertices[k2+1].position[0] << "\ty: " << vertices[k2+1].position[1] << endl;
			*/
			
			currentIndex = currentIndex + 6;
			k1++;
			k2++;
		}
	}


	GLint vpos_location, vtex_location, tex_location, vnor_location;

	// Creation de la texture
	texture = createTexture("textures/Knited_Fabric/Knited_Fabric.png");

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

	/*GLuint acceleration_shader = loadShader("acceleration.comp", GL_COMPUTE_SHADER);
	acceleration = new Computer(acceleration_shader);
	acceleration->setData(1, vertex_buffer);*/

	GLuint cinematic_shader = loadShader("cinematic.comp", GL_COMPUTE_SHADER);
	cinematic = new Computer(cinematic_shader);
	cinematic->setData(1, vertex_buffer);
}

Sheet::~Sheet()
{
	delete[] indices;
	delete collide;
	//delete acceleration;
	delete cinematic;
}

void Sheet::update()
{
	collide->compute(79, 1, 1);
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	/*acceleration->compute(1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);*/
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

	int trianglesCount = (X_VERTIC-1) * (Y_HORIZ-1);
	glDrawElements(GL_TRIANGLES, trianglesCount*6, GL_UNSIGNED_INT, (void*) 0);

	getProgram()->end();
}