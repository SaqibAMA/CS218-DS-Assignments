#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


// --------- GRAPHICS RELATED ----------

#define GAP 5

#include <Windows.h>

// Handle for all graphics functionality.
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

// This function allows to move the cursor to any (x, y)

void gotoxy(int x, int y) {

	COORD scrn;
	scrn.X = x;
	scrn.Y = y;

	SetConsoleCursorPosition(h, scrn);

}

// --------- GRAPHICS RELATED ----------



// Doc Info
/*

This class keeps a track of documents
and their frequency

*/
class DocInfo {

private:
	unsigned int docId;		// Document ID
	unsigned int freq;		// Keeps frequency of a certain word.
	string name;

public:

	// Friendships for private access.
	friend class Rank;
	friend class SearchEngine;
	friend class TermInfo;

	// Constructors

	DocInfo(const DocInfo& D) {
		docId = D.docId;
		freq = D.freq;
		name = D.name;
	}

	// Acts as default constructor too.

	DocInfo(unsigned int docId = 0, unsigned int freq = 0, string name = "") {
		this->docId = docId;
		this->freq = freq;
		this->name = name;
	}

	// Frequency manipulation.

	void incrementFrequency(int step = 1) {
		this->freq += step;
	}
	void resetFrequency() {
		freq = 0;
	}

	// Operators
	friend ostream& operator << (ostream& out, const DocInfo& D) {
		out << "\n[DocName:" << D.name << ", DocID: " << D.docId << ", Freq: " << D.freq << "] ";
		return out;
	}
	bool operator == (const DocInfo& D) {
		return (this->docId == D.docId);
	}

	~DocInfo() {

		docId = freq = 0;
		name = "";

	}

};


// Term Info
/*

A class to keep a track of all
the terms and associates docs.

*/

class TermInfo {

public:

	// Friendships for private access.
	friend class SearchEngine;

	// Constructors
	TermInfo() {
		term = "";
	}
	TermInfo(string term) {
		this->term = term;
	}
	TermInfo(const char* term) {
		this->term = term;
	}

	// Member functions
	void addDoc(const DocInfo& D) {
		DI.push_back(D);
	}

	bool hasDoc(int docId) {

		for (auto x : DI) {
			if (x.docId == docId) return true;
		}

		return false;

	}

	void incrementDocFreq(int docId) {

		for (int i = 0; i < DI.size(); i++) {
			if (DI[i].docId == docId) {
				++DI[i].freq;
			}
		}

	}

	// Operators
	friend ostream& operator << (ostream& out, const TermInfo& T) {
		out << T.term << " -> ";
		for (auto x : T.DI) cout << x;
		return out;
	}
	void operator = (const TermInfo& T) {
		this->term = T.term;
		for (auto i = T.DI.begin(); i != T.DI.end(); i++)
			DI.push_back(*i);
	}
	bool operator == (const TermInfo& T) {

		return (this->term == T.term);

	}

	bool operator < (const TermInfo& T) {

		return (term < T.term);

	}

	bool operator > (const TermInfo& T) {
		return (term > T.term);
	}

	~TermInfo() {
		DI.resize(0);
		term = "";
	}

private:
	string term;			// Keeps the actual word.
	vector <DocInfo> DI;	// Keeps the list of documents.

};


// Rank Class
/*

This class stores the information
regarding a certain rank of the listing.

*/
class Rank {
public:
	unsigned int docId;				// Keeps the document ID.	
	unsigned int collectiveFreq;	// Keeps sum(keyword * freq).
	unsigned int keywords;			// Keeps the total keywords in the document.

public:

	// Constructors
	Rank() {
		docId = collectiveFreq = keywords = 0;
	}

	Rank(const DocInfo& D) {

		docId = D.docId;
		collectiveFreq = D.freq;
		keywords = 1;

	}

	// Operators

	bool operator == (const Rank& R) const {

		return (docId == R.docId);

	}

	bool operator != (const Rank& R) const {

		return (docId == R.docId);

	}

	friend ostream& operator << (ostream& out, const Rank& R) {

		out << "[ID: " << R.docId << "] " << "[CF: " << R.collectiveFreq << "] " << "[Keywords: " << R.keywords << "]";

		return out;
	}

	~Rank() {
		docId = collectiveFreq = keywords = 0;
	}

};



#define MAX_IMBALANCE 1

// - AVL Tree Class -

template <typename T>
class AVL {

	// AVL Node Class

	struct ANode {

		T data;
		int height;
		ANode* right;
		ANode* left;

		ANode(T data = NULL) {
			right = left = nullptr;
			height = 0;
			this->data = data;
		}

	} *root;

public:

	// Constructor

	AVL() {
		root = nullptr;
	}

	// Insertion
	bool insertNode(T data) {

		if (root == nullptr) {
			root = new ANode(data);
			return true;
		}

		ANode* prev = nullptr;
		ANode* curr = root;

		return insertNode(prev, curr, data);

	}

