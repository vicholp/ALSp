build:
	g++ -o main main.cpp
	
run:
	./main

warning:
	g++ -Wall -o main main.cpp
	
valgrind: 
	g++ -o main main.cpp
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
		 ./main 

all: build run
