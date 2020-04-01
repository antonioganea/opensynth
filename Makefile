main.o: main.cpp
	g++ main.cpp -o main.o -c

main: main.o
	g++ main.o -o main -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

run: main
	./main

clean:
	rm ./main.o
	rm ./main