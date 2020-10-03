#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

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

		Node* getAddress() {
			return curr;
		}

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

private:
	Node* head;
	Node* tail;
	size_t size;
};


class DocInfo {

private:
	int DocID;
	int freq;

public:

	// Friendships
	friend class SearchEngine;

	// Constructor
	DocInfo(int DocID = 0, int freq = 0) {
		this->DocID = DocID;
		this->freq = freq;
	}

	// Member functions
	void incrementFrequency() {
		++freq;
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

		if (this->term) delete[] this->term;

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

		if (this->term) delete[] term;

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
			out << "[" << T.term << "] -> ";
			T.DI.print();
			cout << endl;
		}



		return out;

	}

	// Destructor
	~TermInfo() {
		//if (term) delete[] term;
		//term = nullptr;
	}

};



// Search Engine Class
class SearchEngine {

private:
	List <TermInfo> TI;

public:
	SearchEngine() {

	}

	// Initializer function
	// for search engine
	void init() {

		const int indexSize = 4;
		const char* fileNames[indexSize] = {
			"files/Doc1.txt",
			"files/Doc2.txt",
			"files/Doc3.txt",
			"files/Doc4.txt",
		};

		createIndex(fileNames, indexSize);
		
		while (true) {
			search();
		}

	}

	// Function for Indexing
	void createIndex(const char** fileNames, const int indexSize) {

		for (int i = 0; i < indexSize; i++) {

			ifstream fin(fileNames[i]);

			// Creating a DocInfo Entry
			DocInfo docInfo(i + 1);

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

			// Closing the file to open a new one.
			fin.close();

		}

		 // TI.print();

	}

	// Function for Searching
	void search() {

		int querySize = 0;
		char** query = getSearchQuery(querySize);

		int resultSize = 0;

		List <List<DocInfo>*> result;

		for (int i = 0; i < querySize; i++) {

			TermInfo term(query[i]);

			if (TI.has(term)) {

				auto it = TI.get(term);
				result.insertAtTail(&(*it).DI);

			}

		}

		for (auto i = result.begin(); i != result.end(); i++) {
			(*(*i)).print();
			cout << endl;
		}

	}


	// Tokenizer Function
	char** getSearchQuery(int& querySize) {

		// Data members for necessary operations.
		// Building on stack because it's efficient.

		const int buffSize = 100;
		char temp[buffSize];

		char copyBuffer[20][buffSize];
		int copyBuffIter = 0;


		// Taking the query input.

		cout << "Search: ";
		cin.getline(temp, 100, '\n');

		// Scanning through the whole query
		// and splitting it into words

		for (int i = 0; temp[i] != '\0' && i <= strlen(temp); i++) {

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


};


int main() {

	SearchEngine s;
	s.init();

	return 0;
}