#include "iVote.h"

VoteManager::VoteManager(string title) {
	this->title = title;
	this->days = 0;
	
	filename = title;
	filename.append("_star_data.txt");

	datafile_in.open(filename, ios::in);
	if (!datafile_in) {
		cout << "Can't open file" << endl;
		refresh_iVote();
	}
	else {
		load_data();
		if (days % 7 == 0) {
			mid_result();
		}
	}
	datafile_in.close();
}

VoteManager::~VoteManager() {
	save_data();
	datafile_in.close();
	datafile_out.close();
}

//-----Show Function-----
string VoteManager::show_star(int number) {
	if (number > (int)star_list.size() || 0 > number) {
		//cout << "Number Error ( Current number of star : " << star_list.size() << " )" << endl;
		output_str = "Number Error ( Current number of star : " + to_string(star_list.size()) + ")\n";
	}
	else {
		Star info = star_list[number - 1];
		//cout << endl << "===============" << info.get_name() << "'s Data===============" << endl;
		//cout << "[Name]\t[Age]\t[Company]\t[Votes]" << endl;
		//cout << info.get_name() << "\t" << info.get_age() << "\t" << info.get_company() << "\t\t" << info.get_votes() << endl;
		output_str = "===============" + info.get_name() + "'s Data===============\n";
		output_str.append("[Name]\t[Age]\t[Company]\t[Votes]\n");
		output_str.append(info.get_name() + "\t" + to_string(info.get_age()) + "\t" + info.get_company() + "\t\t" + to_string(info.get_votes()) + "\n");
	}
	return output_str;
}

string VoteManager::show_list() {
	//cout << "day : " << days << endl;
	output_str = "day : " + to_string(days) + "\n";

	//cout << "[Num]\t[Name]\t[Votes]" << endl;
	output_str.append("[Num]\t[Name]\t[Votes]\n");
	for (int i = 0; i < (int)star_list.size(); i++) {
		//cout << i + 1 << "\t" << star_list[i].get_name() << "\t" << star_list[i].get_votes() << endl;
		output_str.append( to_string(i+1) + "\t");
		output_str.append(star_list[i].get_name() + "\t");
		output_str.append( to_string(star_list[i].get_votes()) + "\n");
	}
	return output_str;
}

string VoteManager::show_rank() {
	sort_votes();
	//cout << "day : " << days << endl;
	//cout << "[Num]\t[Name]\t[Votes]" << endl;

	output_str = "day : " + to_string(days) + "\n";
	output_str.append("[Num]\t[Name]\t[Votes]\n");

	for (int i = 0; i < (int)star_rank.size(); i++) {
		//cout << i + 1 << "\t" << star_rank[i].get_name() << "\t" << star_rank[i].get_votes() << endl;
		output_str.append( to_string(i+1) + "\t");
		output_str.append(star_rank[i].get_name() + "\t");
		output_str.append( to_string(star_rank[i].get_votes()) + "\n");
	}
	return output_str;
}

void VoteManager::show_board_admin(int number) {
	if (number > (int)star_list.size() || 0 > number) {
		cout << "Number Error ( Current number of star : " << star_list.size() << " )" << endl;
	}
	else {
		vector<string> board = star_list[number-1].get_board();
		string text, name = star_list[number-1].get_name();
		int mode;
		cout << "=====" << name << "'s Board=====" << endl;
		if (board.size() == 0) {
			cout << endl << "[ Board Empty ]" << endl;
		}

		else {
			cout << endl;
			for (int i = 0; i < (int)board.size(); i++) {
				cout << "[ " << i + 1 << " ] " << board[i] << endl << endl;
			}
		}

		cout << "=========================================" << endl;
		cout << "1. Writing     2. Exit" << endl;
		cout << "Input >> ";
		cin >> mode;
		cin.ignore();

		switch (mode) {
		case 1:
			cout << endl << "============Input Text============" << endl;
			getline(cin, text);
			board.push_back(text);
			cout << endl << "============Upload Completed============" << endl;
			for (int i = 0; i < (int)board.size(); i++) {
				cout << "[ " << i + 1 << " ] " << board[i] << endl << endl;
			}
			break;
		case 2:
			break;
		default:
			cout << "Input Error." << endl;
			break;
		}
		star_list[number-1].set_board(board);
		board.clear();
	}
}

