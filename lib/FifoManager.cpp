#include "FifoManager.h"

//-----FifoRead-----
FifoReadManager::FifoReadManager(const char* fifo_name){
    if( access(fifo_name, F_OK) == -1 ) {
        int res = mkfifo(fifo_name, 0777);
        if( res != 0){
            return;
        }
    }
	this->fifo_name = fifo_name;
	cout << "Waiting Fifo Write" << endl;
	fd = open(fifo_name, O_RDONLY);
}

char* FifoReadManager::get_buf(){
    int result = read(fd,buf,BUF_SIZE);
    if(result == 0){
        return NULL;
    }
    //cout << buf << endl;
    return buf;
}

void FifoReadManager::stop_fifo(){
	cout << endl;
	while (1) {
	//cout << " q : Exit >> " << endl;
    kill(c_pid, SIGUSR2);
	read(fd,buf,BUF_SIZE);
		if (buf[0] == 'q') {
			break;
		}
	}
}

void FifoReadManager::input_fifo(int& input){
	read(fd, buf, BUF_SIZE);
	input = stoi(buf);
}

void FifoReadManager::read_pid(){
    read(fd,&c_pid, sizeof(c_pid));
}

pid_t FifoReadManager::get_c_pid(){
    return c_pid;
}
//-----FifoWrite-----
FifoWriteManager::FifoWriteManager(const char* fifo_name) {
    if( access(fifo_name, F_OK) == -1 ) {
        int res = mkfifo(fifo_name, 0777);
        if( res != 0){
            return;
        }
    }
	this->fifo_name = fifo_name;
	cout << "Waiting Fifo Read" << endl;
	fd = open(fifo_name, O_WRONLY);
    my_pid = getpid();
}

int FifoWriteManager::write_buf(){
    printf("[Viewer] Input : ");
    fgets(buf, BUF_SIZE, stdin);
    write(fd, buf, BUF_SIZE);
}

void FifoWriteManager::write_str(char* str){
    write(fd,str,BUF_SIZE);
}

void FifoWriteManager::write_pid(){
    write(fd, &my_pid, sizeof(my_pid));
}