#include "Star.h"

Star::Star(string name, int age, string company, int votes = 0, int votes_pre = 0, int votes_increase = 0) {
	this->name = name;
	this->age = age;
	this->company = company;
	this->votes = votes;
	this->votes_pre = votes_pre;
	this->votes_increase = votes_increase;
}

//-----Get Function-----
string Star::get_name() { return name; }
int Star::get_age() { return age; }
string Star::get_company() { return company; }
int Star::get_votes() { return votes; }
int Star::get_increase() { return votes_increase; }
int Star::get_pre() { return votes_pre; }
vector<string> Star::get_board() { return board; }

//-----Add Function-----
void Star::add_votes() { this->votes++; }

void Star::add_rand_votes() { //Random Voting System
	int rand_votes = rand() % 1000;
	this->votes += rand_votes;

	if (votes_pre == 0) {
		this->votes_pre = rand_votes;
	}
	else {
		int increase = 0;
		increase = this->votes - this->votes_pre;
		this->votes_increase = increase;
	}
	this->votes_pre = this->votes;
}

//-----Set Function-----
void Star::set_votes(int vote) { this->votes = vote; }
void Star::set_increase(int increase) { this->votes_increase = increase; }
void Star::set_pre(int pre) { this->votes_pre = pre; }
void Star::set_board(vector<string> new_board) { this->board = new_board; }

//-----Other Function-----
bool vote_cmp(Star s1, Star s2) {
	return s1.get_votes() > s2.get_votes();
}