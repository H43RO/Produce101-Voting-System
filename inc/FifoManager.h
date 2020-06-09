#ifndef FIFOMANAGER_H
#define FIFOMANAGER_H

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <string.h>

using namespace std;

#define BUF_SIZE 1024

class FifoReadManager {
    int fd;
    char buf[BUF_SIZE];
    string fifo_name;
    int result;
    pid_t c_pid;
public:
    FifoReadManager(const char* fifo_name);

    char* get_buf();

    void stop_fifo();

	void input_fifo(int& input);

    void read_pid();

    pid_t get_c_pid();
};

class FifoWriteManager{
    int fd;
    char buf[BUF_SIZE];
    string fifo_name;
    pid_t my_pid;
public:
    FifoWriteManager(const char* fifo_name);

    int write_buf();

    void write_str(char* str);

    void write_pid();
};

#endif