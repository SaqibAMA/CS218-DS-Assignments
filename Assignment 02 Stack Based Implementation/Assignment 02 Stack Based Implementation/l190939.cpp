#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <ctime>
//#include <stack>

using namespace std;


// Custom stack class.
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

	//Stage class to keep data for every stage.

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

	// Equality Operator
	bool operator == (const Stage& s) {
		return (
			this->name == s.name &&
			this->points == s.points &&
			this->time == s.time);
	}

	bool operator != (const Stage& s) {
		return !(*this == s);
	}

	// Destructor
	~Stage() {
		name = "";
		points = 0;
		time = 0;
	}

};

// Mindthrashing class to hold all the functionality.
class MindThrashing {

	// MindThrashing class with primary functionality.

private:
	vector <Stage> stages;			// Keeps all the stages.
	unsigned int timeToSpare;		// Keeps the time to spare.

public:


	// Constructor
	MindThrashing() {


		getStages();


	}

	// Input function
	void getStages() {

		unsigned int totalStages;										// Taking input for total stages.
		cout << "STAGES: ";
		cin >> totalStages;

		stages.resize((size_t)totalStages);

		cout << "TIME TO SPARE: ";										// Input for total time to spare.
		cin >> timeToSpare;

		for (unsigned int i = 0; i < totalStages; i++) {				// Actual input loop.

			cout << "NAME FOR #" << i + 1 << " : ";
			cin >> stages[i].name;

			cout << "TIME FOR #" << i + 1 << " : ";
			cin >> stages[i].time;

			cout << "PTS FOR  #" << i + 1 << " : ";
			cin >> stages[i].points;

		}


	}

	

	/*
	These functions help in calculating the total time
	and points of a given stack. These are helper functions.
	*/
	unsigned int getCummulativeTime(stack <Stage> stageCombination) {

		unsigned int totalTime = 0;

		while (!stageCombination.empty()) {
			totalTime += stageCombination.top().time;
			stageCombination.pop();
		}

		return totalTime;

	}
	unsigned int getCummulativePoints(stack <Stage> stageCombination) {

		unsigned int totalPoints = 0;

		while (!stageCombination.empty()) {
			totalPoints += stageCombination.top().points;
			stageCombination.pop();
		}

		return totalPoints;

	}
	unsigned int getCummulativeTime(vector <Stage> stageCombination, unsigned int size) {

		unsigned int totalTime = 0;

		for (unsigned int i = 0; i < size; i++)
			totalTime += stageCombination[i].time;

		return totalTime;

	}

	
	// Recursive Solution
	void findOptimalStagesRecursively() {
	
		stack <int> activationRecord;

		stack <Stage> optimalChoice;
		stack <Stage> subset;

		activationRecord.push(0);
		activationRecord.push(0);

		findOptimalStagesRecursively(stages, stages.size(),
			subset, activationRecord, optimalChoice);


		cout << "\n\nTIME CONSTRAINT: " << timeToSpare << endl;

		cout << "\n\nBEST CHOICE: " << endl;


		printStack(optimalChoice);

	}

	void findOptimalStagesRecursively(vector <Stage> stages,
		unsigned int size,
		stack <Stage> & subset,
		stack <int> & activationRecord,
		stack <Stage> & optimalChoice) {

		// Fetching the values that
		// were pushed through the stack.
		int _subIndex = activationRecord.top();
		activationRecord.pop();
		int _setIndex = activationRecord.top();
		activationRecord.pop();

		// Base case, we have traversed
		// all of the stages.
		if (_setIndex == size) {

			// Consistently keep the
			// optimal subset in the stack.
			if (getCummulativePoints(subset) > getCummulativePoints(optimalChoice)) {
						
				if (!optimalChoice.empty() &&
					getCummulativeTime(subset) <= getCummulativeTime(optimalChoice)) {
					optimalChoice = subset;
				}
				else {
					optimalChoice = subset;
				}

			}

			return;

		}
		else {

			// Pops up the stack until a certain size.
			while (subset.size() > _subIndex) {
				subset.pop();
			}
			subset.push(stages[_setIndex]);


			// If we have crossed the time threshold, it is not worth it
			// to go forward.
			if (getCummulativeTime(subset) > timeToSpare) return;

			// This pushes the next pair on the stack
			// for further subset generation.
			activationRecord.push(_setIndex + 1);
			activationRecord.push(_subIndex + 1);

			// First recursion call that includes the element.
			findOptimalStagesRecursively(stages, size,
				subset, activationRecord, optimalChoice);
			
			activationRecord.push(_setIndex + 1);
			activationRecord.push(_subIndex);

			subset.pop();

			// Second recursion call that excludes the element.
			findOptimalStagesRecursively(stages, stages.size(),
				subset, activationRecord, optimalChoice);



		}

	}

