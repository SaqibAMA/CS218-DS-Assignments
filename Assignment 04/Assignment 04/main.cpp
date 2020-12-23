#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

typedef unsigned long long int ull;

#define _CRT_SECURE_NO_WARNINGS


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
		Iterator (const Iterator& I) {
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
		
		head = make_shared <Node> ();		// dummy head.
		tail = make_shared <Node> ();		// dummy tail.

		head->next = tail;
		tail->prev = head;

		size = 0;

	}

	// Copy constructor.
	List(const List& L) {
		
		head = make_shared <Node> ();
		tail = make_shared <Node> ();

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
		
		shared_ptr <Node> newData = make_shared <Node> ();

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
		
		shared_ptr <Node> newData = make_shared <Node> ();

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

// DocInfo Class
class DocInfo {
	
private:
	unsigned int docId;
	unsigned int freq;
	string name;

public:


	// Friends
	friend class TermInfo;
	friend class SearchEngine;
	friend class Rank;
	

	// Def. and Param. Constructors.
	DocInfo(unsigned int docId = 0, unsigned int freq = 0, string name = "") {
	
		this->docId = docId;
		this->freq = freq;
		this->name = name;

	}

	// Operators
	friend ostream& operator << (ostream& out, const DocInfo& D) {
		cout << "[ ID: " << D.docId << "; " << D.name <<  "; F: " << D.freq << "]";
		return out;
	}

	bool operator == (const DocInfo& D) {
		return (this->docId == D.docId);
	}
	
	// Destructors
	~DocInfo () {
		docId = 0;
		freq = 0;
	}

};

// TermInfo Class
class TermInfo {
	
private:
	List <DocInfo> docs;
	string term;

public:

	// Friendships
	friend class HashTable;
	friend class SearchEngine;
	
	// Constructors
	TermInfo(string term = "") {
		this->term = term;
	}

	TermInfo(const TermInfo& T) {
		this->term = T.term;
		this->docs = T.docs;
	}

	// Adding Docs
	void insertDoc(const DocInfo& D) {
		docs.push_back(D);
	}

	// Getting Docs
	DocInfo& getDoc(unsigned int docId) {
		for (auto i : docs)
			if (i.docId == docId) return i;
	}

	// Checking Doc Presence
	bool hasDoc(const unsigned int docId) {
		
		for (auto i : docs)
			if (i.docId == docId) return true;

		return false;

	}

	// Increment Freq
	void incrementFrequency(const unsigned int docId) {
		
		DocInfo d(docId);
		
		docs.get(d).curr->data.freq++;

	}

	// Getting Term
	string getTerm() const {
		return term;
	}


	// Operators
	TermInfo& operator = (const TermInfo& T) {
		this->term = T.term;
		this->docs = T.docs;

		return *this;
	}

	bool operator == (const TermInfo& T) {
		return (this->term == T.term);
	}

	friend ostream& operator << (ostream& out, const TermInfo& T) {
		
		cout << char(200) << char(205) << char(219);
		cout << char(178) << char(177) << char(176) << " \t " << T.term << endl;
		for (auto i : T.docs) {
			cout << "\t\t" << i << endl;
		}

		return out;
	}

	// Destructor
	~TermInfo() {
		// Not needed.
	}

};


// HashTable Class
class HashTable {

private:
	vector <List<TermInfo>> table;		// keeps the hashtable.
	
	vector <int> status;				// keeps the status of elements.
										// 0 - empty
										// 1 - filled

	size_t size;						// keeps all elements.
	size_t maxSize;						// keeps maximum elements.

public:

	// Constructor.
	HashTable() {
		
		size = 0;
		maxSize = 31;					// default initialization to 31 (Prime).
		table.resize(maxSize);
		status.resize(maxSize);

	}

	// Hash function.
	ull hash(string const term) const {

		// hash = ( s[0] + s[1] p + s[2] p^2 + s[3] p^3 ... + s[n] p^n ) mod maxSize;

		ull hashValue = 0;		// this holds the hash value.
		int p = 3;				// this is the prime number we will be using.


		for (unsigned int i = 0; i < term.length(); i++)
			hashValue += term[i] * (ull) pow(p, i);

		hashValue %= maxSize;



		return hashValue;		// return the hash after computing.

	}

	// Searching function.
	List <TermInfo>& get(string const term) {
		
		ull hashValue = hash(term);

		if ((int)hashValue >= maxSize || status[(int)hashValue] != 1) {
			List <TermInfo> dummy;
			return dummy;
		}

		return table[(int)hashValue];

	}

	// Get Specific Term.
	TermInfo* getTerm(string const term) {

		ull hashValue = hash(term);

		if ((int)hashValue >= maxSize || status[(int)hashValue] != 1) {
			
			return nullptr;

		}

		List <TermInfo>& targetIndex = table[(int)hashValue];

		auto listIter = targetIndex.begin();

		for (size_t i = 0; i < targetIndex.getSize(); i++) {
			
			if (listIter.curr->data.getTerm() == term) {

				return &(listIter.curr->data);
			
			}

			listIter++;

		}

		return nullptr;

	}

	// Insertion function.
	void insert(const TermInfo& data) {
		
		ull hashValue = hash(data.term);
		float lf = (float) size / (float) maxSize;

		if (lf >= 2) {
			
			this->doubleCapacity();

		}

		table[(int)hashValue].push_back(data);		// insert data into map.
		status[(int)hashValue] = 1;					// mark as occupied.

		size++;

		// cout << data << " @ " << hashValue << ", lf: " << lf << endl;

	}

	// Table extension function.
	void doubleCapacity() {
		
		vector <List<TermInfo>> extendedTable;								// New hashtable.
		vector <int> extendedStatus;

		extendedTable.resize(table.size() * 2);								// The size is twice as previous.
		extendedStatus.resize(table.size() * 2);							// The size is twice as previous.

		maxSize *= 2;														// Making max size double.

		for (unsigned int i = 0; i < table.size(); i++) {

			auto listIter = table[i].begin();

			for (size_t j = 0; j < table[i].getSize(); j++) {


				TermInfo& frontElem = table[i].peekFront();
				
				ull hashValue = hash(frontElem.term);						// re-hashing value.

				extendedTable[(int)hashValue].push_back(*listIter);				// inserting re-hashed value.
				extendedStatus[(int)hashValue] = 1;								// marking as occupied.

				listIter++;
			
			}

		}

		table = extendedTable;
		status = extendedStatus;


	}

	// Printing function for debugging.
	void print(bool debug = false) const {
		
		for (unsigned int i = 0; i < maxSize; i++) {
			
			if (debug) {
			
				cout << "[" << i << "] -> ";

			}

			if (status[i]) {

				cout << "[" << i << "] -> ";
			
				cout << table[i].getSize() << " element(s).";

				cout << endl;
				table[i].print();
				cout << endl;

			}

			if (debug) {

				cout << endl;
			
			}

		}

	}

	// Destructor.
	~HashTable() {
		// Not needed.
	}

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

	unsigned int docsIndexed;	// Total documents indexed.

	HashTable index;

public:
	// Constructor.
	SearchEngine() {
		init();					// Starts the search engine.
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

			if (!fin) {
				cout << "Unable to open file!" << endl;
				return;
			}

			DocInfo currentDocument;
			currentDocument.docId = docsIndexed + i + 1;
			currentDocument.name = fileNames[i];
			currentDocument.freq = 1;

			while (!fin.eof()) {

				fin >> temp;

				TermInfo currentTerm(temp);
				currentTerm.insertDoc(currentDocument);

				string buff (temp);

				if (!index.getTerm(temp)) {

					index.insert(currentTerm);

				}
				else {

					TermInfo* existingTerm = index.getTerm(temp);

					if (existingTerm->hasDoc(i + 1)) {
						existingTerm->incrementFrequency(i + 1);
					}
					else {
						existingTerm->insertDoc(currentDocument);
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

			if (index.getTerm(searchQuery[i])) {

				results.push_back(index.getTerm(searchQuery[i]));

			}

		}

		/*cout << "Terms that are in the index." << endl;
		for (auto x : results) {
			cout << *x << " " << endl;
		}
		cout << endl;*/

		createRankings(results);


	}

	// Rankings Generator
	void createRankings(vector <TermInfo*> results) {

		vector <DocInfo> resultDocs;

		for (auto x : results) {

			for (auto y : x->docs) {

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

	system("pause");

	return 0;
}