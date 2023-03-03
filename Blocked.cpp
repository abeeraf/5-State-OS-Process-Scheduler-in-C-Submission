#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include <sstream>
#include <string>
#include <iostream>
#include <time.h>
#include "queue.h"
using namespace std;

int main(int agc, char *argv[]) 
{

	cout<<"In Blocked Process"<<endl;

	int fd1,fd2;
	srand(time(NULL));

	istringstream(argv[1])>>fd1;// receive from running
	istringstream(argv[2])>>fd2;// return to ready

	Process proc;

	int nread2;
	queue2 blocked_Q;
  
	while (1) 
	{ 
		while(read(fd1, &proc, sizeof(proc))>0) 
		{
			srand(time(NULL));
			proc.blocking_time=15+rand()%11;
			blocked_Q.put(proc);
			blocked_Q.increaseWaitingTime();
			cout<<"Blocked: Process "<<blocked_Q.data[blocked_Q.start-1].number<<" is in Blocked Queue with blocking Time "<<blocked_Q.data[blocked_Q.start-1].blocking_time<<endl;

		}
		blocked_Q.dec();
		blocked_Q.increaseWaitingTime();
		blocked_Q.incCompletionTime();
		int d= blocked_Q.dispatch();
		if(d!=-1)
		{
			write(fd2,&blocked_Q.data[d],sizeof(proc));
		        cout<<"Blocked: Sent Proc "<<blocked_Q.data[d].number<<" back to ready queue from Blocked state"<<endl;
		        blocked_Q.remove(d);
		}
	}

}
