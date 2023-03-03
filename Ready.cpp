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

	cout<<"In Ready Process"<<endl;

	int fd,fd2,fd3,fd4,fd5,fd6;
	int running_stat=1; //Initially true
	int status=0;	
	int procs_exited=0;

	istringstream(argv[1])>>fd; // Read proc from New 
	istringstream(argv[2])>>fd2;//send proc to running state 
	istringstream(argv[3])>>fd3;//receive from Running after time splice completed
	istringstream(argv[4])>>fd4;//read status to send new process to running
	istringstream(argv[5])>>fd5;//from blocked queue
	istringstream(argv[6])>>fd6;//send total procs to exit

	Process proc,to_run_proc;
	queue procs;//Specialised for acting as both heaps and queues

	while(read( fd , &proc , sizeof(proc) )>0)/////////////READING THE NEW PROCESS FROM THE PIPE/////////////
	{
		procs.enqueue(proc);// Initial queue made.
		procs_exited++;

	}
	write(fd6,&procs_exited,sizeof(procs_exited));

	while (procs_exited)
	{
		while(read( fd5 , &proc , sizeof(proc) )>0)/////////////////READING THE PROCESS FROM THE BLOCKED STATE
		{	
			procs.enqueue(proc);// add it to ready 	
			cout<<"Ready  : Process "<<proc.number<<" Received from Blocked State"<<endl;
		}
		while(read( fd4 , &status , sizeof(status) )>0)//READING THE STATUS FROM THE RUNNING STATE
		{		
			running_stat++;

			if(status==2)
			procs_exited--;
			
			int x;
			if(to_run_proc.algo==1)
			{	if(to_run_proc.burst_time>to_run_proc.quantum_time)
				x=to_run_proc.quantum_time;
				else 
				x=to_run_proc.burst_time;
			}
			else 
			{
				if(to_run_proc.burst_time>5)
				x=5;
				else 
				x=to_run_proc.burst_time;
			}
			procs.add_turn_time(x);
			procs.add_waiting_time(x);

			cout<<"Ready  : Status Received from Running State"<<endl;
		}
		while(read( fd3 , &proc , sizeof(proc) )>0)/////////////////READING THE PROCESS FROM THE RUNNING STATE
		{	
			running_stat++;
			procs.enqueue(proc);// add it to ready 	
			cout<<"Ready  : Process "<<proc.number<<" Received from Running State"<<endl;
		}
		if (proc.algo == 1)// RR //Further Implementation of Quantum time in Running State
		{

			if(running_stat && !procs.isEmpty()){
			
			procs.zeroTracker(2);// This will bring the process with the minimum arrival time to the front
			procs.add_turn_time(1);
			to_run_proc=procs.dequeue();// Dispatching it to the running state			
			procs.add_waiting_time(1);			
			write(fd2,&to_run_proc,sizeof(to_run_proc));	

			cout<<"Ready  : Process number "<<to_run_proc.number<<"'s Arrival Time"<<endl;
			cout<<"Ready  : Sending Process to Running"<<endl;		
			running_stat--;
			}
		}
		else if (proc.algo == 2)//SJF
		{
			if(running_stat && !procs.isEmpty()){
			//This is preemptive scheduling so for every process it will find the one with the minimum burst time
			procs.zeroTracker(1);// This will bring the process with the minimum burrst time to the front			
			procs.add_turn_time(1);
			procs.add_waiting_time(1);						
			to_run_proc=procs.dequeue();// Dispatching it to the running state						
			write(fd2,&to_run_proc,sizeof(to_run_proc));	

			cout<<"Ready  : Process number "<<to_run_proc.number<<"'s Arrival Time"<<endl;
			cout<<"Ready  : Sending Process to Running"<<endl;		
			running_stat--;
			}
		
		}
		else if (proc.algo == 3)//FCFS the process to come first will be dispatched first 
		{
			if(running_stat && !procs.isEmpty()){

			procs.zeroTracker(2);// This will bring the process with the minimum arrival time to the front
			procs.add_turn_time(1);
			procs.add_waiting_time(1);			
			to_run_proc=procs.dequeue();// Dispatching it to the running state
			write(fd2,&to_run_proc,sizeof(to_run_proc));	

			cout<<"Ready  : Process number "<<to_run_proc.number<<"'s Arrival Time"<<endl;
			cout<<"Ready  : Sending Process to Running"<<endl;		
			running_stat--;
			}
			
		}
		else if (proc.algo == 4)//SRTF
		{	
			if(running_stat && !procs.isEmpty()){
	
			procs.zeroTracker(1);// This will bring the process with the minimum remaining time to the front	
			procs.add_turn_time(1);
			procs.add_waiting_time(1);			
			to_run_proc=procs.dequeue();// Dispatching it to the running state		
			write(fd2,&to_run_proc,sizeof(to_run_proc));	

			cout<<"Ready  : Process number "<<to_run_proc.number<<"'s Arrival Time"<<endl;
			cout<<"Ready  : Sending Process to Running"<<endl;		
			running_stat--;
			}
		}
	}
	
}




