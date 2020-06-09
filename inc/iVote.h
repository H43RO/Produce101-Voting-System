
#ifndef IVOTE_H
#define IVOTE_H
#include <fstream>
#include <algorithm>
#include <unistd.h>
#include "Star.h"
#include "FifoManager.h"
#include <time.h>

#define BUF_SIZE 1024

//------VoteManger-----
class VoteManager {
	vector<Star> star_list;
	vector<Star>::iterator it_star;
	vector<Star> star_rank;
	string title;
	string filename;
	ofstream datafile_out;
	ifstream datafile_in;
	int days;
	string output_str;
	char output_buf[BUF_SIZE];
public:

	VoteManager(string title);

	~VoteManager();

	//-----Show Function-----
	string show_star(int number);

	string show_list();

	string show_rank();

	void show_board_admin(int number);

	void show_board_viewer(int number, FifoReadManager* fr, FifoWriteManager* fw);

	//-----Vote Function-----
	string voting_star(int number);

	void voting_rand_star();

	//-----Add/Delete Function-----
	void add_star(Star new_star);

	void delete_star(int number);

	//-----Other Function-----
	void sort_votes();

	string rising_star();

	void mid_result();

	//-----Save/Load Function-----
	void save_data();

	void load_data();

};

//-----Viewer-----
class Viewer {
protected:
	VoteManager* vote;
	FifoReadManager *fr;
	FifoWriteManager *fw;
public:
	Viewer(VoteManager* vote, FifoReadManager* fr, FifoWriteManager* fw);

	string voting_star_viewer(int number);

	string show_list_viewer();

	string show_rank_viewer();

	string show_star_viewer(int number);

	void goto_board(int number);

	void run_viewer();

	string show_rising_star();
};

//-----Admin-----
class Admin : public Viewer {
	string passward_admin;
	fstream passward_io;
	string passward_file_name;
public:

	Admin(VoteManager* vote);

	~Admin();

	void goto_board_admin(int number);
	
	void add_star_admin(string name, int age, string company);

	void delete_star_admin(int number);

	void add_random_votes_admin();

	void run_admin();

	void change_passward(string input);
};


//-----Other Function-----
void refresh_iVote();

void stop_iVote();

void sleep_iVote(int time);

#endif