	/*

	// Iterative Solution
	void findOptimalStagesIteratively() {

		stack <int> activationRecord;
		stack <Stage> optimalChoice;
		stack <Stage> subset;



		unsigned long long int powerSetSize = (int)pow(2, stages.size());

		for (unsigned int i = 0; i < powerSetSize; i++) {

			bool isWithinThreshold = true;									// Terminates the loop if
																			// a certain subset goes out
																			// of bounds.

			for (unsigned int j = 0; j < stages.size() && isWithinThreshold; j++) {

				if (i & (1 << j)) {

					subset.push(stages[j]);
					isWithinThreshold = !(getCummulativeTime(subset) > timeToSpare);
				
				}

				if (!isWithinThreshold) {

					while (!subset.empty()) {
						subset.pop();
					}

				}

			}


			if (getCummulativePoints(subset) > getCummulativePoints(optimalChoice)) {

				if (!optimalChoice.empty() &&
					getCummulativeTime(subset) <= getCummulativeTime(optimalChoice)) {
					optimalChoice = subset;
				}
				else {
					optimalChoice = subset;
				}

			}


			while (!subset.empty()) {
				subset.pop();
			}


		}


		printStack(optimalChoice);



	}

	*/


	// Iterative Approach
	void printBestStage(stack<stack<Stage>> stageCombinations) {

		// Finding the optimal set of stages.

		stack <Stage> bestCombinationSoFar;								// Keeps the best choice so far

		while (!stageCombinations.empty()) {							// Do until the provided stack is empty.

			stack <Stage> currentEntry = stageCombinations.top();		// Take the top stack

			if (getCummulativePoints(currentEntry)						// Scan all of it's entries and
				> getCummulativePoints(bestCombinationSoFar)) {			// compare it with the best choice.

				bestCombinationSoFar = currentEntry;

			}

			stageCombinations.pop();

		}

		cout << "\n\nTIME CONSTRAINT: " << timeToSpare << endl;

		cout << "\n\nBEST CHOICE: " << endl;							// Print the best choice
		
		while (!bestCombinationSoFar.empty()) {							// from the provided stack.

			cout << bestCombinationSoFar.top() << endl;
			bestCombinationSoFar.pop();

		}

	}

	// Iterative Solution
	void findOptimalStagesIteratively() {

		/*

		This algorithm finds the optimal solution
		for the stage problem using an iterative
		approach.

		*/

		// Generating combinations.

		stack <stack<Stage>> stageCombinations;								// Holds all combinations

		unsigned int powerSetSize = (int)pow(2, stages.size());

		for (unsigned int i = 0; i < powerSetSize; i++) {

			stack <Stage> currentSet;										// Holds current subset.

			bool isWithinThreshold = true;									// Terminates the loop if
																			// a certain subset goes out
																			// of bounds.

			for (unsigned int j = 0; j < stages.size() && isWithinThreshold; j++) {

				if (i & (1 << j)) {
					currentSet.push(stages[j]);
					isWithinThreshold = !(getCummulativeTime(currentSet) > timeToSpare);
				}

			}

			if (isWithinThreshold) {
				stageCombinations.push(currentSet);							// Push the combination
																			// only if it is under time.
			}

		}

		printBestStage(stageCombinations);									// Printing the best possible stage.

	}

	// printing function
	template <typename T>
	void printStack(stack <T> s) {

		while (!s.empty()) {
			cout << s.top() << endl;
			s.pop();
		}

		cout << endl;

	}

	// Destructor
	~MindThrashing() {
		stages.resize(0);
		timeToSpare = 0;
	}

	

};


int main() {

	MindThrashing game;
	game.findOptimalStagesRecursively();
	game.findOptimalStagesIteratively();

	return 0;
}