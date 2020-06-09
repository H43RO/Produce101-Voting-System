#include "iVote.h"
#include <time.h>
using namespace std;

#define FIFO_VTOA_NAME "../res/my_fifo_vtoa"
#define FIFO_ATOV_NAME "../res/my_fifo_atov"

VoteManager v1("ProduceIoT");
Admin admin(&v1);

void run_admin(int sig){
	admin.run_admin();
	system("clear");
	cout << "Viewer input waiting" << endl;
}

int main() {
	srand((unsigned int)time(NULL));

	int input;

	FifoReadManager fr(FIFO_VTOA_NAME);
	fr.read_pid();
	FifoWriteManager fw(FIFO_ATOV_NAME);

	Viewer viewer(&v1, &fr, &fw);

	signal(SIGINT, run_admin);

	//-------------------------------reset----------------------------------
	
	cout << "iVote On" << endl;
	viewer.run_viewer();
	cout << "Server close" << endl;
	sleep(1);
}