#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Helper Pair Struct
struct Pair {
	int key;
	int value;

	Pair(int key = 0, int value = 0) {
		this->key = key;  this->value = value;
	}

	friend ostream& operator << (ostream& out, const Pair& P) {
		out << "(" << P.key << ", " << P.value << ")";
		return out;
	}
};

// Custom Stack Class
template <typename T>
class stack {

private:
	unsigned int front;							// denotes top of the stack
	vector <T> data;							// keeps the current data.

public:

	// Constructor
	stack() {
		data.resize(10);						// Default size of the stack.
		front = 0;								// Stack position (pointer).
	}

	// Copy Constructor
	stack(const stack& s) {

		this->front = s.front;
		this->data = s.data;					// Using assignment operator
												// from vector class.

	}

	// Check Empty
	bool empty() const {
		return !front;							// If front is at zero.
	}

	// Check Full
	bool full() const {
		return (front == data.size());			// If the front has exceeded
												// vector indces.
	}


	// Push
	bool push(T element) {

		if (full()) {							// resizing the array
			data.resize(data.size() + 10);		// if it is full.
		}

		data[front] = element;
		front++;

		return true;

	}

	// Pop
	bool pop() {

		if (!empty()) {

			--front;							// just decrimenting the index
												// to keep the operation in O(1).
		}

		return false;

	}



	// Top
	T& top() {

		if (front) {
			return data[front - 1];
		}
		else {
			throw 100;
		}

	}

	T top() const {

		if (front) {
			return data[front - 1];
		}
		else {
			throw 100;
		}

	}

	// GetSize
	int size() const {
		return front;
	}


	// Assignment Operator
	stack& operator = (const stack& s) {

		this->front = s.front;
		this->data.resize(s.data.size());

		this->data = s.data;

		return *this;

	}

	// Printing operator
	friend ostream& operator << (ostream& out, stack& s) {

		while (!s.empty()) {
			out << s.top() << ", ";
			s.pop();
		}

		return out;
	}


	// Destructor
	~stack() {

		data.resize(0);
		front = 0;

	}

};


// Stage class to store data in an organized manner.
class Stage {


private:
	string name;				// Keeps the name of the task.
	unsigned int points;		// Keeps the point count.
	unsigned int time;			// Keeps the time needed to complete the task.

public:
	// Friendships
	friend class MindThrashing;	// To allow access to private members.

	// Hybrid Constructor
	Stage(
		string str = "",
		unsigned int points = 0,
		unsigned int time = 0) {

		this->name = name;
		this->points = points;
		this->time = time;

	}

	// Copy Constructor
	Stage(const Stage& S) {

		this->name = S.name;
		this->points = S.points;
		this->time = S.time;

	}

	// Input Operator
	friend istream& operator >> (istream& in, Stage& S) {

		cout << "Name: ";
		in >> S.name;
		cout << "Points: ";
		in >> S.points;
		cout << "Time: ";
		in >> S.time;

		return in;

	}


	// Output operator
	friend ostream& operator << (ostream& out, const Stage& S) {

		out << "[ Name: " << S.name << "; Points: "
			<< S.points << "; Time: " << S.time << "]";

		return out;

	}

	// Assignment operator
	Stage& operator = (const Stage& S) {
		this->name = S.name;
		this->points = S.points;
		this->time = S.time;

		return *this;
	}

	// Destructor
	~Stage() {
		name = "";
		points = 0;
		time = 0;
	}

};


// MindThrashing Class
class MindThrashing {

private:
	vector <Stage> stages;
	unsigned int timeToSpare;

public:

	// Constructor
	MindThrashing() {

		getStages();

		cout << "\n\nOptimal Choice (Recursive Approach):" << endl;
		findOptimalStagesRecursively();


		cout << "\n\nOptimal Choice (Iterative Approach): " << endl;
		findOptimalStagesIteratively();




	}

	// Input function
	void getStages() {

		int totalStages = -1;

		cout << "Enter total stages: ";
		cin >> totalStages;

		if (totalStages <= 0) {

			cout << "Invalid value." << endl;

		}
		else {

			cout << "Enter total time: ";
			cin >> timeToSpare;

			stages.resize(totalStages);

			for (int i = 0; i < totalStages; i++) {
				cin >> stages[i];
				cout << endl;
			}

		}

	}




	// Helper Function

	/*
	These functions help in calculating the total time
	and points of a given stack. These are helper functions.
	*/

