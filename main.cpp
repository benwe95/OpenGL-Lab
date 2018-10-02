// Author: Quentin Lurkin

#include "Application.h"
#include "Table.h"
#include "Object.h"

#include <iostream>

using namespace std;

class MyApplication : public Application
{
	private:
	Table *table;
	Object *root;

	public:
	void setup()
	{
		GLuint vertex_shader, fragment_shader, program;
		GLint view_location;

		// Chargement des shaders
		fragment_shader = loadShader("shader.frag", GL_FRAGMENT_SHADER);
		vertex_shader = loadShader("shader.vert", GL_VERTEX_SHADER);

		// Creation du pipeline 
		program = createProgram(vertex_shader, fragment_shader);

		// Create a Mesh instance
		table = new Table(program);

		// Create an Object
		root = new Object(table, vec3(0, 0, 0), mat4::identity());
	}

	void update()
	{
		// Update rotation of the object
		root->setRotation(rotate((float) glfwGetTime()*20, 0.0f, 0.0f, 1.0f));
	}

	void render()
	{
		mat4 mvp;
		
		// Création des matrice View et Projection
		float ratio;
		ratio = getWidth() / (float) getHeight();
		mvp = perspective(30.f, ratio, 0.1f, 10);
		mvp = mvp * lookat(vec3(0, -2, 2), vec3(0, 0, 0), vec3(0, 0, 1));

		root->render(mvp);
	}

	void teardown()
	{
		delete root;
		delete table;
	}
};

int main(void)
{
	return MyApplication().run();
}

//TODO: GL Error managment, Object séparation, Log, Exception, Renderable, Updatable, Object Tree, Check comment