build:
	g++ -o main src/main.cpp 
	
run:
	./main

warning:
	g++ -Wall -o main src/main.cpp
	
valgrind: 
	g++ -g -O0 -o main src/main.cpp
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
		 ./main 

all: build run
