#pragma once
#ifndef STAR_H
#define STAR_H

#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

class Star {
	string name, company;
	int age, votes, votes_pre, votes_increase;
	vector<string> board;
	vector<string>::iterator it_board;

public:
	Star(string name, int age, string company, int votes, int votes_pre, int votes_increas);

	//-----Get Function-----
	string get_name();

	int get_age();

	string get_company();

	int get_votes();

	int get_pre();

	int get_increase();

	vector<string> get_board();

	//-----Add Function-----
	void add_votes();

	void add_rand_votes();

	//-----Set Function-----
	void set_votes(int vote);

	void set_increase(int increase);

	void set_pre(int pre);

	void set_board(vector<string> new_board);

	//-----Other Function-----

	friend bool vote_cmp(Star s1, Star s2);
};

bool vote_cmp(Star s1, Star s2);

#endif