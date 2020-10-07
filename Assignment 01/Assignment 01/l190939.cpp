#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

int totalBlocksAllocated = 0;

//void* operator new (size_t size) {
//
//	cout << "Allocated Block # " << ++totalBlocksAllocated << endl;
//	return malloc(size);
//
//}
//
//void operator delete (void* mem) {
//
//	cout << "Allocated Block # " << --totalBlocksAllocated << endl;
//	free(mem);
//
//}

// Related to Graphics
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


// Doubly Linked List Implementation
template <typename T>
class List {

	// Node Class
	class Node {

	private:
		Node* prev;
		Node* next;
		T data;
	public:

		friend class List;

		// Constructor
		Node() {
			prev = next = nullptr;
		}

		Node(T data) {
			prev = next = nullptr;
			this->data = data;
		}

		~Node() {/*Not needed.*/}

	};

	// Iterator Class
	class Iterator {

	public:

		friend class List;

		// Constructor
		Iterator(Node* curr = nullptr) {
			this->curr = curr;
		}

		// Operators
		
		bool operator == (const Iterator& I) const {
			return (this->curr == I.curr);
		}

		bool operator != (const Iterator& I) const {
			return !(*this == I);
		}

		Iterator operator ++ (int) {
			this->curr = this->curr->next;
			return *this;
		}

		Iterator operator -- (int) {
			this->curr = this->curr->prev;
			return *this;
		}

		T& operator * () {
			return (this->curr->data);
		}

		//Node* getAddress() {
		//	return curr;
		//}

	private:
		Node* curr;

	};

public:

	// Constructor

	List() {

		// null <- head -> tail -> null

		head = new Node;
		tail = new Node;

		head->prev = nullptr;
		head->next = tail;
		
		tail->prev = head;
		tail->next = nullptr;

		size = 0;
	
	}

	// Insertion
	Iterator& insertNodeAt(const Iterator& I, T data) {

		/*
		Insert a piece of data at any point of the linked list.
		*/

		Node* newData = new Node(data);

		newData->next = I.curr;
		newData->prev = I.curr->prev;

		I.curr->prev->next = newData;
		I.curr->prev = newData;

		++size;

		Iterator it(newData);
		return it;

	}
	Iterator& insertAtHead(T data) {
		Iterator it(head->next);
		return insertNodeAt(it, data);
	}
	Iterator& insertAtTail(T data) {
		Iterator it(tail);
		return insertNodeAt(it, data);
	}

	// Deletion
	Iterator& deleteNodeAt(const Iterator& I) {


		/*
		Delete a piece of data at any point of the list.
		*/

		if (I != end()) {

			Node* obj1 = I.curr->prev;
			Node* obj2 = I.curr;
			Node* obj3 = I.curr->next;

			obj3->prev = obj1;
			obj1->next = obj3;

			delete (obj2);
			Iterator it(obj1);
			return it;

		}

	}
	Iterator& deleteAtHead() {
		Iterator it(head->next);
		return deleteNodeAt(it);
	}
	Iterator& deleteAtTail() {
		Iterator it(tail->prev);
		return deleteNodeAt(it);
	}

	// Iterator related functions
	Iterator begin() const {
		Iterator it(head->next);
		return it;
	}
	Iterator end() const {
		Iterator it(tail);
		return it;
	}

	// Custom Member Functions
	bool has(T data) {

		for (auto i = begin(); i != end(); i++)
			if (*i == data) return true;

		return false;

	}

	bool isEmpty() {
		return (head->next == tail->prev);
	}
	Iterator& get(T data) {
		for (auto it = begin(); it != end(); it++)
			if (*it == data) return it;

	}


	// Printing function
	void print () const {
		
		for (auto it = begin(); it != end(); it++)
			cout << *it;

	}

	// Destructor
	~List() {

		/*if (head) {

			Node* it = head;

			while (it) {

				Node* next = it->next;
				delete it;
				it = next;

			}

		}*/

	}

private:
	Node* head;
	Node* tail;
	size_t size;
};

// DocInfo Class
class DocInfo {

private:
	int DocID;
	int freq;

public:

	// Friendships
	friend class SearchEngine;
	friend class Rank;

