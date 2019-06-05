#include<iostream>
#include<thread>
#include<vector>
#include<ctime>
#include<fstream>
#include<stdlib.h>
#include<sys/time.h>
#include<unistd.h>
#include<sys/syscall.h>
#include<sys/types.h>
#include<stdio.h>
#include<string>
//size of memory in terms of GB
//#define S 1*600
//size of 1MB
#define M 1024*1024*100
//default is 8
//number of threads
#define N 12


using namespace std;

int S = 10840;
char *BLOCK_1M;

void write(int tid, const char* file_name) {
	//char *BLOCK_1M;
	//BLOCK_1M = new char[M];
        //for (int i = 0; i < M; i++) {
        //        BLOCK_1M[i] = 'b';
        //}
        //BLOCK_1M[M-1] = '\0';
	cout << tid << " start write: " << file_name << endl;
	ofstream ofile;
	while (1) {
		ofile.open(file_name, ios::in | ios::out);
		for(int i = 0; i < 100; i++) {
			//ofile << *BLOCK_1M << endl;
			ofile.write(BLOCK_1M, M);
			//usleep(1000);
		}
		ofile.close();
	}
}

void _file_(int index) {

	//sieze of 1K pointer
	string file_name;
	file_name = "/home/user/memory-test/10G_" + to_string(index) + ".file";
	write(index, file_name.c_str());

	return;
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf ("please dpecify mb per core. using defalt 600MB\n");
	} else {
		S = atoi(argv[1]);
	}
	int pid = getpid();
	//int memcg_id=syscall(333,pid);
	std::cout << "pid: " << pid << endl;
	BLOCK_1M = new char[M];
	for (int i = 0; i < M; i++) {
		BLOCK_1M[i] = 'a';
	}
	BLOCK_1M[M-1] = '\0';

	//allocation
	timespec ts, te;
	clock_gettime(CLOCK_REALTIME, &ts);
	std::cout<<"start"<<std::endl;
	std::vector<thread> threads;
	for(int i = 1; i <= N; i++) {
		threads.push_back(thread(&_file_,i));
	}

	for(auto& th : threads){
		th.join();
	}
	clock_gettime(CLOCK_REALTIME, &te);
	std::cout << "finish " << double(te.tv_sec-ts.tv_sec) << std::endl;
	return 0;
}
