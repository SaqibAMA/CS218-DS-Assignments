#include <iostream>						// For IO
#include <memory>						// For shared_ptr
#include <fstream>						// For file handling
#include <string>						// For string
#include <vector>						// For dynamic arrays
#include <iomanip>						// For IO Manipulation

using namespace std;

// Linked List Class
template <class t>
class List {

public:

	// Node class.
	struct Node {

		shared_ptr <Node> prev;
		shared_ptr <Node> next;
		t data;

		friend class List;

		Node() {
			prev = next = nullptr;
		}

		Node(t data) {
			this->data = data;
			prev = next = nullptr;
		}

		~Node() {
			// Not needed.
		}

	};


	// Iterator class.
	class Iterator {

	public:
		Node* curr;

	public:
		// Constructor
		Iterator(Node* curr = nullptr) {
			this->curr = curr;
		}

		Iterator(shared_ptr <Node> curr) {
			this->curr = curr.get();
		}

		// Copy Constructor
		Iterator(const Iterator& I) {
			this->curr = I.curr;
		}

		// Operators
		void operator ++ () {
			this->curr = this->curr->next.get();
		}

		void operator ++ (int) {
			this->curr = this->curr->next.get();
		}

		void operator -- () {
			this->curr = this->curr->prev.get();
		}

		void operator -- (int) {
			this->curr = this->curr->prev.get();
		}

		Iterator& operator = (const Iterator& I) {
			this->curr = I.curr;
		}

		bool operator == (const Iterator& I) const {
			return this->curr == I.curr;
		}

		bool operator != (const Iterator& I) const {
			return this->curr != I.curr;
		}

		t& operator * () {
			return this->curr->data;
		}


	};

private:
	shared_ptr <Node> head;					// head pointer.
	shared_ptr <Node> tail;					// tail pointer.
	size_t size;							// size.

public:

	// Constructor.
	List() {

		head = make_shared <Node>();		// dummy head.
		tail = make_shared <Node>();		// dummy tail.

		head->next = tail;
		tail->prev = head;

		size = 0;

	}

	// Copy constructor.
	List(const List& L) {

		head = make_shared <Node>();
		tail = make_shared <Node>();

		head->next = tail;
		tail->prev = head;

		size = 0;

		for (auto i : L)
			this->push_back(i);

	}

	// Assignment operator.
	List& operator = (const List& L) {

		while (!this->isEmpty()) {
			this->pop_back();
		}

		for (auto x : L)
			this->push_back(x);

		return *this;

	}

	// Checking empty.
	bool isEmpty() const {
		return (size == 0);
	}

	// Append at the end.
	void push_back(t data) {

		shared_ptr <Node> newData = make_shared <Node>();

		newData->data = data;
		newData->next = tail;
		newData->prev = tail->prev;

		tail->prev->next = newData;
		tail->prev = newData;

		size++;

	}

	// Remove from back.
	void pop_back() {

		if (size) {

			tail->prev->prev->next = tail;
			tail->prev = tail->prev->prev;

			size--;

		}

	}

	// Append at front.
	void push_front(t data) {

		shared_ptr <Node> newData = make_shared <Node>();

		newData->data = data;
		newData->next = head->next;
		newData->prev = head;

		head->next = newData;
		newData->next->prev = newData;

		size++;

	}

	// Remove at front.
	void pop_front() {

		if (size) {

			head->next = head->next->next;
			head->next->next->prev = head;

			size--;

		}

	}

	// Check the front element.
	t& peekFront() const {

		return head->next->data;

	}

	// Printing list.
	void print() const {

		if (size) {

			cout << endl;
			for (auto x : *this)
				cout << x << " " << endl;
			cout << endl;

		}

	}

	// Check if an element exists.
	bool has(const t& data) const {

		for (auto x : *this)
			if (data == x) return true;

	}

	// Getting size.
	size_t getSize() const {
		return size;
	}

	// Get a specific element.
	Iterator& get(const t& data) {

		for (auto i = begin(); i != end(); ++i)
			if (*i == data) return i;

		Iterator it = end();
		return it;

	}

	// Get begin.
	Iterator begin() const {
		Iterator i(head->next);
		return i;
	}