void VoteManager::show_board_viewer(int number, FifoReadManager* fr, FifoWriteManager* fw) {
	if (number > (int)star_list.size() || 0 > number) {
		//cout << "Number Error ( Current number of star : " << star_list.size() << " )" << endl;
		output_str = "Number Error ( Current number of star : " + to_string(star_list.size()) + " )\n";
		strcpy(output_buf, output_str.c_str());
		fw->write_str(output_buf);
		kill(fr->get_c_pid(), SIGUSR1);
	}
	else {
		vector<string> board = star_list[number-1].get_board();
		string text, name = star_list[number-1].get_name();
		int mode;
		output_str = "=====" + name + "'s Board=====\n";
		if (board.size() == 0) {
			output_str.append("\n[ Board Empty ]\n");
		}

		else {
			output_str.append("\n");
			for (int i = 0; i < (int)board.size(); i++) {
				output_str.append("[ " + to_string(i+1) + " ] " + board[i] + "\n");
			}
		}
		output_str.append("=========================================\n");
		output_str.append("1. Writing     2. Exit\n");
		strcpy(output_buf, output_str.c_str());
		fw->write_str(output_buf);
		kill(fr->get_c_pid(), SIGUSR1);
		cout << "Viewer input waiting" << endl;
		fr->input_fifo(mode);

		switch (mode) {
		case 1:
			output_str = "============Input Text============\n";
			strcpy(output_buf, output_str.c_str());
			fw->write_str(output_buf);
			kill(fr->get_c_pid(), SIGUSR1);

			text = fr->get_buf();
			text.erase(text.length()-1);
			board.push_back(text);

			output_str = "============Upload Completed============\n";
			for (int i = 0; i < (int)board.size(); i++) {
				output_str.append("[ " + to_string(i+1) + " ] " + board[i] + "\n");
			}
			output_str.append("\n q : Exit\n");
			strcpy(output_buf, output_str.c_str());
			fw->write_str(output_buf);
			kill(fr->get_c_pid(), SIGUSR1);
			break;
		case 2:
			output_str = " q : Exit\n";
			strcpy(output_buf, output_str.c_str());
			fw->write_str(output_buf);
			kill(fr->get_c_pid(), SIGUSR1);
			break;
		default:
			output_str = "Input Error\n";
			strcpy(output_buf, output_str.c_str());
			fw->write_str(output_buf);
			kill(fr->get_c_pid(), SIGUSR1);
			break;
		}
		star_list[number-1].set_board(board);
		board.clear();
	}
}

//-----Vote Function-----
string VoteManager::voting_star(int number) {
	if (number > (int)star_list.size() || 0 > number) {
		//cout << "Number Error ( Current number of star : " << star_list.size() << " )" << endl;
		output_str = "Number Error ( Current number of star : " + to_string(star_list.size()) + ")\n";
	}
	else {
		//cout << "Vote " << number << " Completed." << endl;
		output_str = "Vote " + to_string(number) + " Completed\n";
		star_list[number - 1].add_votes();
	}
	return output_str;
}

void VoteManager::voting_rand_star() { //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	int set_day = this->days % 7;
	set_day = 7 - set_day;
	this->days += set_day;
	for (int i = 0; i < (int)star_list.size(); i++) {
		star_list[i].add_rand_votes();
	}
}

//-----Add/Delete Function-----
void VoteManager::add_star(Star new_star) {
	star_list.push_back(new_star);
}

void VoteManager::delete_star(int number) {
	if (number > (int)star_list.size() || 0 > number) {
		cout << "Number Error ( Current number of star : " << star_list.size() << " )" << endl;
	}
	else {
		cout << number << " " << star_list[number - 1].get_name() << " Deleted." << endl;
		it_star = star_list.begin();
		star_list.erase(it_star + (number - 1));
	}
}

//-----Other Function-----
void VoteManager::sort_votes() {
	star_rank = star_list;
	sort(star_rank.begin(), star_rank.end(), vote_cmp);
}

string VoteManager::rising_star() {
	if (star_list.size() == 0) {
		//cout << "There are no stars" << endl;
		output_str = "There are no stars\n";
	}
	else{
		int i, rising_index;
		rising_index = 0;
		for (i = 0; i < (int)star_list.size(); i++) {
			if (star_list[rising_index].get_increase() < star_list[i].get_increase()) {
				rising_index = i;
			}
		}
		//cout << "==================================================\n\n";
		//cout << "\t    The Rising star is " << star_list[rising_index].get_name() << " !\n\n";
		//cout << "==================================================\n\n";
		output_str = "==================================================\n\n";
		output_str.append("\t    The Rising star is " + star_list[rising_index].get_name() + " !\n\n");
		output_str.append("==================================================\n\n");
	}
	return output_str;
}

