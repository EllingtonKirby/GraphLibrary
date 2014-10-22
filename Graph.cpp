#include "Graph.h"

using namespace std;

//Construct an empty graph of the specified type vertices is initialized to one to ease the creation of the matrix
Graph::Graph(Type t){
        direction = t;
        vertices = 1;
        numEdges = 0;
        edgeArray = new double*[1]; edgeArray[0] = new double[1];
        edgeArray[0][0] = -1;
}

//Delete a graph
//Iterates through deleting all values in the dynamic array, then deletes the outer array
Graph::~Graph(){
        for (int i = 0; i < vertices; i++){
                delete [] edgeArray[i];
        }
        delete [] edgeArray;
}

//Read a graph from a file
//loops through the file line by line grabbing pertinent information and writing to variables 
//uses a stringstream to convert text to integer/double values
void Graph::readFromFile(std::string file){
        ifstream infile(file);
	if(!infile.is_open()){
		cerr << "Invalid Filename/File Error" << endl;
	}
        string line, word;
        int numVert;
        Type t = UNDIRECTED;
	nodes.push_back({1, WHITE});
        getline(infile, line);
        if(line == "undirected"){
                t = UNDIRECTED;
        }
        else if(line == "directed"){
                t = DIRECTED;
        }       
        direction = t;
        getline(infile, line);
        stringstream aa(line);
        aa >> numVert;
        for(int i = 0 ; i < numVert - 1 ; ++i){
                this->addVertex();
        }
        getline(infile, line);
        stringstream bb(line);
        bb >> numEdges;
        while(getline(infile, line)){
                stringstream ss(line);
                int v1, v2;
                double weight;
                ss >> v1;       
                ss >> v2;
                ss >> weight;
                this->addEdge(v1, v2, weight);
        }
}

//Write a graph to a file
void Graph::writeToFile(std::string file){
        ofstream outfile(file);
	if(!outfile.is_open()){
		cerr << "Invalid Filename/File Error" << endl;
		return;
	}
        if(direction == UNDIRECTED){
                outfile << "undirected" << endl;
        }
        else{
                outfile << "directed" << endl;
        }
        outfile << vertices << endl;
        outfile << numEdges << endl;
        for( int i = 0; i < vertices ; ++i){ 
                for(int j = 0; j < vertices; ++j){
                        if(edgeArray[i][j] != -1){
                                outfile << i + 1 << " " << j + 1 << " " << edgeArray[i][j] << endl;
                        }
                }
        }
        outfile.close();
}

//Empty
//Checks if the vector containing our vertices is empty
bool Graph::empty(){
	return nodes.empty();
}

