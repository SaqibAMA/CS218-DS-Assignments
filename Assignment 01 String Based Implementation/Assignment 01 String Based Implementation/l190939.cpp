/*

Saqib Ali
19L-0939
CS-G

*/

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;



// --------- GRAPHICS RELATED ----------
#include <Windows.h>

// Handle for all graphics functionality.
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

// GotoXY

// This function allows to move the cursor
// to any (x, y)

void gotoxy(int x, int y) {

	COORD scrn;
	scrn.X = x;
	scrn.Y = y;

	SetConsoleCursorPosition(h, scrn);

}
// --------- GRAPHICS RELATED ----------


// Doubly Linked List Class
/*

Doubly Linked List class to aid
in development of the program.

*/
template <typename T>
class List {

private:

	// Node class to keep data nodes.
	class Node {

	public:
		// For private member access.
		friend class List;

		// Constructors
		Node() {
			prev = next = nullptr;
		}

		Node(T data) {
		
			this->data = data;
			prev = next = nullptr;

		}

		// Destructor
		~Node() {
			next = prev = nullptr;
		}

	private:
		Node* prev;		// Address of previous node.
		Node* next;		// Address of next node.
		T data;			// Data.
	};

	// Iterator class for traversal and access.
	class Iterator {

	public:
		// For access to private members.
		friend class List;

		// Hybrid constructor
		Iterator(Node* curr = nullptr) {

			this->curr = curr;

		}

		// Pre-increment operator.
		Iterator& operator ++ () {
		
			curr = curr->next;
			return *this;

		}

		// Pre-decriment operator.
		Iterator& operator -- () {

			curr = curr->prev;
			return *this;

		}

		// De-reference operator.
		T& operator * () {
			return curr->data;
		}

		// Operators
		bool operator == (const Iterator& I) const {
			return (this->curr == I.curr);
		}
		bool operator != (const Iterator& I) const {
			return !(*this == I);
		}

	public:
		Node* curr;		// Keeps the current address.

	};

public:

	// Constructor
	List() {
	
		head = new Node;
		tail = new Node;

		head->next = tail;
		head->prev = nullptr;

		tail->next = nullptr;
		tail->prev = head;

		size = 0;

	}

	// Member functions

	// Insertion
	bool insertAtHead(T data) {
	
		Node* newNode = new Node(data);

		newNode->prev = head;
		newNode->next = head->next;

		head->next = newNode;
		newNode->next->prev = newNode;

		size++;

		return true;

	}
	bool insertAtTail(T data) {
	
		Node* newNode = new Node(data);

		newNode->next = tail;
		newNode->prev = tail->prev;

		tail->prev->next = newNode;
		tail->prev = newNode;

		size++;

		return true;

	}

	// Removal
	bool removeAtHead() {
	
		if (head->next == tail) return false;

		Node* toBeRemoved = head->next;

		head->next = toBeRemoved->next;
		toBeRemoved->next->prev = head;

		delete toBeRemoved;

		size--;

		return true;

	}
	bool removeAtTail() {
	
		if (head->next == tail) return false;

		Node* toBeRemoved = tail->prev;

		toBeRemoved->prev->next = tail;
		tail->prev = toBeRemoved->prev;

		delete toBeRemoved;

		size--;

		return true;

	}
	bool removeAt(Iterator& it) {
	
		if (head->next == tail) return false;

		Node* toBeRemoved = it.curr;

		toBeRemoved->prev->next = toBeRemoved->next;
		toBeRemoved->next->prev = toBeRemoved->prev;

		it.curr = toBeRemoved->next;

		delete toBeRemoved;

		size--;

		return true;

	}

	// Operators
	void copy(const List& L) {
	
		//cout << "In the assignment operator" << endl;

		if (L.size) {
		
			if (this->size) {
			
				while (this->size) {

					this->removeAtTail();

				}

			}
			
			for (auto it = L.begin(); it != L.end(); ++it) {

				this->insertAtTail(*it);

			}


		}

		//cout << "Done copying." << endl;

	}
	