	// Recursive Sub-Function
	bool insertNode(ANode* prev, ANode* curr, T data) {

		if (curr == nullptr) {

			curr = new ANode(data);

			if (prev) {

				if (curr->data > prev->data) {

					prev->right = curr;

				}
				else if (curr->data < prev->data) {

					prev->left = curr;

				}

			}

			return true;

		}
		else {


			if (data > curr->data) {
				if (insertNode(curr, curr->right, data)) {
					curr->height = heightofBSTRec(curr);
					return true;
				}
			}
			else if (data < curr->data) {
				if (insertNode(curr, curr->left, data)) {
					curr->height = heightofBSTRec(curr);
					return true;
				}
			}
			else {
				return false;
			}

			/*if (!nodeIsBalanced(curr)) {
				
				if (getBalanceFactor(curr->left, curr->right) < 0) {



				}

			}*/


		}

		return false;

	}


	// Checking Imbalance
	bool nodeIsBalanced(ANode* node) const {

		int rightNodeHeight = 0, leftNodeHeight = 0;

		if (node->right) {
			rightNodeHeight = node->right->height;
		}

		if (node->left) {
			leftNodeHeight = node->left->height;
		}

		return !(abs(rightNodeHeight - leftNodeHeight) > MAX_IMBALANCE);

	}

	// Getting Balance Factor
	int getBalanceFactor(ANode* left, ANode* right) {

		int leftHeight = 0, rightHeight = 0;

		if (left) {
			leftHeight = left->height;
		}

		if (right) {
			rightHeight = right->height;
		}

		return (leftHeight - rightHeight);

	}

	// LL Rotation

	void LLRotation(ANode* curr, ANode* prev = nullptr) {

		ANode* prevBData = curr->left->right;

		if (curr == root) {
			root = curr->left;
			curr->left->right = curr;
			curr->left = prevBData;
		}
		else {

			if (prev) {
				prev = curr->left;
			}

			curr->left->right = curr;
			curr->left = prevBData;

		}

	}

	// RR Rotation

	void RRRotation(ANode* curr, ANode* prev = nullptr) {

		ANode* prevBData = curr->right->left;

		if (curr == root) {
			root = curr->right;
			curr->right->left = curr;
			curr->right = prevBData;
		}
		else {

			if (prev) {
				prev = curr->right;
			}

			curr->right->left = curr;
			curr->right = prevBData;

		}

	}

	// LR Rotation

	void LRRotation(ANode* curr, ANode* prev = nullptr) {

		ANode* CLeftTree = curr->left->right->left;

		curr->left->right->left = curr->left;
		curr->left = curr->left->right;
		curr->left->left->right = CLeftTree;

		LLRotation(curr, prev);

	}

	// RL Rotation

	void RLRotation(ANode* curr, ANode* prev = nullptr) {

		
		ANode* CRightTree = curr->right->left->right;

		curr->right->left->right = curr->right;
		curr->right = curr->right->left;
		curr->right->right->left = CRightTree;


		RRRotation(root, prev);

	}


	// Tree Height Calculation.
	int heightofBSTRec(ANode* start) const {

		if (root == nullptr) return -1;

		return max(heightofBSTRecSub(start->left), heightofBSTRecSub(start->right));

	}

	// Tree Height Sub-Function.
	int heightofBSTRecSub(ANode* curr) const {

		if (curr == nullptr) return 0;

		return (1 + max(heightofBSTRecSub(curr->left),
			heightofBSTRecSub(curr->right)));


	}


	// Printing in Tree Form

	// Print Tree Form
	void Printintreeform() {

		// LLRotation(root);
		// RRRotation(root);
		// LRRotation(root);
		// RLRotation(root);

		ANode* curr = root;
		int xAxis = 0;
		int yAxis = 0;

		Printintreeform(curr, xAxis, yAxis);

		gotoxy(20, 20);
		cout << endl;


	}

	void Printintreeform(ANode* curr, int& xAxis, int yAxis) const {

		if (curr == nullptr) {


			xAxis += GAP;

			return;

		}
		else {

			Printintreeform(curr->left, xAxis, yAxis + GAP);
			Printintreeform(curr->right, xAxis, yAxis + GAP);

			gotoxy(xAxis + yAxis, yAxis);
			cout << curr->data << ", " << curr->height << endl;

			if (curr->left) {
				gotoxy(xAxis + yAxis - 2, yAxis + 2);
				cout << "/" << endl;
			}

			if (curr->right) {
				gotoxy(xAxis + yAxis + 2, yAxis + 2);
				cout << "\\" << endl;
			}

		}


	}


	// Simple Inorder Traversal.
	void print() {

		ANode* curr = root;

		print(curr);

	}

	// Sub-function for inorder traversal.
	void print(ANode* curr) const {

		if (curr == nullptr) {

			return;

		}
		else {

			print(curr->left);
			cout << curr->data << endl << endl;
			print(curr->right);
		}


	}


	// Element Exists
	T* search(T key) {

		return search(root, key);

	}

