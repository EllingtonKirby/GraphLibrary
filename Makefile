PROJ = GraphLab
COMP = g++
FLAGS = -Wall -g -std=c++0x -O2 

all: Graph.o Driver.o
	ar rcs Graph.a Graph.o Driver.o 
	$(COMP) $(FLAGS) -o $(PROJ) Graph.a 

Graph.o: Graph.cpp
	$(COMP) $(FLAGS) -c Graph.cpp

Driver.o: Driver.cpp
	$(COMP) $(FLAGS) -c Driver.cpp


clean: 
	rm -f *.o $(PROJ)
