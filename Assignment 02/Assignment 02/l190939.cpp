#include <iostream>
#include <string>
#include <vector>

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
	
		out << ":: TASK DETAIL ::" << endl;
		out << "Name: " << S.name << endl;
		out << "Points: " << S.points << endl;
		out << "Time: " << S.time << endl;

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

};

int main() {

	MindThrashing game;

	return 0;
}