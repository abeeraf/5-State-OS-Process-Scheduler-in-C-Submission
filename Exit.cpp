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
	cout<<"In Exit Process"<<endl;
	int fd1,fd2,fd3,E_stat=1;
	int counter = 0;	
	int num_procs_total;
	queue collect_exited_procs;	
	
	Process proc;

	istringstream(argv[1])>>fd1;// receive proc from running
	istringstream(argv[2])>>fd2;// write status to running
	istringstream(argv[3])>>fd3;//reading total procs from ready

	read(fd3 , &num_procs_total , sizeof(num_procs_total));

	while(num_procs_total)
	{
	
		counter++;
		////////////////////////////////READING FROM THE RUNNING PROCESS/////////////////////////////////////////
		while(read(fd1 , &proc , sizeof(proc))>0)
		{
			cout<<"Exit   : Process "<<proc.number<<" has Exited."<<endl;
			write(fd2 , &E_stat , sizeof(E_stat));
			num_procs_total--;
			collect_exited_procs.enqueue(proc);

                  	cout<<"Process "<<proc.number<<" has Exited."<<endl<<endl;
			cout<<"Arrival Time     :"<<proc.arrival_time <<endl;
			cout<<"Burst Time       :"<<proc.burst_time2 <<endl;
			cout<<"Waiting Time     :"<<proc.waiting_time <<endl;
			cout<<"Turn Around Time :"<<proc.turnat <<endl<<endl;
	
			if(counter%30==0 or !num_procs_total) //if timer is 30 or all processes exit, writing to file
			{
				//Writing Output to File

				int OutPut_File_Desc=open("Output.txt",O_WRONLY|O_APPEND);
				int Terminal=dup(1);
				dup2(OutPut_File_Desc,1);
				collect_exited_procs.Display();
				dup2(Terminal,1);
				close(Terminal);
				//Now empty Queue for next incoming processes
				while(!collect_exited_procs.isEmpty())
				collect_exited_procs.dequeue();
			}
		

		}
		
	}

}
