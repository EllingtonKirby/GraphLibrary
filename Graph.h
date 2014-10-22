#ifndef GRAPH
#define GRAPH

//Put any include statements here
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <queue>
#include <set>
#include <unordered_set>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <iterator> 
//This class will be used to create a graph library.
enum Type {DIRECTED,UNDIRECTED};
enum Color {WHITE, GREY, BLACK};
enum Part {RED, BLUE, NEUTRAL};
struct Node{
        int number;
        Color color;
	int distance;
	Part p;
};
struct edge{
	int v1;
	int v2;
	double weight;
};
class Graph{
        private:
                //Put your private data members here
                //Put your private methods here
                //Holds an adjacency matrix of doubles.
                double ** edgeArray;
                //A vector of nodes.
                std::vector<Node> nodes;
                //Stores whether the graph is directed or not.
                Type direction;
                //Stores the number of vertices in the graph.
                int vertices;
                int numEdges;
		void DFTHelper(Node &s, std::ofstream &os);
        	void BFTHelp(std::multiset<int> &seen, int source);
		void DFTHelp(int source, int dest);
		std::list<int> MSTHelp(std::multiset<int> &seen, int source);
		class Ordering{
			public:
				bool operator()(const edge &e1, const edge &e2){
					return e2.weight < e1.weight; 
				}
		};
		std::priority_queue<edge, std::vector<edge>, Ordering> E;
		bool PartHelp(std::multiset<int> &seen, int source);
	public:
                //Construct an empty graph of the specified type
                Graph(Type t);
                //Delete a graph
                ~Graph();
                //Read a graph from a file
                void readFromFile(std::string file);
                //Write a graph to a file
                void writeToFile(std::string file);
                //Empty
                bool empty();
                //Add edge
                void addEdge(int v1, int v2, double weight);
                //Add vertex
                void addVertex();
                //Count connected components
                int numConnectedComponents();
                //Tree check
                bool tree();
                //Depth First Traverse - proceed from source
                void DFT(int source, std::string file);
                //Breadth First Traverse - proceed from source
                void BFT(int source, std::string file);
                //Closeness - determine minimum number of edges to get
                // from one node to the other
                int closeness(int v1, int v2);
                //* Partition - determine if you can partition the graph
                bool partitionable();
                //* MST - print the minimum spanning tree of the graph
                //to a file with the passed name (return whether or not
        //this operation was successful)
                bool MST(std::string file);
                //* Step Away - print the nodes who are a degree of
                //closeness from the source to a file with the passed name
                void stepAway(int source, int closeness, std::string file);
};

#endif
