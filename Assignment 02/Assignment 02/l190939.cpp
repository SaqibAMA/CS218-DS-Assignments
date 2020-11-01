#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <ctime>

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
	bool empty() {
		return !front;							// If front is at zero.
	}

	// Check Full
	bool full() {
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


	// Assignment Operator
	stack& operator = (const stack& s) {

		this->front = s.front;
		this->data.resize(s.data.size());

		this->data = s.data;

		return *this;

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
			<< S.points << "; Time: " << S.time <<  "]";

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

// Mindthrashing class to hold all the functionality.
class MindThrashing {

// MindThrashing class with primary functionality.

private:
	vector <Stage> stages;			// Keeps all the stages.
	unsigned int timeToSpare;		// Keeps the time to spare.

	// -- graphics related --

	unsigned int topMargin;			// top margin
	unsigned int leftMargin;		// left margin

public:


	// Constructor
	MindThrashing() {
		topMargin = 5;
		leftMargin = 30;
		setupScreen();
		
		
		getStages();									// Starts the program by taking input.


		clock_t begin = clock();
		
		findOptimalStagesIteratively();
		
		SetConsoleTextAttribute(h, 4 | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY);
		gotoxy(3, topMargin + 1);
		cout << "ITER: " << (float)(clock() - begin) / 1000 << "s." << endl;
		begin = clock();


		findOptimalStagesRecursively();


		gotoxy(3, topMargin + 5);
		cout << "REC: " << (float)(clock() - begin) / 1000 << "s." << endl;
		gotoxy(0, 20);
		SetConsoleTextAttribute(h, 15);


	}
	
	// Input function
	void getStages() {

		/*
		
		This function is responsible for taking
		the input of stages from the user.

		*/

		unsigned int totalStages;						// Taking input for total stages.
		gotoxy(leftMargin + 1, topMargin + 1);
		cout << "STAGES: ";
		cin >> totalStages;

		stages.resize( (size_t) totalStages);			// We declare the vector
														// size initially so that the
														// backend cost is less while pushing.

		gotoxy(leftMargin + 1, topMargin + 1);
		cout << "TIME TO SPARE: ";						// Input for total time to spare.
		cin >> timeToSpare;

		setupScreen();

		for (unsigned int i = 0; i < totalStages; i++) {			// Actual input loop.
			
			gotoxy(leftMargin + 1, topMargin + 1);
			cout << "NAME FOR #" << i + 1 << " : ";
			cin >> stages[i].name;

			setupScreen();
			
			gotoxy(leftMargin + 1, topMargin + 1);
			cout << "TIME FOR #" << i + 1 << " : ";
			cin >> stages[i].time;

			setupScreen();

			gotoxy(leftMargin + 1, topMargin + 1);
			cout << "PTS FOR  #" << i + 1 << " : ";
			cin >> stages[i].points;

			setupScreen();

		}


	}

	/*
	This function takes a stack and then finds out the best
	combination of stages that exists within it.
	*/
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

		gotoxy(leftMargin + 1, topMargin + 1);
		cout << "TIME CONSTRAINT: " << timeToSpare << endl;

		gotoxy(leftMargin + 1, topMargin + 2);
		cout << "BEST CHOICE: " << endl;								// Print the best choice

		gotoxy(leftMargin, topMargin + 3);
		cout << "                                               " << endl;

		unsigned int currEntry = 5;

		while (!bestCombinationSoFar.empty()) {							// from the provided stack.

			gotoxy(leftMargin, topMargin + currEntry);
			cout << "                                               " << endl;
			gotoxy(leftMargin, topMargin + currEntry + 1);
			cout << "                                               " << endl;
			gotoxy(leftMargin + 1, topMargin + currEntry + 1);
			cout << bestCombinationSoFar.top() << endl;
			gotoxy(leftMargin, topMargin + currEntry + 2);
			cout << "                                               " << endl;

			bestCombinationSoFar.pop();

			currEntry += 4;

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

	// Recursive Solution
	void findOptimalStagesRecursively() {
	
		vector <Stage> subset(stages.size());										// This is a buffer subset
																					// vector that aids the recursive
																					// function.

		stack <stack <Stage>> stageCombinations;									// Holds all the subsets.

		generateSubsets(stages, stages.size(), subset, 0, 0, stageCombinations);	// This function recursively
																					// generates all subsets.
		
		printBestStage(stageCombinations);											// Prints the best possible stage
																					// combination.


	}
	void generateSubsets(vector <Stage> stages,
						unsigned int size,
						vector <Stage> subset,
						unsigned int setIndex,
						unsigned int subIndex,
						stack <stack <Stage>> & stageCombinations) {

		/*
		
		This function recursively generates the subsets
		of the provided stages.

		*/

		if (setIndex == size) {																		// Base case: we have 
																									// traversed all elements.
			
			stack <Stage> currCombination;

			for (unsigned int i = 0; i < subIndex; i++) {													// Convert vector into stack.
				currCombination.push(subset[i]);
			}

			if (getCummulativeTime(currCombination) <= timeToSpare) {								// Push the current combination.
				stageCombinations.push(currCombination);
			}

		}
		else {


			subset[subIndex] = stages[setIndex];													// Recursive case.
			
			if (getCummulativeTime(subset, subIndex) > timeToSpare) return;

			generateSubsets(stages, size, subset, setIndex + 1, subIndex + 1, stageCombinations);	// Include the element.
			generateSubsets(stages, size, subset, setIndex + 1, subIndex, stageCombinations);		// Exclude the element.

		}

	}


	// Destructor
	~MindThrashing() {
		stages.resize(0);
		timeToSpare = 0;
	}

	// Graphics Related
	void setupScreen() {

		int deviation = 0;

		SetConsoleTextAttribute(h, 1 | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY);

		gotoxy(leftMargin, topMargin + deviation);
		cout << "                                               " << endl;
		gotoxy(leftMargin, topMargin + deviation + 1);
		cout << "                                               " << endl;
		gotoxy(leftMargin, topMargin + deviation + 2);
		cout << "                                               " << endl;

		for (int i = 0; i < 3; i++) {

			gotoxy(2, topMargin + i);
			cout << "                  ";

			gotoxy(2, topMargin + i + 4);
			cout << "                  ";

		}


	}

};


int main() {

	MindThrashing game;

	return 0;
}