	// Get end.
	Iterator end() const {
		Iterator i(tail);
		return i;
	}

	// Destructor.
	~List() {
		// Not needed.
	}


};

// Computer Class
class Computer {

public:
	unsigned int ID;			// To keep ID.
	string location;			// To keep location.
	int weight;

	unsigned int originID;		// Keeps the parent ID


	// Constructor
	Computer(unsigned int ID = 0, string location = "", int weight = 0, unsigned int originID = 0) {

		this->ID = ID;
		this->location = location;
		this->weight = weight;
		this->originID = originID;
	
	}

	Computer(const Computer& C) {
		
		this->ID = C.ID;
		this->location = C.location;
		this->weight = C.weight;
		this->originID = C.originID;

	}

	// operators
	bool operator == (const Computer& C) {

		return (this->ID == C.ID);

	}

	bool operator != (const Computer& C) {

		return (*this == C);
	
	}

	Computer& operator = (const Computer& C) {
		
		this->ID = C.ID;
		this->location = C.location;
		this->weight = C.weight;
		this->originID = C.originID;

		return *this;
	}

	friend ostream& operator << (ostream& out, const Computer& C) {

		const int boxSize = 10;

		out <<  " ----------" << endl;
		out << "|" << setw(boxSize) << C.ID << "|" << endl;
		out << "|" << setw(boxSize) << C.location << "|" << endl;
		out << "|" << setw(boxSize) << C.weight << "|" << endl;
		out << " ----------" << endl;

		return out;

	}

	~Computer() {
		// Not needed.
	}

};




// Graph Class
class Graph {

private:
	unsigned int v;					// total vertices
	List <Computer>* adj;			// adjacency list

public:
	
	// For Private Access
	friend class NetworkOptimiser;

	// Constructor
	Graph() {

		v = 0;								// default vertices
		adj = nullptr;						// default adjacency list

	}

	// Allocates memory for the graph and sets size.
	void allocateMemoryAndSetSize(unsigned int v) {

		this->v = v;							// setting size
		adj = new List <Computer> [v];			// allocating list

	}

	// Reading data network data from the file.
	void readNodesFromFile(string filename) {

		ifstream fin;								// nodes file
		fin.open(filename);

		if (!fin) {
			cout << "Couldn't open file!";
			return;
		}

		unsigned int buffer;						// buffer to read data.
		char charBuffer[100];

		fin >> buffer;								// reading total vertices.
		
		this->allocateMemoryAndSetSize(buffer);		// allocating memory for
													// data.

		for (int i = 0; i < this->v; i++) {

			fin >> buffer;								// reading ID

			fin.ignore();								// ignoring , and space
			
			fin.getline(charBuffer, 100, '\n');

			Computer newEntry(buffer, charBuffer, 0);	// default weight 0

			adj[i].push_back(newEntry);

		}

		int edges;
		fin >> edges;

		for (int i = 0; i < edges; i++) {

			int v1ID;
			fin >> v1ID;							// reading vertex 1 ID

			fin.ignore();							// ignoring comma

			int v2ID;
			fin >> v2ID;							// reading vertex 2 ID

			int v12Weight;
			fin >> v12Weight;						// reading weight

			auto vertex = adj[v2ID - 1].begin();	// getting the vertex name

			Computer newEdge(v2ID, vertex.curr->data.location, v12Weight, v1ID);	// creating the new vertex

			adj[v1ID - 1].push_back(newEdge);		// -1 deviation to accomodate
													// for IDs.

			auto parentVertex = adj[v1ID - 1].begin();

			Computer parentEdge(v1ID, parentVertex.curr->data.location, v12Weight, v2ID);

			adj[v2ID - 1].push_back(parentEdge);

		}


		cout << "Original Adjacency List: " << endl;
		this->printAdjacencyList();


		fin.close();

	}

	// Adjacency List Printer
	void printAdjacencyList() const {

		for (int i = 0; i < v; i++) {

			for (auto j : adj[i])
				cout << "(" << "or: " << j.originID << ", " << j.ID << ", " << j.weight << ") -> ";
			cout << "\b\b\b" << "   " << std::flush;
			cout << endl;

		}

	}