//Add edge
//increments the location in the matrix to the passed weight
//if graph is undirected, mirrors the value. Also adds edges to the edge list used to solve the MST
void Graph::addEdge(int v1, int v2, double weight){
	if(v1 <= vertices && v2 <= vertices && v1 > -1 && v2 > -1){
		if(direction == DIRECTED){
			edgeArray[v1 - 1][v2 - 1] = weight;
        	}
		else{
			edgeArray[v1-1][v2-1] = weight;
			edgeArray[v2-1][v1-1] = weight;
		}
	}
	E.push({v1,v2,weight});
}
//Add vertex
//Also updates the adjacency matrix to expand the usable space for edges. Initializes vertices to default values
void Graph::addVertex(){
        nodes.push_back({vertices + 1, WHITE, -1, NEUTRAL});
        double **tempArray = new double*[vertices + 1];
        for (int i = 0; i < vertices; i++){
                tempArray[i] = new double[vertices + 1];
                for (int j = 0; j < vertices; j++){
                        tempArray[i][j] = edgeArray[i][j];
                }
                tempArray[i][vertices] = -1;
        }
        tempArray[vertices] = new double[vertices + 1];
        for (int j = 0; j < vertices + 1; j++){
                tempArray[vertices][j] = -1;
        }
        for (int i = 0; i < vertices; i++){
                delete [] edgeArray[i];
        }
        delete [] edgeArray;
        edgeArray = tempArray;
        vertices++;
}
//Helper function for the numConnectedComponents and Tree check
//implements a Breadth First Traversal, from a passed source node, updating a referenced multiset for each vertex that is seen
void Graph::BFTHelp(multiset<int> &seen, int source){
        queue<Node> nodeQueue;
        Node s = nodes[source];
	nodeQueue.push(s);
        while(!nodeQueue.empty()){
                Node curr = nodeQueue.front();
                curr.color = BLACK;
                seen.insert(curr.number-1);
		for(int i = 0; i < vertices; ++i){
                        if((edgeArray[curr.number - 1][i] != -1 || edgeArray[i][curr.number-1] != -1) && nodes[i].color == WHITE){
                                nodes[i].color = GREY;
                                nodeQueue.push(nodes[i]);
			}
                }
                nodeQueue.pop();
        }
	for(int i = 0; i < vertices; ++i){
		nodes[i].color = WHITE;
	}
}
//Count connected components
//Maintains a set of seen vertices, runs a BFT on any Node that hasn't been seen yet. 
//The number of times the BFT needs to be run indicates the number of components
int Graph::numConnectedComponents(){
	multiset<int> seen;
	int num = 0;		
	for(int i = 1; i < vertices ; ++i){
		if(seen.count(i-1) == 0){
			BFTHelp(seen, i);
			++num;
		}
	}
	return num;
}
//Tree check
//Runs the helper function for the Num Connected Components. The BFT search starts from a random node
//Attempts to see if it can reach every other vertex once, with no cycles, as well as checks the number of edges
//BFTHelp is written to check both halves of the matrix, allowing tree() to work on Directed Graphs
bool Graph::tree(){
	multiset<int> seen;
	srand(time(NULL));
	int x = rand() % vertices;	
	BFTHelp(seen, x);
	int nCount = 0;
	for(int i = 0 ; i<vertices; ++i){
		if(seen.count(i) != 1){		
			return false;
		}
		else{
			++nCount;
		}
	}
	if(nCount == vertices && vertices == numEdges+1){
		return true;
	}
	return false;	
}
//Helper function for the depth first traversal
//Implements the recursive call
void Graph::DFTHelper(Node &s, ofstream &os){
        s.color = GREY;
	for(int i = 0 ; i < vertices ; ++i){
                if(edgeArray[s.number-1][i] != -1 && nodes[i].color == WHITE){
			DFTHelper(nodes[i], os);
		}
        }

	s.color = BLACK;
	os << s.number << endl;
}
//Depth First traversal algorithm, recursively visits every child of a vertex, writes to passed file
void Graph::DFT(int source, string file){
	ofstream outfile(file);
	if(!outfile.is_open()){
		cerr << "Invalid File" << endl;
		return;
	}
	DFTHelper(nodes[source-1], outfile);
	outfile.close();
	for(int i = 0; i < vertices; ++i){
                nodes[i].color = WHITE;
        }

}
//Breadth First Traverse - proceed from source
//BFT algorithm, loops through graph and writes to passed file the order of the traversal
void Graph::BFT(int source, string file){
	int nCount = 0;	
	queue<Node> nodeQueue;
	Node s = nodes[source-1];
	ofstream outfile(file);
	if(!outfile.is_open()){
		cerr << "Invalid File" << endl;
		return;
	}
	nodeQueue.push(s);
	++nCount;
	while(!nodeQueue.empty()){
		Node curr = nodeQueue.front();
		curr.color = BLACK;
		++nCount;
		outfile << curr.number << endl;
		for(int i = 0; i < vertices; ++i){
			if(edgeArray[curr.number - 1][i] != -1 && nodes[i].color == WHITE){
				nodes[i].color = GREY;
				nodeQueue.push(nodes[i]);
			}
		}
		nodeQueue.pop();
		if(nCount == vertices-1){
			break;
		}
	}
	outfile.close();
	for(int i = 0; i < vertices; ++i){
                nodes[i].color = WHITE;
        }

}
//Helper function for the Closeness algorithm, calls a depth first traversal, updating the distance value for each Node struct encountered
void Graph::DFTHelp(int source, int dest){
	Node s = nodes[source];
        s.color = GREY;
        for(int i = 0 ; i < vertices ; ++i){
                if(edgeArray[s.number-1][i] != -1 && nodes[i].color == WHITE){
                        if(nodes[i].distance == -1 || nodes[i].distance > s.distance+1){
				nodes[i].distance = s.distance+1;
				int src = i;
				DFTHelp(src, dest);
			}
                }
        }
        s.color = BLACK;
}
//Closeness - determine minimum number of edges to get from one node to the other
//Runs a DFT to determine the minimum path between vertices. In the case of a directed graph, the function is run twice
//Once with v1 as the source, again with v2 as the source, finding the minimum path between the two
int Graph::closeness(int v1, int v2){
	if(v1 > vertices || v2 > vertices || v1 < 0 || v2 < 0){
		cerr << "Invalid vertices: Too large/below zero" << endl;
		return 0;
	}
	for(int i = 0 ; i < vertices ; ++i){
		nodes[i].distance = -1;	
	}
	nodes[v1-1].distance = 0;
	DFTHelp(v1-1, v2);
	int dist1 = nodes[v2-1].distance;
	if(direction == UNDIRECTED){
		return dist1;
	}
	for(int i = 0; i<vertices; ++i){
		nodes[i].distance = -1;
	}
	DFTHelp(v2-1, v1);
	int dist2 = nodes[v1-1].distance;
	for(int i = 0; i<vertices; ++i){
		nodes[i].color = WHITE;
	}
//this code block determines which value to be returned by comparing the two
	if(dist1 != dist2){
		if(dist1 !=-1 && dist2 != -1){
			return dist1<dist2 ? dist1 : dist2;
		}
		else{
			return dist1 != -1 ? dist1 : dist2;	
		}
	}
	else{
		return dist1;
	}
}
//Helper function for the Partitioniable method
//Colors all adjacent nodes the opposite color of their parent using a Breadth first traversal and updating the passed multiset
//returns false if a child would share a color with its parent, returns true otherwise
bool Graph::PartHelp(multiset<int> &seen, int source){
        queue<Node> nodeQueue;
        Node s = nodes[source];
        s.p = RED;
        nodeQueue.push(s);
        while(!nodeQueue.empty()){
                Node curr = nodeQueue.front();
                seen.insert(curr.number-1);
                for(int i = 0; i < vertices; ++i){
                        if((edgeArray[curr.number - 1][i] != -1 || edgeArray[i][curr.number-1] != -1) && nodes[i].p == curr.p){
                                return false;
                        }
                        if((edgeArray[curr.number - 1][i] != -1 || edgeArray[i][curr.number-1] != -1) && nodes[i].p == NEUTRAL){
                                curr.p == RED ? nodes[i].p = BLUE : nodes[i].p = RED;
                                nodeQueue.push(nodes[i]);
                        }
                }
                nodeQueue.pop();
        }
        return true;
}
//Function that determines if it is possible to partition the graph
//Calls the helper function for each component of the graph, using a multiset to maintain a list of seen nodes
bool Graph::partitionable(){
        multiset<int> seen;
        bool b = true;
        for(int i = 0; i < vertices ; ++i){
                if(seen.count(i) == 0){
                        b = PartHelp(seen, i);
                        if(!b){return b;}
                }
        }
	for(int i = 0 ; i < vertices ; ++i){
		nodes[i].p = NEUTRAL;
	}
        return b;
}
//Helper function for the displaying of information in the Minimum spanning tree
//It is required to maintain a list of the components in each graph, this function is used to maintain such a list
//stored in a vector of lists, this function returns the specific list 
list<int> Graph::MSTHelp(multiset<int> &seen, int source){
        queue<Node> nodeQueue;
        Node s = nodes[source];
        nodeQueue.push(s);
	list<int> l;
        while(!nodeQueue.empty()){
                Node curr = nodeQueue.front();
                curr.color = BLACK;
		l.push_back(curr.number-1);
                seen.insert(curr.number-1);
                for(int i = 0; i < vertices; ++i){
                        if((edgeArray[curr.number - 1][i] != -1 || edgeArray[i][curr.number-1] != -1) && nodes[i].color == WHITE){
                                nodes[i].color = GREY;
                                nodeQueue.push(nodes[i]);
                        }
                }
                nodeQueue.pop();
        }
        for(int i = 0; i < vertices; ++i){
                nodes[i].color = WHITE;
        }
	return l;
}
//* MST - print the minimum spanning tree of the graph to a file with the passed name (return whether or not this operation was successful)
//utilizes Kruskal's algorithm with a priority queue of edges, sorted by their weight
//Pops off the edge of least weight each time, updating both a set of vertices called seen, as well as a queue of edges forming the tree, called T
//To check for cycles, checks if both vertices of an added edge have been seen, throwing out edges that satisfy this condition
//After the MST has been assembled into the queue, runs MSTHelp to determine where the components are which vertices fit where
//The lists can then be sorted and made unique, which greatly simplifies the process of formatting output
bool Graph::MST(string file){
	ofstream outfile(file);
	if(!outfile.is_open()){
		cerr << "Invalid File" << endl;
		return false;
	}
	set<int> seen;
	multiset<int> viewed;
	vector<list<int>> components;
	queue<edge> T;
	while(!E.empty() && (int)T.size() != vertices-1){
		if(seen.count(E.top().v1) >= 1 && seen.count(E.top().v2) >= 1){
			E.pop();
		}
		else{
			seen.insert(E.top().v1);
			seen.insert(E.top().v2);
			T.push(E.top());	
			E.pop();
		}
	}
	for(int i = 0; i < vertices; ++i){
		if(viewed.count(i) == 0){
			list<int> l = MSTHelp(viewed, i);
			components.push_back(l);
		}
	}	
	while(!components.empty()){
		outfile << "{ {";
		list<int> l1 = components.front();
		l1.sort();
		l1.unique();
		for(auto iter = l1.begin(); iter!=l1.end() ; ++iter){
			if(distance(iter, l1.end()) == 1){
				outfile << *iter+1 << "}, { ";
			}
			else{
				outfile << *iter+1 << ", ";
			}
		}
		int num = l1.size() - 1;
		while(num > 0){
			if(num - 1 == 0){
				outfile << "(" << T.front().v1 << ", " << T.front().v2 << ", " << T.front().weight << ") ";
			}
			else{
				outfile << "(" << T.front().v1 << ", " << T.front().v2 << ", " << T.front().weight << "), ";
			}
			T.pop();
			num--;
		}
		outfile <<  "} }" << endl;	
		auto iter2 = components.begin();
		components.erase(iter2);
	}
	outfile.close();
	return true;
}
//* Step Away - print the nodes who are a degree of closeness from the source to a file with the passed name
//repeatedly calls Closeness on the source, adding values that fit the passed closeness to the output
//The name of closeness was changed to prevent confusion
void Graph::stepAway(int source, int close, string file){
	ofstream outfile(file);
	if(!outfile.is_open()){
		cerr << "Invalid File" << endl;
		return;
	}
	for(int i = 0; i < vertices ; ++i){
		int k = closeness(source, i+1);
		if( k == close ){
			outfile << i+1 << endl;
		}
	}
}