	// Constructor
	DocInfo(int DocID = 0, int freq = 0) {
		this->DocID = DocID;
		this->freq = freq;
	}

	// Member functions
	void incrementFrequency() {
		++freq;
	}

	void incrementFrequency(unsigned int n) {
		freq += n;
	}

	void resetFrequency() {
		freq = 0;
	}

	// Operators
	friend ostream& operator << (ostream& out, const DocInfo& D) {
		
		out << "[ID: " << D.DocID << " TF: " << D.freq << "] ";
		return out;

	}
	bool operator == (const DocInfo& D) const {

		return (this->DocID == D.DocID);

	}

	// Destructor
	// Not needed.

};

// TermInfo Class
class TermInfo {

private:
	char* term;
	List <DocInfo> DI;

public:

	// Friendships
	friend class SearchEngine;

	// Constructor
	TermInfo(const char* term = nullptr) {
		
		if (term) {

			this->term = new char[strlen(term) + 1];
			strcpy(this->term, term);

		}
		else {

			this->term = nullptr;
		
		}

	}

	// Member Functions
	void addDoc(const DocInfo& D) {
		DI.insertAtTail(D);
	}
	DocInfo& getDoc(const DocInfo& D) const {
		
		for (auto i = DI.begin(); i != DI.end(); i++)
			if (*i == D) return *i;

		DocInfo d (-1);
		return d;

	}

	// Operators
	bool operator == (const char* term) const {
		return !(strcmp(this->term, term));
	}
	bool operator == (const TermInfo& T) const {
		return !(strcmp(this->term, T.term));
	}


	TermInfo& operator = (const TermInfo& T) {

		if (this->term) delete this->term;
		this->term = nullptr;

		if (T.term) {

			this->term = new char[strlen(T.term) + 1];
			strcpy(this->term, T.term);

		}
		else {

			this->term = nullptr;

		}

		return *this;


	}
	TermInfo& operator = (const char* term) {

		cout << "Changing term from: ";
		if (this->term) cout << this->term;
		cout << " to " << term;

		if (this->term) delete[] this->term;
		this->term = nullptr;

		if (term) {

			this->term = new char[strlen(term) + 1];
			strcpy(this->term, term);

		}
		else {

			this->term = nullptr;

		}

	}
	
	// Setting DocInfo List
	void setDocInfoList(const TermInfo& T) {
	
		auto it = T.DI.begin();

		DI.insertAtTail(*it);

	}

	friend ostream& operator << (ostream& out, const TermInfo& T) {
		
		if (T.term) {
			out << "[" << T.term << "]";
			T.DI.print();
			cout << endl;
		}



		return out;

	}

	// Destructor
	~TermInfo() {
		/*free(term);
		term = nullptr;*/
	}

};

// Ranking Class
class Rank {
public:
	int docId;
	int collectiveFreq;
	int keywords;

public:
	Rank() {
		docId = collectiveFreq = keywords = 0;
	}

	Rank(const DocInfo& D) {

		docId = D.DocID;
		collectiveFreq = D.freq;
		keywords = 1;

	}

	bool operator == (const Rank& R) const {
	
		return (docId == R.docId);

	}

	bool operator != (const Rank& R) const {

		return (docId == R.docId);

	}

	friend ostream& operator << (ostream& out, const Rank& R) {

		out << "[ID: " << R.docId << "] " << "[CF: " << R.collectiveFreq << "] " << "[Keywords: " << R.keywords << "].";

		return out;
	}

	~Rank() {
		docId = collectiveFreq = keywords = 0;
	}

};

// Search Engine Class
class SearchEngine {

private:
	List <TermInfo> TI;

	// For Graphics
	const int screenTop = 5;
	const int screenSide = 35;

	unsigned int docsIndexed;

public:
	SearchEngine() {
		init();
	}

	// Initializer function.

	/*
	
	This function contains the main loop for operating the search engine.

	*/

	void init() {

		const int indexSize = 4;
		const char* fileNames[indexSize] = {
			"files/_Doc1.txt",
			"files/_Doc2.txt",
			"files/_Doc3.txt",
			"files/_Doc4.txt",
		};

		createIndex(fileNames, indexSize);
		
		while (true) {
			setupScreen();
			search();
		}

	}

	// Function for Indexing