	int getCummulativeTime(stack <Stage> stageCombination) {

		if (stageCombination.empty()) return -1;

		unsigned int totalTime = 0;

		while (!stageCombination.empty()) {
			totalTime += stageCombination.top().time;
			stageCombination.pop();
		}

		return totalTime;

	}

	int getCummulativePoints(stack <Stage> stageCombination) {

		if (stageCombination.empty()) return 0;

		unsigned int totalPoints = 0;

		while (!stageCombination.empty()) {
			totalPoints += stageCombination.top().points;
			stageCombination.pop();
		}

		return totalPoints;

	}

	int getCummulativeTime(vector <Stage> stageCombination, unsigned int size) {

		unsigned int totalTime = 0;

		for (unsigned int i = 0; i < size; i++)
			totalTime += stageCombination[i].time;

		return totalTime;

	}



	// Recursive Solution (Wrapper)
	void findOptimalStagesRecursively() {

		stack <int> activationRecord;

		stack <Stage> optimalChoice;
		stack <Stage> subset;

		activationRecord.push(0);
		activationRecord.push(0);

		findOptimalStagesRecursively(stages, stages.size(),
			subset, activationRecord, optimalChoice);
		printStack(optimalChoice);

	}


	// Recursive Solution (Sub-Function)
	void findOptimalStagesRecursively(vector <Stage> stages,
		unsigned int size,
		stack <Stage> & subset,
		stack <int> & activationRecord,
		stack <Stage> & optimalChoice) {


		int _subIndex = activationRecord.top();
		activationRecord.pop();
		int _setIndex = activationRecord.top();
		activationRecord.pop();


		if (_setIndex == size) {

			if (getCummulativeTime(subset) <= timeToSpare) {

				if (optimalChoice.empty()) {

					optimalChoice = subset;

				}
				else {

					if (getCummulativePoints(subset) >= getCummulativePoints(optimalChoice)) {

						optimalChoice = subset;

					}


				}

			}

			return;

		}
		else {

			while (subset.size() > _subIndex) {
				subset.pop();
			}

			if (getCummulativeTime(subset) <= timeToSpare) {

				if (optimalChoice.empty()) {

					optimalChoice = subset;

				}
				else {

					if (getCummulativePoints(subset) >= getCummulativePoints(optimalChoice)) {

						optimalChoice = subset;

					}
					

				}

			}

			subset.push(stages[_setIndex]);

			if (getCummulativeTime(subset) > timeToSpare) return;

			activationRecord.push(_setIndex + 1);
			activationRecord.push(_subIndex + 1);

			findOptimalStagesRecursively(stages, stages.size(),
				subset, activationRecord, optimalChoice);

			activationRecord.push(_setIndex + 1);
			activationRecord.push(_subIndex);

			findOptimalStagesRecursively(stages, stages.size(),
				subset, activationRecord, optimalChoice);

		}


	}


	// Iterative Approach
	void findOptimalStagesIteratively() {


		stack <Pair> activationRecord;

		// Generating Activation Record
		for (int i = 0; i < stages.size(); i++) {

			for (int j = 0; j < stages.size(); j++) {

				Pair P(i, j);
				activationRecord.push(P);

			}

		}

		stack <Stage> optimalChoice;


		// Generating Power Sets Iteratively

		while (!activationRecord.empty()) {

			Pair temp = activationRecord.top();

			int startIndex = temp.key;
			int endIndex = temp.value;

			int iter = startIndex;

			stack <Stage> subset;

			if (startIndex <= endIndex) {

				do {

					subset.push(stages[iter]);
					iter++;

				} while (iter <= endIndex);

			}
			else {

				iter = startIndex;

				do {

					subset.push(stages[iter]);
					iter++;
					if (iter == stages.size()) iter = 0;

				} while (iter != endIndex);

				subset.push(stages[iter]);

			}

			
			if (getCummulativeTime(subset) <= timeToSpare) {

				if (getCummulativePoints(subset) >= getCummulativePoints(optimalChoice)) {
					optimalChoice = subset;
				}

			}

			activationRecord.pop();

		}

		printStack(optimalChoice);


	}


	// Stack Printing Function
	void printStack(stack <Stage> optimalChoice) {

		while (!optimalChoice.empty()) {

			cout << optimalChoice.top() << ", ";
			optimalChoice.pop();

		}

		cout << endl;


	}

	// Destructor
	~MindThrashing() {
		timeToSpare = 0;
		stages.resize(0);
	}

};

int main() {

	MindThrashing m;

	return 0;
}