void VoteManager::mid_result() {
	int result_vote_a = 0;
	int result_vote_b = 0;
	if ((int)star_list.size() == 0) {
		cout << "Star empty" << endl;
	}
	else {
		for (int i = 0; i < (int)star_list.size(); i++) {
			result_vote_a = star_list[i].get_votes();
			result_vote_b = star_list[i].get_pre();
			star_list[i].set_increase(result_vote_a - result_vote_b);
			star_list[i].set_pre(result_vote_a);
		}
	}
}

//-----Save/Load Function-----
void VoteManager::save_data() {
	filename = title;
	filename.append("_star_data.txt");
	datafile_out.open(filename, ios::out);

	//-----Save Star Data-----
	for (int i = 0; i < (int)star_list.size(); i++) {
		datafile_out << star_list[i].get_name() << endl;
		datafile_out << star_list[i].get_age() << endl;
		datafile_out << star_list[i].get_company() << endl;
		datafile_out << star_list[i].get_votes() << endl;
		datafile_out << star_list[i].get_increase() << endl;
		datafile_out << star_list[i].get_pre() << endl;
	}

	//-----Save Star Board-----
	vector<string> star_board;
	for (int i = 0; i < (int)star_list.size(); i++) {
		filename = "board_";
		filename.append(star_list[i].get_name());
		filename.append(".txt");
		datafile_out.close();
		datafile_out.open(filename, ios::out);
		star_board = star_list[i].get_board();
		for (int i = 0; i < (int)star_board.size(); i++) {
			datafile_out << star_board[i] << endl;
		}
	}
	datafile_out.close();
	//-----Save Days-----
	this->days++;
	datafile_out.open("days.txt", ios::out);
	datafile_out << this->days << endl;
	datafile_out.close();
}

void VoteManager::load_data() {
	int line_num = 1;
	cout << "-----Load Start-----" << endl;

	string read, text, name, company;
	int age, votes, increase, pre;
	ifstream boardfile_in;
	vector<string> new_board;

	//-----Load Star Data-----
	while (getline(datafile_in, read)) {
		switch (line_num)
		{
		case 1:
			name = read; break;
		case 2:
			age = stoi(read); break;
		case 3:
			company = read; break;
		case 4:
			votes = stoi(read); break;
		case 5:
			increase = stoi(read); break;
		case 6:
			pre = stoi(read); break;
		default:
			break;
		}

		if (line_num == 6) {
			Star new_star(name, age, company,votes,pre,increase);
			//-----Load Star Board-----
			filename = "../Board_data/board_";
			filename.append(name);
			filename.append(".txt");
			boardfile_in.open(filename, ios::in);
			while (getline(boardfile_in, text)) {
				new_board.push_back(text);
			}
			boardfile_in.close();
			new_star.set_board(new_board);
			new_board.clear();

			//-----Add Star-----
			add_star(new_star);
			line_num = 0;
		}
		line_num++;
	}
	datafile_in.close();
	//-----Add Days-----
	datafile_in.open("days.txt", ios::in);
	if (datafile_in) {
		getline(datafile_in, read);
		this->days = stoi(read);
	}
	datafile_in.close();
	refresh_iVote();
	cout << "-----Load Completion-----" << endl;
	refresh_iVote();
}

//--------------------------------------------------Viewer-------------------------------------------------

Viewer::Viewer(VoteManager* vote, FifoReadManager* fr = NULL, FifoWriteManager* fw = NULL) {
	this->vote = vote;
	this->fr = fr;
	this->fw = fw;
}

string Viewer::voting_star_viewer(int number) {

	return vote->voting_star(number);

}

string Viewer::show_list_viewer() {

	return vote->show_list();

}

string Viewer::show_rank_viewer() {
	return vote->show_rank();
}

string Viewer::show_star_viewer(int number) {

	return vote->show_star(number);

}

void Viewer::goto_board(int number) {

	vote->show_board_viewer(number, fr, fw);

}

string Viewer::show_rising_star() {
	return vote->rising_star();
}