	/*
	
	This function creates an index according to the specified requirements.

	*/

	void createIndex(const char** fileNames, const int indexSize) {

		for (int i = 0; i < indexSize; i++) {

			ifstream fin(fileNames[i]);
			if (!fin) {
				cerr << "Something is wrong with files." << endl;
				system("pause");
			}

			// Creating a DocInfo Entry
			DocInfo docInfo(i + 1);

			// Reading the file word-by-word
			while (!fin.eof()) {

				char temp[100];
				fin >> temp;

				TermInfo termInfo(temp);

				// cout << "TermInfo at this point: " << termInfo << endl;
				
				// If it is a unique word
				if (!TI.has(termInfo)) {

					docInfo.incrementFrequency();
					TI.insertAtTail(termInfo);

					auto i = TI.get(termInfo);
					(*i).addDoc(docInfo);

					docInfo.resetFrequency();

				}
				else {
					
					// If it is not a unique word.

					auto i = TI.get(termInfo);
					DocInfo& doc = (*i).getDoc(docInfo);

					if (doc.DocID == -1) {
						
						docInfo.incrementFrequency();
						auto i = TI.get(termInfo);
						(*i).addDoc(docInfo);
						docInfo.resetFrequency();

					}
					else {
						doc.incrementFrequency();
					}


				}


			}

			// Closing the file to open a new one.
			fin.close();

		}

		docsIndexed = indexSize;

		 // TI.print();

	}

	// Function for Searching.
	/*
	
	This function takes input and calls the tokenizer function to parse it.
	Then it searches the input using the index.

	*/
	void search() {

		List <TermInfo> searchTerms;

		int querySize = 0;
		char** query = getSearchQuery(querySize);

		if (query[0][0] == '+') addFileToIndex(query[1]);

		int resultSize = 0;

		List <List<DocInfo>*> result;

		for (int i = 0; i < querySize; i++) {

			TermInfo term(query[i]);

			if (TI.has(term)) {

				auto it = TI.get(term);
				result.insertAtTail(&(*it).DI);
				searchTerms.insertAtTail(term);

			}

		}

		createRankings(searchTerms, result);

	}


	// Add File to Index

	/*
	
	This function adds a file to the index.

	*/

	void addFileToIndex(char* fileName) {

		// cout << "Parsing" << endl;

		ifstream fin(fileName);
		if (fin) {

			// Creating a DocInfo Entry
			DocInfo docInfo(++docsIndexed);

			// Reading the file word-by-word
			while (!fin.eof()) {

				char temp[100];
				fin >> temp;

				TermInfo termInfo(temp);

				// If it is a unique word
				if (!TI.has(termInfo)) {

					docInfo.incrementFrequency();
					TI.insertAtTail(termInfo);

					auto i = TI.get(termInfo);
					(*i).addDoc(docInfo);

					docInfo.resetFrequency();

				}
				else {

					// If it is not a unique word.

					auto i = TI.get(termInfo);
					DocInfo& doc = (*i).getDoc(docInfo);

					if (doc.DocID == -1) {

						docInfo.incrementFrequency();
						auto i = TI.get(termInfo);
						(*i).addDoc(docInfo);
						docInfo.resetFrequency();

					}
					else {
						doc.incrementFrequency();
					}


				}


			}


			SetConsoleTextAttribute(h, 6 | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY);
			gotoxy(screenSide, screenTop + 5);
			cout << "                                               " << endl;
			gotoxy(screenSide, screenTop + 6);
			cout << "  [File Indexed]                               " << endl;
			gotoxy(screenSide, screenTop + 7);
			cout << "                                               " << endl;


		}

		fin.close();

	}


	// Rankings Function.

	/*
	
	This does not necessarily calculate the ranking, however, it combines all the docs
	and pushes them into calculateRankings() function.

	Also, it brings out unique terms in the query.

	*/

	void createRankings(List<TermInfo> searchTerms, List<List<DocInfo>*> result) {

		auto searchTermsIter = searchTerms.begin();

		SetConsoleTextAttribute(h, 10);
		
		int deviation = 0;

		for (auto i = result.begin(); i != result.end(); i++) {
			gotoxy(screenSide + deviation, screenTop + 4);
			cout << *searchTermsIter;
			searchTermsIter++;
			deviation += 15;

		}

		SetConsoleTextAttribute(h, 15);


		List <DocInfo> documentList;

		for (auto i = result.begin(); i != result.end(); i++) {

			for (auto j = (*(*i)).begin(); j != (*(*i)).end(); j++) {

				documentList.insertAtTail(*j);

			}

		}

		calculateRankings(documentList);


	}

