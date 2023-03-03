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
	cout<<"In Running Process"<<endl;
	int fd1,fd2,fd3,fd4,fd5,fd6;
	int E_stat=0;
	int read_error;
	int ask_ready=1;
	int from_exit=2;

	int loop_var=5;
	Process Proc;
	
	istringstream(argv[1])>>fd1;// receive from ready
	istringstream(argv[2])>>fd2;// send to exit
	istringstream(argv[3])>>fd3;// to blocked queue 
	istringstream(argv[4])>>fd4;// sending proc to ready state, time slice completed
	istringstream(argv[5])>>fd5;// receive from exit
	istringstream(argv[6])>>fd6;// For sending status to ready to send new process 

	srand(time(NULL));
	while(true)
	{	
		////////////////////////////////READING FROM THE EXIT PROCESS/////////////////////////////////////////
		while(read(fd5 , &E_stat , sizeof(E_stat))>0)//FROM EXIT PROCESS
		{ 
			if(E_stat)
			{
				cout<<"Running: Process "<<Proc.number<<" has Exited"<<endl;// SO SEND NEXT PROCESS FOR EXECUTION 
				write (fd6, &from_exit, sizeof(from_exit));//using same pipe as for blocked
			} 
		 }
		
		////////////////////////////////COMMUNICATION WITH READY, BLOCKED AND RUNNING////////////////////////
		////////////////////////////////WRITING TO THEM//////////////////////////////////////////////////////
		while(read( fd1 , &Proc, sizeof(Proc))>0)// receive proc from ready
		{ 
			for( int x = 0 ; x < loop_var ; x++ )
			{	
				Proc.turnat++;//for adding to turn time 
				if(Proc.algo==1)// so if round robin it will run according to the quantum time
				loop_var=Proc.quantum_time;				

				sleep(1);
				Proc.burst_time--;
				if(!Proc.burst_time)
				{
					write (fd2, &Proc, sizeof(Proc));// send to exit state 
					break;
				}
		    	}
			//AFTER 5 TICKS RANDOM NUMBER IS GENERATED 
			if(Proc.burst_time)
			{	Proc.turnat++;
				int random=rand()%2;
				if(!random)
				{	
					//Send to Blocked State
					write (fd3, &Proc, sizeof(Proc));
					// ask Ready for another proc
					cout<<"Running: Asking Ready for a new Process to be sent"<<endl;
					write (fd6, &ask_ready, sizeof(ask_ready));// This will send the running_stat
				}
				else
				{
					//Send to Ready State 	
					cout<<"Running: Sending Process "<<Proc.number<<" to Ready as Time Slice completed"<<endl;			
					write (fd4, &Proc, sizeof(Proc));
		
				}	
			}				
		}
	}

}

















