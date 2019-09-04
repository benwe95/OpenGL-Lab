#include "shader.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

/* Charge le shader -> utilisé par la fonction main, au setup() de l'Application 
A l'appel de cette fonction, on définit manuellement le type du shader via 'shaderType, 
ex: FRAGMENT_SAHDER, VERTEX_SHADER */
GLuint loadShader(const char* path, GLenum shaderType)
{
    ifstream in(path, ios::in);
    /* le contenu du shader que l'on parse en string */
    string content;

    if(!in.is_open())
    {
        cout << "File " << path << " not found" << endl;
        return -1;
    }

    /* REM: les shaders sont traités au format STRING !!
    Pour plus de facilité, on les écrit dans des fichiers à part (shader.vert, shader.frag)
    et ensuite on les parse au format string */
    string line = "";
    while(!in.eof()){
        getline(in, line);
        content.append(line+"\n");
    }

    in.close();

    const char* src = content.c_str();


    /* Création d'un shader correspondant au type donné */
	GLuint id = glCreateShader(shaderType);
    /* Assignation du code source */
	glShaderSource(id, 1, &src, NULL);
    /* Compilation du shader
    RAPPEL: le shader n'est rien d'autre qu'un programme destiné à être exécuté par le processeur graphique.
    Comme un programme CPU, il doit d'abord être compilé 
    (REM: il est compilé à l'execution du programme et non pas avant dans le exe) */
	glCompileShader(id);

    /* Vérification du status de la compilation, OK ou non */
	GLint status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
		
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(id, infoLogLength, NULL, strInfoLog);
		
		cout << "Shader Compile Error (" << path << "): " << strInfoLog;
		delete[] strInfoLog;
	}

    /* Si aucun problème de compilation, alors la référence vers le shader est renvoyée */
    return id;

}