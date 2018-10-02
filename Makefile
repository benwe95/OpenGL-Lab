all: lab

lab: main.o Application.o shader.o Triangle.o Mesh.o Object.o texture.o
	g++ -o lab main.o Application.o shader.o Triangle.o Mesh.o Object.o texture.o -lglfw3 -lglew32 -lopengl32 -lpng

main.o: main.cpp
	g++ -o main.o -c main.cpp

Application.o: Application.cpp Application.h
	g++ -o Application.o -c Application.cpp

shader.o: shader.cpp shader.h
	g++ -o shader.o -c shader.cpp

Triangle.o: Triangle.cpp Triangle.h
	g++ -o Triangle.o -c Triangle.cpp

Mesh.o: Mesh.cpp Mesh.h
	g++ -o Mesh.o -c Mesh.cpp

Object.o: Object.cpp Object.h
	g++ -o Object.o -c Object.cpp

texture.o: texture.cpp texture.h
	g++ -o texture.o -c texture.cpp

clean:
	rm -rf *.o

mrproper: clean
	rm -rf lab