	// Calculate Ranking Function
	/*
	
	This function implements the ranking algorithm as well as
	filter out the document data.

	*/

	void calculateRankings(List <DocInfo> documentList) {

		List <Rank> rankings;			// Keeps the actual ranking.
		List <DocInfo> uniqueDocs;		// Keeps the list of documents uniquely.


		// This loop performs crucial functions of:
		// a) Collecting all the documents currently under analysis.
		// b) Remove duplicates from that linked list.
		// c) Calculate CF and Keyword Count for the Rank object.

		for (auto i = documentList.begin(); i != documentList.end(); i++) {

			Rank docRank(*i);

			// If the doc is not in, put it in.
			// If it is already present, iterate the keyword count and update CF.

			if (!uniqueDocs.has(*i)) {
				uniqueDocs.insertAtTail(*i);

				rankings.insertAtTail(docRank);
			}
			else {
				auto iter = uniqueDocs.get(*i);
				(*iter).freq += (*i).freq;

				auto r = rankings.get(*i);
				(*r).collectiveFreq += (*i).freq;
				(*r).keywords += 1;

			}

		}


		// Ranking Algorithm Implementation.
		// Works in O(n^2) to compare all elements
		// with each other and swaps data based on conditions.

		for (auto i = rankings.begin(); i != rankings.end(); i++) {
		
			for (auto j = rankings.begin(); j != rankings.end(); j++) {

				// Implementation of the ranking algorithm

				// a) If keyword count is greater, then rank higher.
				// b) If keyword count is equal, but TF is higher, then rank higher.
				// c) If keyword count and TF are equal respectively, then rank higher on the basis of ID.

				if (
					((*i).keywords > (*j).keywords) ||

					((*i).keywords == (*j).keywords &&
					(*i).collectiveFreq > (*j).collectiveFreq) ||
					
					((*i).keywords == (*j).keywords &&
					(*i).collectiveFreq == (*j).collectiveFreq &&
					(*i).docId < (*j).docId)
					
					) {
				
					swap((*i).keywords, (*j).keywords);
					swap((*i).collectiveFreq, (*j).collectiveFreq);
					swap((*i).docId, (*j).docId);
				
				}

			}
		
		}


		// Printing all the search results on the screen.

		SetConsoleTextAttribute(h, 4 | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY);
		int deviation = 4;
		for (auto i = rankings.begin(); i != rankings.end(); i++) {

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


		// To avoid instant clearing of screen
		// and eventually clear it for repeated use.

		cin.get();
		system("cls");

	}

	// Tokenizer Function

	/*
	
	This takes a variable query size that keeps a record
	of the total number of words in the query.

	*/

	char** getSearchQuery(int& querySize) {

		// Data members for necessary operations.
		// Building on stack because it's efficient.

		const int buffSize = 100;
		char temp[buffSize];

		char copyBuffer[20][buffSize];
		int copyBuffIter = 0;


		// Taking the query input.

		cout << ">>: ";
		cin.getline(temp, 100, '\n');

		// Scanning through the whole query
		// and splitting it into words

		for (unsigned int i = 0; temp[i] != '\0' && i <= strlen(temp); i++) {

			int j = i;
			copyBuffIter = 0;
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

	// Screen Setup Function

	/*
	
	This function sets up the search console by printing it
	on specific coordinates. This is called every time we
	perform a search so that the screen is reset.

	*/

	void setupScreen() {

		// Printing the search bar

		SetConsoleTextAttribute(h, 0 | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY);
		gotoxy(screenSide, screenTop);
		cout << "                                               " << endl;
		gotoxy(screenSide, screenTop + 1);
		cout << "                                               " << endl;
		gotoxy(screenSide, screenTop + 2);
		cout << "                                               " << endl;
		gotoxy(screenSide + 1, screenTop + 1);

	}

};

int main() {

	SearchEngine s;

	return 0;
}