	// Iterator functions
	Iterator begin() const {
		Iterator it(head->next);
		return it;
	}
	Iterator end() const {
		Iterator it(tail);
		return it;
	}
	
	// Printing
	void print() const {
	
		for (auto i = begin(); i != end(); ++i)
			cout << "[" << *i << "]\n" << endl;

	}

	// Size
	size_t getSize() {
		return size;
	}

	// has
	bool has(const T& data) {
	
		for (auto i = begin(); i != end(); ++i) {
			if (*i == data) return true;
		}

		return false;

	}

	// get
	Iterator& get(const T& data) {

		for (auto i = begin(); i != end(); ++i)
			if (*i == data) return i;

		Iterator it;
		return it;

	}

	// Destructor
	~List() {

		if (head && tail) {

			Node* curr = head;
			Node* next = head->next;

			do {
				delete curr;
				curr = next;
				next = curr->next;
			} while (next != nullptr);

			head = tail = nullptr;

		}

	}


private:
	size_t size;		// Keeps the size of the list.
	Node* head;			// Start of the list.
	Node* tail;			// End of the list.
};

// Doc Info
/*

This class keeps a track of documents
and their frequency

*/
class DocInfo {

private:
	unsigned int docId;		// Document ID
	unsigned int freq;		// Keeps frequency of a certain word.

public:

	// Friendships for private access.
	friend class Rank;
	friend class SearchEngine;

	// Constructors
	DocInfo() {
		docId = freq = 0;
	}
	DocInfo(const DocInfo& D) {
		docId = D.docId;
		freq = D.freq;
	}
	DocInfo(unsigned int docId = 0, unsigned int freq = 0) {
		this->docId = docId;
		this->freq = freq;
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
		out << "\n[DocID: " << D.docId << ", Freq: " << D.freq << "] ";
		return out;
	}
	bool operator == (const DocInfo& D) {
		return (this->docId == D.docId);
	}

