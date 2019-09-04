#include "Program.h"
#include "Application.h"

#include <iostream>

using namespace std;

/* Gestion des objets "PROGRAM" -> il s'agit d'objets auxquels on peut attacher un shader 
ex d'objets "program": shpère, tissu, ... */
Program::Program(GLuint vertex_shader, GLuint fragment_shader)
{
    program = glCreateProgram();
    /* On associe/attache les shaders FRAGMENT et VERTEX au programme.
    Ces shaders sont écrits dans des fichiers à part (shader.frag, shader.vert) et ils sont D'ABORD compilés
    par la fonction 'loadShader' (shader.cpp) qui est appelée par main.cpp, avant d'être passés ici au programme */
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

    GLint status;

    /* Vérification des erreurs */
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);

		cout << "Program link error: " << strInfoLog;
		delete[] strInfoLog;
	}

    /* glGetUniformLocation returns an integer that represents the location of a specific uniform variable within a program object
    Une variable uniforme est une structure (STRUCT) ou un tableau (ARRAY) avec différents champs (projection, view, model)
    Les variables "Projection", "View" et "Model" existent dans le vertex shader. Il s'agit de mat4, les matrices de transformations
    utilisées pour les positionnement des vertex dans les différents espaces successifs du pipeline 
    REM: dans le shader, ont décrit les opérations entre ces matrices MAIS les matrices elles-mêmes sont crées à l'extérieur
    et sont ensuite passées par les fonctions plus bas. 
    
    Les variables de type 'uniform' sont celles du shaders, que l'on récupère et que l'on modifie ici 
    (au lieu des les hardcoder au niveau du shader lui-même)
    Cela permet donc de pouvoir réutiliser le même shader pour différents objets */
    proj_location = glGetUniformLocation(program, "Projection");
	view_location = glGetUniformLocation(program, "View");
    model_location = glGetUniformLocation(program, "Model");
}

void Program::setModelMatrix(mat4 matrix)
{
    glUniformMatrix4fv(model_location, 1, GL_FALSE, (const GLfloat*) matrix);
}

void Program::begin()
{
    bind();
    glUniformMatrix4fv(proj_location, 1, GL_FALSE, (const GLfloat*) Application::getProjection());
	glUniformMatrix4fv(view_location, 1, GL_FALSE, (const GLfloat*) Application::getView());
}

void Program::begin(mat4 model_matrix)
{
    begin();
    setModelMatrix(model_matrix);
}

void Program::end()
{
    unbind();
}

void Program::bind()
{
    /* !! Fonction qui permet l'activation du programme !! */
    glUseProgram(program); 
}

void Program::unbind()
{
    glUseProgram(0);
}

GLuint Program::getId()
{
    return program;
}