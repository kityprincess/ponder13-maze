###############################################################
# Program:
#     Week 13, Graph
#     Brother XXXXX, CS265
# Author:
#     <your name here>
# Summary:
#     <put a description here>
###############################################################

##############################################################
# The main rule
##############################################################
a.out: week13.o graph.o maze.o
	g++ -o a.out week13.o graph.o maze.o -g
	tar -cf week13.tar *.h *.cpp makefile

##############################################################
# The individual components
#      week13.o     : the driver program
##############################################################
week13.o: graph.h vertex.h week13.cpp
	g++ -c week13.cpp -g

graph.o: graph.h set.h vertex.h graph.cpp
	g++ -c graph.cpp -g

maze.o: maze.cpp maze.h vertex.h graph.h
	g++ -c maze.cpp -g
