#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vmath.h"
#include "shader.h"

using namespace vmath;

class Application
{
	private:
	int width = 0;
	int height = 0;

	public:
	virtual void setup() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
	virtual void teardown() = 0;
	int run();
	int getWidth();
	int getHeight();
};

#endif