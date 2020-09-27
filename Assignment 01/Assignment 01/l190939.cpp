#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;


// Doubly Linked List Class for Index

template <typename T>
class List {

public:

	// Constructor

	List() {

		head = new Node(0);
		head->next = tail;
		head->prev = nullptr;


		tail = new Node(0);
		tail->next = nullptr;
		tail->prev = head;
		
		size = 0;

	}

	// Member functions

private:

	// Node class to store data

	class Node {

	public:
		Node(T data = 0) {
			this->data = data;
		}

	private:
		T data;
		Node* next;
		Node* prev;
	};

	class Iterator {

	public:

		// Constructor
		Iterator(Node* node = nullptr) {
			curr = node;
		}

	private:
		Node* curr;	// keeps the current address

	};

private:
	Node* head;		// keeps head
	Node* tail;		// keeps tail
	size_t size;	// size_t to keep size

};


int main() {

	List <int> L;

	/*const char* fileFilter = "files/";*/

	return 0;
}