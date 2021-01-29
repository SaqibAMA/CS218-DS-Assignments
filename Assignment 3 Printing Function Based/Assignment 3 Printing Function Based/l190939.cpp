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
		out << T.term;
		/*for (auto x : T.DI) cout << x;*/
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

	// ---- Additional Code ----

	void print2DUtil(ANode* root, int space = 5) const {
		const int COUNT = 10;
		// Base case
		if (root == NULL)
			return;

		// Increase distance between levels
		space += COUNT;

		// Process right child first
		print2DUtil(root->right, space);

		// Print current node after space
		// count
		cout << endl;
		for (int i = COUNT; i < space; i++)
			cout << " ";
		cout << root->data << "\n";

		// Process left child
		print2DUtil(root->left, space);

	}

	ANode* getRoot() const {
		return root;
	}




	// Constructor

	AVL() {
		root = nullptr;
	}

	// Copy Constructor

	AVL(const AVL& A) {

		if (A.root == nullptr) {

			this->root = nullptr;

		}
		else {

			copyTree(A.root);

		}

	}

	// Tree Copying Function

	void copyTree(ANode* curr) {

		if (curr == nullptr) {
			return;
		}
		else {
			this->insertNode(curr->data);
			copyTree(curr->right);
			copyTree(curr->left);
		}

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
				}
			}
			else if (data < curr->data) {
				if (insertNode(curr, curr->left, data)) {
					curr->height = heightofBSTRec(curr);
				}
			}
			else {
				return false;
			}

			if (!nodeIsBalanced(curr)) {

				if (data > curr->data) {

					if (data > curr->right->data) {
						RRRotation(curr, prev);
					}
					else {
						RLRotation(curr, prev);
					}

				}
				else {

					if (data < curr->left->data) {
						LLRotation(curr, prev);
					}
					else {
						LRRotation(curr, prev);
					}

				}

			}
			else {
				return true;
			}


		}

		return false;

	}


	// Checking Imbalance
	bool nodeIsBalanced(ANode* node) const {

		int rightNodeHeight = 0, leftNodeHeight = 0;

		if (node->right) {
			node->right->height = heightofBSTRec(node->right);
			rightNodeHeight = node->right->height;
		}

		if (node->left) {
			node->left->height = heightofBSTRec(node->left);
			leftNodeHeight = node->left->height;
		}

		return !(abs(rightNodeHeight - leftNodeHeight) > MAX_IMBALANCE);

	}

	// Getting Balance Factor
	int getBalanceFactor(ANode* left, ANode* right) const {

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

	void LLRotation(ANode*& curr, ANode*& prev) {

		ANode* prevBData = curr->left->right;

		if (curr == root) {
			root = curr->left;
			curr->left->right = curr;
			curr->left = prevBData;
		}
		else {

			if (prev) {

				if (prev->right == curr) {
					prev->right = curr->left;
				}
				else {
					prev->left = curr->left;
				}

			}

			curr->left->right = curr;
			curr->left = prevBData;

		}

	}

	// RR Rotation

	void RRRotation(ANode*& curr, ANode*& prev) {

		ANode* prevBData = curr->right->left;

		if (curr == root) {
			root = curr->right;
			curr->right->left = curr;
			curr->right = prevBData;
		}
		else {

			if (prev) {

				if (prev->right == curr) {

					prev->right = curr->right;

				}
				else {

					prev->left = curr->right;

				}

			}

			curr->right->left = curr;
			curr->right = prevBData;

		}

	}

	// LR Rotation

	void LRRotation(ANode*& curr, ANode*& prev) {

		ANode* CLeftTree = curr->left->right->left;

		curr->left->right->left = curr->left;
		curr->left = curr->left->right;
		curr->left->left->right = CLeftTree;

		LLRotation(curr, prev);

	}

	// RL Rotation

	void RLRotation(ANode*& curr, ANode*& prev) {


		ANode* CRightTree = curr->right->left->right;

		curr->right->left->right = curr->right;
		curr->right = curr->right->left;
		curr->right->right->left = CRightTree;


		RRRotation(curr, prev);

	}


	// Tree Height Calculation.
	int heightofBSTRec(ANode* start) const {

		if (root == nullptr) return -1;

		return (1 + max(heightofBSTRecSub(start->left), heightofBSTRecSub(start->right)));

	}

	// Tree Height Sub-Function.
	int heightofBSTRecSub(ANode* curr) const {

		if (curr == nullptr) return 0;

		return (1 + max(heightofBSTRecSub(curr->left),
			heightofBSTRecSub(curr->right)));


	}


	// Printing in Tree Form

	// Print Tree Form
	void Printintreeform() const {

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
			cout << curr->data << endl;

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

		cleanTree(root);

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

		const int indexSize = 3;	// Keeps the index size.
		const char* fileNames[indexSize] = {
			"files/Doc1.txt",
			"files/Doc2.txt",
			"files/Doc3.txt"
		};							// Keeps the name of the files to initially index.

		system("cls");

		docsIndexed = 0;

		createIndex(fileNames, indexSize);

		while (true) {

			printInstructions();

			int querySize = 0;

			char** searchQuery = getSearchQuery(querySize);

			if (searchQuery[0][0] == '!') {
				executeInstruction(searchQuery);
			}
			else {
				search(searchQuery, querySize);
			}

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

		docsIndexed += indexSize;

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

		cout << "Terms that are in the index." << endl;
		for (auto x : results) {
			cout << *x << " ";
		}
		cout << endl;

		createRankings(results);


	}

	// Rankings Generator
	void createRankings(vector <TermInfo*> results) {

		vector <DocInfo> resultDocs;

		for (auto x : results) {

			for (auto y : x->DI) {

				resultDocs.push_back(y);

			}

		}

		vector <Rank> ranks;

		for (auto x : resultDocs) {

			Rank currentDoc;
			currentDoc.docId = x.docId;
			currentDoc.keywords = 0;
			currentDoc.collectiveFreq = 0;

			if (find(ranks.begin(), ranks.end(), currentDoc) == ranks.end()) {

				ranks.push_back(currentDoc);

			}

		}

		for (auto x : resultDocs) {

			Rank currentDoc;
			currentDoc.docId = x.docId;
			currentDoc.keywords = 0;
			currentDoc.collectiveFreq = 0;

			auto ranking = find(ranks.begin(), ranks.end(), currentDoc);

			ranking->keywords++;
			ranking->collectiveFreq += x.freq;


		}



		// Ranking Algorithm

		// Main ranking algorithm. O(n^2)

		for (auto i = ranks.begin(); i != ranks.end(); ++i) {

			for (auto j = ranks.begin(); j != ranks.end(); ++j) {

				/*

				Ranking is peformed on the following conditions.

				a) More keywords
				b) If equal keywords, then more collective frequency.
				c) If equal keywords and equal collective frequency, then docId.

				*/

				if (

					(*i).keywords > (*j).keywords ||

					(
						((*i).keywords == (*j).keywords) &&
						((*i).collectiveFreq > (*j).collectiveFreq)

						) ||

					(

						((*i).keywords == (*j).keywords) &&
						((*i).collectiveFreq == (*j).collectiveFreq) &&
						((*i).docId < (*j).docId)

						)

					) {

					swap((*i).keywords, (*j).keywords);
					swap((*i).collectiveFreq, (*j).collectiveFreq);
					swap((*i).docId, (*j).docId);

				}

			}

		}



		cout << "\n\nFound (" << ranks.size() << ") results.\n" << endl;

		for (auto x : ranks) {
			cout << x << endl;
		}



	}

	// General instructions printing function
	void printInstructions() {

		gotoxy(60, 2);
		cout << "!index - to print index.\n\n" << endl;
		gotoxy(60, 3);
		cout << "!add k - to add file k. " << endl;
		gotoxy(60, 4);
		cout << "Files: " << docsIndexed << ". " << endl;

	}

	// Instruction Parser
	void executeInstruction(char** searchQuery) {

		if (strcmp(searchQuery[0], "!index") == 0) {

			system("cls");
			index.Printintreeform();
			cin.get();


			// Danial Tree Printing Code.

			system("cls");
			index.print2DUtil(index.getRoot());
			cin.get();

		}
		else if (strcmp(searchQuery[0], "!indexs") == 0) {

			system("cls");
			index.print();
			cin.get();

		}
		else if (strcmp(searchQuery[0], "!add") == 0) {

			string file(searchQuery[1]);

			const char* filePtr = file.c_str();

			createIndex(&filePtr, 1);

			cout << "File added!" << endl;

		}

	}

	// Destructor
	~SearchEngine() {

		docsIndexed = 0;		// Resetting docs.

	}

};


int main() {

	SearchEngine s;

	return 0;
}
