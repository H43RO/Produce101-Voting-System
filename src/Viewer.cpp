#include "FifoManager.h"
using namespace std;

#define FIFO_VTOA_NAME "../res/my_fifo_vtoa"
#define FIFO_ATOV_NAME "../res/my_fifo_atov"

int output_available = 0;
void availale(int sig){
    output_available = 1;
}

void not_available(int sig){
    output_available = 0;
}

int main(){
    signal(SIGUSR1, availale);
    signal(SIGUSR2, not_available);
    FifoWriteManager fw(FIFO_VTOA_NAME);
    fw.write_pid();
    FifoReadManager fr(FIFO_ATOV_NAME);

    while(1){
        pause();
        system("clear");
        if(output_available == 1){
            cout << fr.get_buf() << endl;
            output_available = 0;
        }
        fw.write_buf();

    }

    return 0;
}