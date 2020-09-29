#include <iostream>
#include <cstring>

using namespace std;

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
		Node(T data = 0) {
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

	private:
		Node* curr;

	};

public:

	// Constructor

	List() {

		// null <- head -> tail -> null

		head = new Node(0);
		tail = new Node(0);

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
			if (*i == data) return false;

	}


private:
	Node* head;
	Node* tail;
	size_t size;
};


int main() {


	List <int> L;

	L.insertAtHead(5);
	L.insertAtHead(6);
	L.insertAtHead(7);
	L.insertAtTail(7);
	L.insertAtTail(7);
	L.deleteAtTail();
	L.deleteAtTail();
	L.deleteAtHead();

	for (auto i = L.begin(); i != L.end(); i++) {
		cout << *i << endl;
	}

	return 0;
}