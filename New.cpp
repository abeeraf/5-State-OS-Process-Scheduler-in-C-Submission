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
	cout<<"In New Process"<<endl;

	char buffer [256];
	for(int x = 0 ; x < sizeof(buffer) ; x++)
	buffer[x]='\0';

	int fd2=open("Processes.txt",O_RDONLY);
	read(fd2,buffer,sizeof(buffer));		//reading from file

	int fd;
	istringstream(argv[1])>>fd;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	string str1="RR";
	string str2="SJF";
	string str3="FCFS";
	string str4="SRTF";

	//queue procs;

	string copy="               ";
  	int count=0,process_num=0,arrtime,burtime;

	int y=0;
	while (buffer[y]!='\n')
	{
		copy[y]=buffer[y];
		y++;
		count++;
	}
	
	y++; // moving from the NULL character
	copy.resize(count);

	int algo;// 1=RR 2=SJF 3=FCFS 4=SRTF
	int quantum_num=-1;

	if(copy==str1)
	{
		algo=1;
		copy="               ";
		count=0;
		while (buffer[y]!='\n')
		{
			copy[count]=buffer[y];
			y++;
			count++;
		}
		
		copy.resize(count);

		stringstream conv(copy);
		conv >> quantum_num;
		cout<<"THE SCEDULING ALGORITHM IS "<<str1<<endl;
	}
	
	else if(copy==str2)
	{
		algo=2;
		cout<<"THE SCEDULING ALGORITHM IS "<<str2<<endl;
	}
	else if(copy==str3)
	{
		algo=3;
		cout<<"THE SCEDULING ALGORITHM IS "<<str3<<endl;
	}
	else if(copy==str4)
	{
		algo=4;
		cout<<"THE SCEDULING ALGORITHM IS "<<str4<<endl;
	}

	copy="               ";
	count=0;
	y++;
	for(int x = y ; x < sizeof(buffer); )
	{
		while (buffer[x]!='\n')
		{
			x++;
		}
		process_num++;
		x++;
		while (buffer[x]!='\n')
		{
			copy[count]=buffer[x];
			x++;
			count++;
		}
		x++;
		copy.resize(count); 
		stringstream conv(copy);
		conv >>arrtime;

		copy="               ";
		count=0;

		while (buffer[x]!='\n')
		{
			copy[count]=buffer[x];
			x++;
			count++;
		}
		x++;
		copy.resize(count);
		stringstream conv2(copy);
		conv2 >>burtime;

		copy="               ";
		count=0;

		Process Proc;
		Proc.arrival_time=arrtime;
		Proc.burst_time=burtime;
		Proc.number=process_num;
		Proc.algo=algo;
		Proc.priority=0;
		Proc.waiting_time=0;
		Proc.quantum_time=quantum_num;
		Proc.burst_time2=burtime;
		//procs.enqueue(Proc);
		write(fd,&Proc,sizeof(Proc));		//writing to pipe one process at a time
		
	}
}