	// Sub-function
	T* search(ANode* curr, T key) {

		if (curr == nullptr) return nullptr;


		if (key == curr->data) {
			return &curr->data;
		}
		else if (key < curr->data) {
			return search(curr->left, key);
		}
		else if (key > curr->data) {
			return search(curr->right, key);
		}

		return nullptr;


	}

	// Destructor
	~AVL() {

		// cleanTree(root);

		if (root) {
			delete root;
			root = nullptr;
		}

	}

	// Helper function for destructor
	void cleanTree(ANode* curr) {

		if (curr == nullptr) return;

		cleanTree(curr->left);
		cleanTree(curr->right);

		delete curr;

	}



};


// Search Engine Class
/*

Holds the entire indexing and searching
functionality.

*/
class SearchEngine {

private:

	// For Graphics
	const int screenTop = 5;		// Margin from top.
	const int screenSide = 15;		// Margin from side.

	unsigned int docsIndexed = 0;	// Total documents indexed.

	AVL <TermInfo> index;

public:
	// Constructor.
	SearchEngine() {
		init();						// Starts the search engine.
	}

	// Initializer function
	void init() {

		/*

		Initializes all the activity and contains the main
		search engine loop.

		This is the entry point of the program.

		*/

		cout << "Kindly go full screen for better experience..." << endl;
		cin.get();

		const int indexSize = 4;	// Keeps the index size.
		const char* fileNames[indexSize] = {
			"files/Doc1.txt",
			"files/Doc2.txt",
			"files/Doc3.txt",
			"files/Doc4.txt"
		};							// Keeps the name of the files to initially index.

		system("cls");

		createIndex(fileNames, indexSize);

		while (true) {

			int querySize = 0;

			char** searchQuery = getSearchQuery(querySize);

			search(searchQuery, querySize);

			free(searchQuery);

			cin.get();
			system("cls");

		}

	}

	// Indexing function
	void createIndex(const char** fileNames, const int indexSize) {

		/*

		This function takes a list of documents and extracts
		unique terms from it.

		It builds an index of terms that can be used for further
		searching and processing.


		*/

		for (int i = 0; i < indexSize; i++) {


			char temp[100];

			ifstream fin;
			fin.open(fileNames[i]);

			DocInfo currentDocument;
			currentDocument.docId = i + 1;
			currentDocument.name = fileNames[i];
			currentDocument.freq = 1;

			while (!fin.eof()) {

				fin >> temp;

				TermInfo newTerm(temp);
				newTerm.addDoc(currentDocument);

				if (!index.search(newTerm)) {

					index.insertNode(newTerm);

				}
				else {

					TermInfo* existingTerm = index.search(newTerm);

					if (existingTerm->hasDoc(i + 1)) {

						existingTerm->incrementDocFreq(i + 1);
					
					}
					else {
					
						existingTerm->addDoc(currentDocument);
					
					}


				}


			}


			fin.close();


		}

		docsIndexed = indexSize;

	}

	// Tokenizer function
	char** getSearchQuery(int& querySize) {

		// Data members for necessary operations.
		// Building on stack because it's efficient.

		const int buffSize = 100;
		char temp[buffSize];

		char copyBuffer[20][buffSize];
		int copyBuffIter = 0;

		temp[0] = 0;

		while (!temp[0]) {

			cout << ">>: ";														// Taking the query input.
			cin.getline(temp, 100, '\n');

		}

		for (unsigned int i = 0; temp[i] != '\0' && i <= strlen(temp); i++) {

			int j = i;
			copyBuffIter = 0;													// Scanning through the whole query
																				// and splitting it into words
			while (temp[j] != '\0' && temp[j] != ' ') {

				copyBuffer[querySize][copyBuffIter] = temp[j];
				++j;
				++copyBuffIter;

			}

			copyBuffer[querySize][copyBuffIter] = '\0';
			++querySize;
			i = j;

		}

		// Allocating memory and copying everything
		// in a dynamic array

		char** tokenizedQuery = new char* [querySize];

		for (int i = 0; i < querySize; i++) {

			for (unsigned int j = 0; j < strlen(copyBuffer[i]); j++) {

				if (copyBuffer[i][j] >= 'A' && copyBuffer[i][j] <= 'Z') {
					copyBuffer[i][j] += 32;
				}

			}

			tokenizedQuery[i] = new char[strlen(copyBuffer[i]) + 1];
			strcpy(tokenizedQuery[i], copyBuffer[i]);

		}

		return tokenizedQuery;

	}

	// Search function
	void search(char** searchQuery, int querySize) {


		vector <TermInfo*> results;


		for (int i = 0; i < querySize; i++) {

			TermInfo word(searchQuery[i]);

			if (index.search(word)) {

				results.push_back(index.search(word));
			
			}

		}

		// createRankings(results);


	}

	// Destructor
	~SearchEngine() {
		// Not needed.
	}

};


int main() {

	SearchEngine s;




	return 0;
}