void Viewer::run_viewer() {
	char output_buf[BUF_SIZE];
	int input;
	string output_str;
	//cout << "Login Viewer." << endl;
	while (1) {
		refresh_iVote();
		/*
		cout << "===== Viewer Mode =====" << endl;
		cout << "1: Show Star List" << endl;
		cout << "2: Show Star Date" << endl;
		cout << "3: Show Star Rank" << endl;
		cout << "4: Vote Star" << endl;
		cout << "5: Star Board" << endl;
		cout << "6: Show Rising Star" << endl;
		cout << "0: Exit" << endl;
		cout << endl << "Input >> ";
		*/

		output_str = "===== Viwer Mode =====\n";
		output_str.append("1: Show Star List\n");
		output_str.append("2: Show Star Data\n");
		output_str.append("3: Show Star Rank\n");
		output_str.append("4: Vote Star\n");
		output_str.append("5: Star Board\n");
		output_str.append("6: Show Rising Star\n");
		output_str.append("0: Exit\n");
		strcpy(output_buf,output_str.c_str());
		kill(fr->get_c_pid(), SIGUSR1);
		fw->write_str(output_buf);

		cout << "Viewer input waiting" << endl;
		this->fr->input_fifo(input);
		if (input == 0) {
			kill(fr->get_c_pid(), SIGINT);
			break;
		}

		switch (input)
		{
		case 1:
			refresh_iVote();
			output_str = show_list_viewer();
			output_str.append("\n q: Exit");
			strcpy(output_buf,output_str.c_str());
			fw->write_str(output_buf);
			kill(fr->get_c_pid(), SIGUSR1);
			cout << "Viewer input waiting" << endl;
			this->fr->stop_fifo();
			break;

		case 2:
			refresh_iVote();
			output_str = show_list_viewer();
			output_str.append("\nInput Star Number :");
			strcpy(output_buf,output_str.c_str());
			fw->write_str(output_buf);
			kill(fr->get_c_pid(), SIGUSR1);
			cout << "Viewer input waiting" << endl;
			this->fr->input_fifo(input);
			output_str = show_star_viewer(input);
			output_str.append("\n q : Exit\n");
			strcpy(output_buf,output_str.c_str());
			fw->write_str(output_buf);
			kill(fr->get_c_pid(), SIGUSR1);
			cout << "Viewer input waiting" << endl;
			this->fr->stop_fifo();
			break;

		case 3:
			refresh_iVote();
			output_str = show_rank_viewer();
			output_str.append("\n q : Exit\n");
			strcpy(output_buf,output_str.c_str());
			fw->write_str(output_buf);
			kill(fr->get_c_pid(), SIGUSR1);
			cout << "Viewer input waiting" << endl;
			this->fr->stop_fifo();
			break;

		case 4:
			refresh_iVote();
			output_str = show_list_viewer();
			strcpy(output_buf,output_str.c_str());
			fw->write_str(output_buf);
			kill(fr->get_c_pid(), SIGUSR1);
			cout << "Viewer input waiting" << endl;
			this->fr->input_fifo(input);

			output_str = voting_star_viewer(input);
			output_str.append("\n q >> Exit\n");
			strcpy(output_buf,output_str.c_str());
			fw->write_str(output_buf);
			kill(fr->get_c_pid(), SIGUSR1);
			cout << "Viewer input waiting" << endl;
			this->fr->stop_fifo();
			break;

		case 5:
			refresh_iVote();
			output_str = show_list_viewer();
			output_str.append("\nInput Star Number :");
			strcpy(output_buf,output_str.c_str());
			fw->write_str(output_buf);
			kill(fr->get_c_pid(), SIGUSR1);
			cout << "Viewer input waiting" << endl;
			this->fr->input_fifo(input);
			goto_board(input);
			cout << "Viewer input waiting" << endl;
			this->fr->stop_fifo();
			break;

		case 6:
			refresh_iVote();
			output_str = show_rising_star();
			output_str.append("\n q : Exit\n");
			strcpy(output_buf,output_str.c_str());
			fw->write_str(output_buf);
			kill(fr->get_c_pid(), SIGUSR1);
			cout << "Viewer input waiting" << endl;
			this->fr->stop_fifo();
			break;

		default:
			/*
			output_str = "Input Error.\n";
			strcpy(output_buf,output_str.c_str());
			fw->write_str(output_buf);
			kill(fr->get_c_pid(), SIGUSR1);
			*/
			break;
		}
	}
}


//--------------------------------------------------Admin-------------------------------------------------
Admin::Admin(VoteManager* vote) : Viewer(vote) {
	passward_admin = "0000";
	passward_file_name = "passward_admin.txt";
}

