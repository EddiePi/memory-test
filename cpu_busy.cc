#include<iostream>
#include<thread>
#include<vector>
#include<ctime>
#include<stdlib.h>
#include<sys/time.h>
#include<unistd.h>
#include<sys/syscall.h>
#include<sys/types.h>
#include<stdio.h>
//size of memory in terms of GB
//#define S 1*600
//size of 1MB
//#define M 1024*1024
//default is 8
//number of threads
//#define N 16


using namespace std;


void allocate(int tid) {

	cout << tid << " start cpu busy" << endl;
	while(1) {
		//int a;
		//a = 1;	
	}
}


void _cpu_(int index) {

	//sieze of 1K pointer
	allocate(index);
	return;
}

int main(int argc, char *argv[]) {
	int N = 16;
	if (argc < 2) {
		printf("using default thread: %d\n", N);
	} else {
		N = atoi(argv[1]);
	}
	int pid = getpid();
	std::cout << "pid: " << pid << endl;

	//timespec ts, te;
	//clock_gettime(CLOCK_REALTIME, &ts);
	//std::cout<<"start"<<std::endl;
	std::vector<thread> threads;
	for(int i = 1; i <= N; i++) {
		threads.push_back(thread(&_cpu_,i));
	}

	for(auto& th : threads){
		th.join();
	}
	//clock_gettime(CLOCK_REALTIME, &te);
	//std::cout << "finish " << double(te.tv_sec-ts.tv_sec) << std::endl;
	return 0;
}
