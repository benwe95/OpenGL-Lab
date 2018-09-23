all: lab

lab: main.o Application.o shader.o Triangle.o
	g++ -o lab main.o Application.o shader.o Triangle.o -lglfw3 -lglew32 -lopengl32 

main.o: main.cpp
	g++ -o main.o -c main.cpp

Application.o: Application.cpp
	g++ -o Application.o -c Application.cpp

shader.o: shader.cpp
	g++ -o shader.o -c shader.cpp

Triangle.o: Triangle.cpp
	g++ -o Triangle.o -c Triangle.cpp

clean:
	rm -rf *.o

mrproper: clean
	rm -rf lab