#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <algorithm>

#include "vmath.h"
#include "Mesh.h"

using namespace vmath;
using namespace std;

/* la classe Object représente un element de l'espace (sphère, cube, drap, ...)
Cet élément est défini par un ensemble de vertices ( = MESH).
L'élément (chacun de ses vertex) va subir différentes transformations pour être positionné
correctement dans l'espace de rendu.
Ces transformations sont définies par le VERTEX_SHADER qui est lié à un 'program', lui-même
lié à un 'Mesh' -> MAIS il faut leur fournir les valeurs des matrices de PROJ, VIEW et MOD
qui sont propre à chaque objet */
class Object
{
	private:
	vec3 position;
	mat4 rotation;
	Mesh* mesh;
	vector<Object*> children;
	Object* parent;


	public:
	Object(Mesh* mesh, vec3 position, mat4 roration);
	~Object();
	void render(mat4 model);
	void setParent(Object* parent);
	void addChild(Object* child);
	void unsetParent();
	void removeChild(Object* child);
	vector<Object*> getChildren();

	vec3 getPosition();
	mat4 getRotation();
	void setPosition(vec3 position);
	void setRotation(mat4 rotation);
	Mesh* getMesh();
};

#endif