Admin::~Admin() {
}
void Admin::add_star_admin(string name, int age, string company) {

	Star new_star(name, age, company, 0, 0, 0);

	vote->add_star(new_star);

	cout << "Add Star Completed" << endl;
}

void Admin::goto_board_admin(int number){
	vote->show_board_admin(number);
}

void Admin::delete_star_admin(int number) {

	vote->delete_star(number);

	cout << "Delete Star Completed" << endl;
}

void Admin::add_random_votes_admin() {
	vote->voting_rand_star();
}
void Admin::run_admin() {
	string passward;

	passward_io.open(passward_file_name, ios::in);
	if (passward_io) {
		getline(passward_io, passward_admin);
	}
	passward_io.close();

	cout << "Input Passward : ";
	getline(cin, passward);
	if (passward.compare(passward_admin) != 0) {
		cout << "Passward Error" << endl;
		sleep_iVote(1);
		return;
	}
	int input, age;
	string name, company, pwd;
	cout << "Login Admin." << endl;
	while (1) {
		refresh_iVote();
		cout << "===== Admin Mode =====" << endl;
		cout << "1: Show Star List" << endl;
		cout << "2: Show Star Date" << endl;
		cout << "3: Show Star Rank" << endl;
		cout << "4: Vote Star" << endl;
		cout << "5: Star Board" << endl;
		cout << "6: Rising star" << endl;
		cout << "7: Add Star" << endl;
		cout << "8: Delete Star" << endl;
		cout << "9: Random votes star" << endl;
		cout << "10: Change passward" << endl;
		cout << "0: Exit" << endl;
		cout << endl << "Input >> ";
		cin >> input;
		cin.ignore();
		cin.clear();
		if (input == 0) {
			passward_io.open(passward_file_name, ios::out);
			passward_io << passward_admin << endl;
			passward_io.close();
			break;
		}
		switch (input)
		{
		case 1:
			refresh_iVote();
			cout << show_list_viewer() << endl;
			stop_iVote();
			break;

		case 2:
			refresh_iVote();
			cout << show_list_viewer() << endl;
			cout << "Input Star Number : ";
			cin >> input;
			cin.ignore();
			cout << show_star_viewer(input) << endl;
			stop_iVote();
			break;
		case 3:
			refresh_iVote();
			cout << show_rank_viewer() << endl;
			stop_iVote();
			break;
		case 4:
			refresh_iVote();
			cout << show_list_viewer() << endl;
			cout << "Input Star Number : ";
			cin >> input;
			cin.ignore();
			cout << voting_star_viewer(input) << endl;
			sleep_iVote(1);
			refresh_iVote();
			cout << show_list_viewer() << endl;
			stop_iVote();
			break;

		case 5:
			refresh_iVote();
			cout << show_list_viewer() << endl;
			cout << "Input Star Number : ";
			cin >> input;
			cin.ignore();
			goto_board_admin(input);
			stop_iVote();
			break;

		case 6:
			refresh_iVote();
			cout << show_rising_star() << endl;
			stop_iVote();
			break;

		case 7:
			refresh_iVote();
			cout << "Input Star Name : ";
			getline(cin, name);
			cout << "Input Star Age : ";
			cin >> age;
			cin.ignore();
			cout << "Input Star Company : ";
			getline(cin, company);
			add_star_admin(name, age, company);
			stop_iVote();
			break;

		case 8:
			refresh_iVote();
			cout << show_list_viewer() << endl;
			cout << "Input Star Number : ";
			cin >> input;
			cin.ignore();
			delete_star_admin(input);
			stop_iVote();
			break;

		case 9:
			refresh_iVote();
			cout << show_list_viewer() << endl;
			sleep_iVote(1);
			add_random_votes_admin();
			refresh_iVote();
			cout << show_list_viewer() << endl;
			stop_iVote();
			break;

		case 10:
			cout << "Input passward to change : ";
			getline(cin, pwd);
			change_passward(pwd);
			cout << "Change passward Completed" << endl;
			break;
		default:
			cout << "Input Error." << endl;
			refresh_iVote();
			break;
		}
	}
}

void Admin::change_passward(string input) {
	this->passward_admin = input;
}

//-----Other Function-----
void refresh_iVote() {
	system("clear");
}

void stop_iVote() {
	string input;
	cout << endl;
	while (1) {
		cout << " q : Exit >> ";
		getline(cin, input);
		cin.clear();
		if (input == "q") {
			break;
		}
	}
}

void sleep_iVote(int time = 0) {
	sleep(time);
}