	void addVertex(const Computer& C) {

		this->v = v + 1;
		List <Computer>* newList = new List <Computer> [v];

		for (int i = 0; i < v - 1; i++) {
			newList[i] = adj[i];
		}

		newList[v - 1].push_back(C);

		delete[] adj;
		adj = newList;

	}

	// Destructor
	~Graph() {
		if (adj)
			delete[] adj;

		adj = nullptr;
	}


};


// MIN_HEAP class

class MIN_HEAP {

private:
	vector <Computer*> heap;						// stores the array

public:

	// Constructor
	MIN_HEAP(unsigned int v = 0) {

		heap.resize(v);

	}

	// Adding data
	void addData(Computer** vertices, unsigned int size) {


		for (unsigned int i = 0; i < size; i++) {

			heap[i] = vertices[i];					// copying the pointers into the vector.

		}

	}

	// Function to calculate parent of any node.
	int parent(int n) const {

		int parentIndex = (n - 1) / 2;

		if (parentIndex >= 0)
			return parentIndex;
		
		return 0;

	}

	// Function to generate minheap in O(n).
	void generateMinHeap() {

		// Start from the bottom.

		for (int i = heap.size() - 1; i >= 0; i--) {

			int currentNode = i;


			// Swap the element upwards until it has reached its position.

			while (heap[currentNode]->weight < heap[parent(currentNode)]->weight) {

				swap(heap[currentNode], heap[parent(currentNode)]);
				currentNode = parent(currentNode);

			}

		}

	}

	// Reset Heap
	void resetHeap() {

		for (int i = 0; i < heap.size(); i++)		// resetting weights.
			heap[i]->weight = INT_MAX;

	}

	// Function to extract the minimum from top.
	Computer extractMinimum() {

		Computer minimum = *(heap.front());			// keeping a copy.
		heap.erase(heap.begin());					// removing the top elem.

		generateMinHeap();							// remaking the minheap.

		return minimum;								// returning min.

	}

	// Peak
	void peak(ofstream& fin) const {

		for (auto i : heap)
			fin << "(o: " << i->originID << ", " << i->ID << ", " << i->weight << ") ->";
		fin << "\b\b\b" << "   " << std::flush;
		fin << endl;

	}

	// Check if heap is empty.
	bool empty() const {

		return heap.empty();

	}

};


class NetworkOptimiser {

private:
	Graph network;								// computer network.

public:
	NetworkOptimiser() {

		while (true) {

			system("cls");
			printMenu();

			int choice = 0;
			cout << ">> ";
			cin >> choice;

			if (choice == 1) {

				// Input a graph from file.
				this->loadGraph();

				cin.get();
				cin.get();

			}
			else if (choice == 2) {

				// Save graph in file.
				this->saveGraph();
				cout << "Saved!" << endl;

				cin.get();
				cin.get();

			}
			else if (choice == 3) {

				// Add a vertex to the graph.
				
				Computer newVertex;

				cout << "Enter location: ";
				cin >> newVertex.location;

				newVertex.ID = network.v + 1;

				network.addVertex(newVertex);

			}
			else if (choice == 4) {

				// Getting details

				int v1ID, v2ID, weight;
				cout << "Enter origin vertex: ";
				cin >> v1ID;
				cout << "Enter second vertex: ";
				cin >> v2ID;
				cout << "Enter edge weight: ";
				cin >> weight;

				// Appending edge 

				{
					
					Computer newEdge(network.adj[v2ID - 1].begin().curr->data);
					newEdge.weight = weight;
					newEdge.originID = v1ID;

					network.adj[v1ID - 1].push_back(newEdge);

				}
				{

					Computer newEdge(network.adj[v1ID - 1].begin().curr->data);
					newEdge.weight = weight;
					newEdge.originID = v2ID;

					network.adj[v2ID - 1].push_back(newEdge);
				
				}




			}
			else if (choice == 5) {


				Graph* optimalGraph = this->optimizeNetwork();

				cout << "\nOptimal Graph: " << endl;
				optimalGraph->printAdjacencyList();

				cin.get();
				cin.get();


			}
			else if (choice == 6) {

				// Printing graph
				cout << "Adjacency List: " << endl;
				network.printAdjacencyList();

				cin.get();
				cin.get();

			}

		}

		

	}
	
