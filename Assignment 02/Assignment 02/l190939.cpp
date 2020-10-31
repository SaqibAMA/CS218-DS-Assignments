#include <iostream>
#include <string>
#include <vector>
#include <math.h>

#include <stack>

using namespace std;

class Stage {

//Stage class to keep data for every stage.

private:
	string name;
	unsigned int points;
	unsigned int time;

public:
	// Friendships
	friend class MindThrashing;

	// Constructor
	Stage(
		string str = "",
		unsigned int points = 0,
		unsigned int time = 0) {
	
		this->name = name;
		this->points = points;
		this->time = time;

	}

	Stage(const Stage& S) {

		this->name = S.name;
		this->points = S.points;
		this->time = S.time;

	}

	// Operators
	friend istream& operator >> (istream& in, Stage& S) {
	
		cout << "Name: ";
		in >> S.name;
		cout << "Points: ";
		in >> S.points;
		cout << "Time: ";
		in >> S.time;

		return in;

	}

	friend ostream& operator << (ostream& out, const Stage& S) {
	
		out << "[ Name: " << S.name << "; Points: "
			<< S.points << "; Time: " << S.time <<  "]";

		return out;

	}

	Stage& operator = (const Stage& S) {
		this->name = S.name;
		this->points = S.points;
		this->time = S.time;
	}

	// Destructor
	~Stage() {
		name = "";
		points = 0;
		time = 0;
	}

};

class MindThrashing {

// MindThrashing class with primary functionality.

private:
	vector <Stage> stages;			// Keeps all the stages.
	unsigned int timeToSpare;		// Keeps the time to spare.

public:
	MindThrashing() {
		getStages();
	}

	void getStages() {

		/*
		
		This function is responsible for taking
		the input of stages from the user.

		*/

		unsigned int totalStages;
		cout << "Total Stages: ";
		cin >> totalStages;

		stages.resize( (size_t) totalStages);

		cout << "Enter time to spare: ";
		cin >> timeToSpare;

		for (int i = 0; i < totalStages; i++) {
			cout << endl;
			cin >> stages[i];
			cout << endl;
		}


	}

	// Iterative Solution

	unsigned int getCummulativeTime(stack <Stage> stageCombination) {
	
		unsigned int totalTime = 0;

		while (!stageCombination.empty()) {
			totalTime += stageCombination.top().time;
			stageCombination.pop();
		}

		return totalTime;

	}

	void findOptimalStagesIteratively() {

		/*
		
		This algorithm finds the optimal solution
		for the stage problem using an iterative
		approach.

		*/

		stack <stack<Stage>> stageCombinations;

		unsigned int powerSetSize = (int)pow(2, stages.size());

		for (int i = 0; i < powerSetSize; i++) {

			stack <Stage> currentSet;
			bool isWithinThreshold = true;

			for (int j = 0; j < stages.size() && isWithinThreshold; j++) {

				if (i & (1 << j)) {

					currentSet.push(stages[j]);

					if (getCummulativeTime(currentSet) > timeToSpare) {
						isWithinThreshold = false;
					}
					
				}

			}

			if (isWithinThreshold) {
				stageCombinations.push(currentSet);
			}

		}


		cout << "\n\n\n\n\n";


		while (!stageCombinations.empty()) {
			
			stack <Stage> temp = stageCombinations.top();
			
			while (!temp.empty()) {
				cout << temp.top() << ",";
				temp.pop();
			}

			cout << endl;

			stageCombinations.pop();

		}

	}

};

int main() {

	MindThrashing game;

	game.findOptimalStagesIteratively();

	return 0;
}