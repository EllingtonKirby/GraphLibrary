#include "Graph.h"
#include <chrono>
using namespace std;
int main(){
	chrono::monotonic_clock::time_point start, finish;
	chrono::microseconds elapsed;
	Graph myGraph(UNDIRECTED);
	string file;
	cout << "Enter the filename: " << endl;
	cin >> file;	
	myGraph.readFromFile(file);
	myGraph.writeToFile("outTest.txt");
	myGraph.BFT(2, "BFT.txt");
	myGraph.DFT(1, "DFT.txt");
	cout << "Connect: " << myGraph.numConnectedComponents() << endl;	
	if(myGraph.tree()){
		cout << "is a tree" << endl;
	}
	else{ cout << "not a tree" << endl;}
	cout << "1-4: " << myGraph.closeness(1,4) << endl;
	cout << "2-6: " << myGraph.closeness(2,6) << endl;
	cout << "4-5: " << myGraph.closeness(4,5) << endl;
	cout << "2-2: " << myGraph.closeness(2,2) << endl;
	myGraph.stepAway(2, -1, "sa.g1.2.-1.txt");
	myGraph.stepAway(1, 2, "sa.g1.1.2.txt");
	myGraph.stepAway(2, 1, "sa.g1.2.1.txt");
	myGraph.stepAway(4, -1, "sa.g2.4.-1.txt");	
	start = chrono::monotonic_clock::now();
	myGraph.MST("mst.g1.txt");
	cout <<  myGraph.partitionable() << endl;
	finish = chrono::monotonic_clock::now();
	elapsed = chrono::duration_cast<chrono::microseconds>(finish-start);
	cout << elapsed.count() << " microseconds" << endl;
	Graph newGraph(DIRECTED);
	newGraph.empty() ? cout << "is Empty!" << endl : cout << "is not empty!" << endl;
	return 0;
}
