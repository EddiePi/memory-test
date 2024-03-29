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
#define M 1024*1024
//default is 8
//number of threads
#define N 24


using namespace std;

int S = 600;

void allocate(int tid, char** p) {

cout << tid << " start allocate" << endl;

	for(int i = 0; i < S; i++) {

		p[i] = new char[M];
		for(int j = 0; j < M; j++){
			p[i][j]='a';
  		}
		usleep(1000);
	}
}

void do_access (int tid, char** p) {

	cout << tid << " start access" << endl;
	int loop=0;

	//default is 2
	while(1) {
	//default is S
		int start_index = rand() % S;
		for(int i = 0; i < S; i++){
   			for(int n = 0; n < M; n++) {
				p[(i + start_index) % S][n]='b';
        usleep(10);
			}
		}
	usleep(100000);
	//sleep(1);
  //break;
	loop++;
	}
  
  while (1)
  {
    sleep(1);
  }

}

void _memory_(int index) {

    int pid = getpid();
    std::cout << "child pid: " << pid << endl;
	//sieze of 1K pointer
	char *p[S];
	allocate(index,p);
	do_access(index,p);

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
	char i;
	std::cout << "press any key to start" << std::endl;
	i = getchar();

	//allocation
	timespec ts, te;
	clock_gettime(CLOCK_REALTIME, &ts);
	std::cout<<"start"<<std::endl;
	std::vector<thread> threads;
	for(int i = 1; i <= N; i++) {
		threads.push_back(thread(&_memory_,i));
	}

	for(auto& th : threads){
		th.join();
	}
	clock_gettime(CLOCK_REALTIME, &te);
	std::cout << "finish " << double(te.tv_sec-ts.tv_sec) << std::endl;
	return 0;
}