	~DocInfo() {

		// Not needed.

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

	~TermInfo() {
		// Not Needed
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

// Search Engine Class
/*

Holds the entire indexing and searching
functionality.

*/
class SearchEngine {

private:
	List <TermInfo> TI;				// Keeps the index.

	// For Graphics
	const int screenTop = 5;		// Margin from top.
	const int screenSide = 15;		// Margin from side.

	unsigned int docsIndexed = 0;	// Total documents indexed.

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

		createIndex(fileNames, indexSize);
									// Initiates indexing of files.
								
		while (true) {				// Main search engine loop.
			system("cls");
			setupScreen();
			search();
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

			ifstream fin(fileNames[i]);											// Opens the files one by one.

			DocInfo currentDocument(i + 1);										// Creates a document object
																				// gets appended with the term

			char temp[100];														// Buffer array to read from
																				// the file.
			while (!fin.eof()) {
			
				fin >> temp;

				for (unsigned int j = 0; j < strlen(temp); j++) {				// Makes all the capital letters
					if (temp[j] >= 'A' && temp[j] <= 'Z')						// into small ones.
						temp[j] += 32;
				}
				
				TermInfo term(temp);											// Term object that will ultimately
																				// be appended in the term index.

				if (!TI.has(term)) {											// If the terms doesn't exist,
																				// it is appended along with
					TI.insertAtTail(term);										// current doc's object.
					auto it = TI.get(term);
					currentDocument.freq = 1;
					(*it).addDoc(currentDocument);
					currentDocument.freq = 0;

				}
				else {															// If the term exists,
																				// we check if the current document
																				// already exists in its list of docs.
					auto it = TI.get(term);
					auto findDoc = find((*it).DI.begin(), (*it).DI.end(), currentDocument);

					if (findDoc != (*it).DI.end()) {							// If the current doc exists
																				// We change its frequency.
						findDoc->freq++;

					}
					else {														// If it doesn't exist,
																				// we append it to the term.
						currentDocument.freq = 1;
						(*it).addDoc(currentDocument);

					}


				}


			}

			fin.close();

		}

		docsIndexed = indexSize;												// Keeps the track of all documents
																				// that have been indexed.
																				// Convenient for docId.
	}

	// Search function
	void search() {

		/*
		
		This function performs the core functionality of traversing the index
		and finding the search terms.

		It then redirects its outputs to a ranking function that implements the
		ranking algorithm.

		*/

		int querySize = 0;
		char** query = getSearchQuery(querySize);								// Gets the tokenized query.

		if (query[0][0] == '+') addFileToIndex(query[1]);						// Special functions
		if (query[0][0] == '!') showIndex();

		vector <DocInfo> results;												// Holds the list of all
																				// the docs that have the terms.

		for (int i = 0; i < querySize; i++) {									// Traverses the whole index
																				// logs all the documents.
			string currentQuery = query[i];

			for (auto it = TI.begin(); it != TI.end(); ++it) {					// Traverse the entire document list.

				if ((*it).term == currentQuery) {								// If the term is in the index.
				
					for (auto x : (*it).DI) {

						results.push_back(x);									// Adding the document to
																				// the log.
					}

				}

			}

		}

		generateRankings(results);												// Direction to the ranking algorithm.


	}

	// Generate Rankings
	void generateRankings(vector <DocInfo> results) {

		/*
		
		This function has the implementation of ranking as specified
		in the documentation.

		*/
	
		List <Rank> rankings;													// Keeps the list of unique documents.

		for (auto x : results) {
		
			Rank currentRank(x);												// Rank object for every iteration.

			if (!rankings.has(currentRank)) {									// If the document is not in the rankings,
																				// add it with the default frequency.
				rankings.insertAtTail(currentRank);

			}
			else {																// If it is present in the rankings,
																				// update its collective frequency and keywords.
				auto it = rankings.get(currentRank);
				(*it).collectiveFreq += x.freq;
				(*it).keywords++;

			}

		}


		// Main ranking algorithm. O(n^2)
	
		for (auto i = rankings.begin(); i != rankings.end(); ++i) {

			for (auto j = rankings.begin(); j != rankings.end(); ++j) {

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

		SetConsoleTextAttribute(h, 4 | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY);
		int deviation = 6;
		for (auto i = rankings.begin(); i != rankings.end(); ++i) {

			gotoxy(screenSide, screenTop + deviation);
			cout << "                                               " << endl;
			gotoxy(screenSide, screenTop + 1 + deviation);
			cout << "                                               " << endl;
			gotoxy(screenSide, screenTop + 1 + deviation);
			cout << "  " << *i << "     " << endl;
			gotoxy(screenSide, screenTop + 2 + deviation);
			cout << "                                               " << endl;

			// Deviation pushes the cursor alone y-axis based
			// on what iteration we are on.

			deviation += 4;

		}
		SetConsoleTextAttribute(h, 15);
		cin.get();


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

	// Show Index function
	void showIndex() {

		int deviation = 7;
		gotoxy(screenSide, screenTop + deviation - 1);
		cout << "                                               " << endl;
		for (auto i = TI.begin(); i != TI.end(); ++i) {

			gotoxy(screenSide, screenTop + deviation);
			cout << "                                               " << endl;
			gotoxy(screenSide, screenTop + deviation);
			cout << "  " << (*i).term << "     " << endl;

			++deviation;

		}
		gotoxy(screenSide, screenTop + deviation);
		cout << "                                               " << endl;

	}

	// Add file to index
	void addFileToIndex(const char* fileName) {

		ifstream fin(fileName);															// Opening the individual file.

		DocInfo currentDocument(++docsIndexed);											// Creating a new document object.

		char temp[100];																	// Buffer for reading data.

		while (!fin.eof()) {

			fin >> temp;

			for (unsigned int j = 0; j < strlen(temp); j++) {							// Making the words lowercase.
				if (temp[j] >= 'A' && temp[j] <= 'Z')
					temp[j] += 32;
			}

			TermInfo term(temp);														// Making TermInfo object for unique terms.

			if (!TI.has(term)) {

				TI.insertAtTail(term);
				auto it = TI.get(term);
				currentDocument.freq = 1;
				(*it).addDoc(currentDocument);
				currentDocument.freq = 0;

			}
			else {

				auto it = TI.get(term);
				auto findDoc = find((*it).DI.begin(), (*it).DI.end(), currentDocument);

				if (findDoc != (*it).DI.end()) {

					findDoc->freq++;

				}
				else {

					currentDocument.freq = 1;
					(*it).addDoc(currentDocument);

				}


			}


		}

		fin.close();

	}

	// Destructor
	~SearchEngine() {
		// Not needed.
	}

	// Graphics related stuff.
	void setupScreen() {

		// Printing the search bar

		SetConsoleTextAttribute(h, 0 | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		gotoxy(screenSide + 2, screenTop + 1);
		cout << "                                               " << endl;
		gotoxy(screenSide + 2, screenTop + 2);
		cout << "                                               " << endl;
		gotoxy(screenSide + 2, screenTop + 3);
		cout << "                                               " << endl;
		gotoxy(screenSide + 2, screenTop + 4);
		cout << "                                               " << endl;

		SetConsoleTextAttribute(h, 0 | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY);
		gotoxy(screenSide, screenTop);
		cout << "                                               " << endl;
		gotoxy(screenSide, screenTop + 1);
		cout << "                                               " << endl;
		gotoxy(screenSide, screenTop + 2);
		cout << "                                               " << endl;
		gotoxy(screenSide, screenTop + 3);
		cout << "                                               " << endl;


		// Printing the instructions

		SetConsoleTextAttribute(h, 0 | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);

		int deviation = 60;
		gotoxy(screenSide + deviation + 2, screenTop + 1);
		cout << "                                 " << endl;
		gotoxy(screenSide + deviation + 2, screenTop + 2);
		cout << "                                 " << endl;
		gotoxy(screenSide + deviation + 2, screenTop + 3);
		cout << "                                 " << endl;
		gotoxy(screenSide + deviation + 2, screenTop + 4);
		cout << "                                 " << endl;


		SetConsoleTextAttribute(h, 0 | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY);

		gotoxy(screenSide + deviation, screenTop);
		cout << "                                 " << endl;
		gotoxy(screenSide + deviation, screenTop + 1);
		cout << "   ! - See the complete index.   " << endl;
		gotoxy(screenSide + deviation, screenTop + 2);
		cout << "   + - Add new document.         " << endl;
		gotoxy(screenSide + deviation, screenTop + 3);
		cout << "                                 " << endl;
		gotoxy(screenSide + 1 + deviation, screenTop + 1);
		gotoxy(screenSide + 1, screenTop + 1);


		// Printing further instructions

		// Printing the instructions

		SetConsoleTextAttribute(h, 0 | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);

		int topDev = 6;

		gotoxy(screenSide + deviation + 2, screenTop + topDev + 1);
		cout << "                                 " << endl;
		gotoxy(screenSide + deviation + 2, screenTop + topDev + 2);
		cout << "                                 " << endl;
		gotoxy(screenSide + deviation + 2, screenTop + topDev + 3);
		cout << "                                 " << endl;
		gotoxy(screenSide + deviation + 2, screenTop + topDev + 4);
		cout << "                                 " << endl;


		SetConsoleTextAttribute(h, 0 | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY);

		gotoxy(screenSide + deviation, screenTop + topDev);
		cout << "                                 " << endl;
		gotoxy(screenSide + deviation, screenTop + topDev + 1);
		cout << "   - Enter your query.           " << endl;
		gotoxy(screenSide + deviation, screenTop + topDev + 2);
		cout << "   - Press enter to search.      " << endl;
		gotoxy(screenSide + deviation, screenTop + topDev + 3);
		cout << "                                 " << endl;
		gotoxy(screenSide + 1, screenTop + 1);

		SetConsoleTextAttribute(h, 1 | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY);



	}


};


int main() {

	SearchEngine s;

	return 0;
}