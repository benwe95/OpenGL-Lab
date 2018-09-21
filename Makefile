all: lab

lab: main.o Application.o shader.o
	g++ -o lab main.o Application.o shader.o -lglew32 -lopengl32 -lglfw3

main.o: main.cpp
	g++ -o main.o -c main.cpp

Application.o: Application.cpp
	g++ -o Application.o -c Application.cpp

shader.o: shader.cpp
	g++ -o shader.o -c shader.cpp

clean:
	rm -rf *.o

mrproper: clean
	rm -rf lab