	// Loads the graph data from file.
	void loadGraph() {

		string filename;

		cout << "Enter filename: ";								// Taking filename.
		cin >> filename;

		network.readNodesFromFile(filename);					// Loading the data in graph.

	}

	// Save graph
	void saveGraph() {

		string filename;

		cout << "Enter filename: ";
		cin >> filename;

		ofstream fout(filename);


		fout << network.v << endl;

		for (int i = 0; i < network.v; i++) {

			fout << network.adj[i].begin().curr->data.ID << ", ";
			fout << network.adj[i].begin().curr->data.location << endl;

		}



		fout.close();

	}

	// Optimizes the graph and returns it.
	Graph* optimizeNetwork() {

		Computer** vertices = new Computer*[network.v];			// We keep a copy of original vertices
																// here for our minheap to operate.

																// This allows us to have O(1) access times.


		for (int i = 0; i < network.v; i++) {

			vertices[i] = new Computer;
			vertices[i][0] = network.adj[i].begin().curr->data;	// Creating a deep copy of adjacency list vertices.
			vertices[i]->weight = INT_MAX;
		
		}

		MIN_HEAP minheap(network.v);							// Creating a minheap object

		minheap.addData(vertices, network.v);					// This just adds the pointers. It doesn't perform anything.
		minheap.generateMinHeap();								// Generating the minheap.

		vector <bool> visited(network.v);						// keeps a track of visited nodes.

		Graph* optimalGraph = new Graph;						// Creating our optimal graph.
		optimalGraph->allocateMemoryAndSetSize(network.v);

																// Loading all the primary vertices in the graph.
		for (int i = 0; i < network.v; i++) {

			Computer primaryVertex(network.adj[i].begin().curr->data);
			optimalGraph->adj[i].push_back(primaryVertex);

		}


		// Initializing starting point
		const int startingPoint = 1;
		vertices[startingPoint - 1][0].weight = 0;
		vertices[startingPoint - 1][0].originID = startingPoint;
		visited[startingPoint - 1] = true;


		cout << "\n\n(FROM, TO, WEIGHT)" << endl;
		unsigned int totalWeight = 0;

		ofstream fin("debug.txt");

		while (!minheap.empty()) {

		
			minheap.generateMinHeap();							// generating minheap.
			
			minheap.peak(fin);

			
			Computer minimum = minheap.extractMinimum();		// minimum weighted node.
			// minheap.resetHeap();								// resetting weights.

			if (minimum.originID != minimum.ID) {
				cout << "(" << minimum.originID << ", " << minimum.ID << ", " << minimum.weight << ")" << endl;
				totalWeight += minimum.weight;
			}


			for (auto i : network.adj[minimum.ID - 1]) {

				if (!visited[i.ID - 1]) {

					if (i.weight <= vertices[i.ID - 1][0].weight) {
						vertices[i.ID - 1][0].weight = i.weight;			// assigning weights.
						vertices[i.ID - 1][0].originID = minimum.ID;		// setting parent ID.
					}

				}

			}



			if (minimum.originID != minimum.ID) {

				optimalGraph->adj[minimum.originID - 1].push_back(minimum);

				Computer originVertex(optimalGraph->adj[minimum.originID - 1].begin().curr->data);
				originVertex.weight = minimum.weight;

				optimalGraph->adj[minimum.ID - 1].push_back(originVertex);

			}

		}

		// Debugging file
		fin.close();

		// Total weight
		cout << "Total distance: " << totalWeight << endl;

		// Deallocating our vertices
		for (int i = 0; i < network.v; i++) {
			delete vertices[i];
		}

		delete[] vertices;

		return optimalGraph;


	}


	// UI Stuff
	void printMenu() const {

		cout << "+-----------------------------------+" << endl;
		cout << "+ 1. Load graph from file.          +" << endl;
		cout << "+ 2. Save graph in file.            +" << endl;
		cout << "+ 3. Add a vertex in graph.         +" << endl;
		cout << "+ 4. Add a edge in graph.           +" << endl;
		cout << "+ 5. Optimize graph.                +" << endl;
		cout << "+ 6. Print existing graph.          +" << endl;
		cout << "+-----------------------------------+" << endl;

	}



};

int main() {
	
	NetworkOptimiser n;


	return 0;
}