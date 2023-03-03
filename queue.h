#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sstream> 

using namespace std;

struct Process{

	Process* next;
	
	int number;
	int algo;
	int priority;
	int arrival_time;
	int burst_time;
	int quantum_time;
	int blocking_time;
	int waiting_time;

	int burst_time2;
	int turnat;

	Process()
	{
		number=0;
		algo=0;
		priority=0;
		arrival_time=0;
		burst_time=0;
		quantum_time=0;
		waiting_time=0;
		blocking_time=0;
		
		burst_time2=0; //for output will cout
		turnat=0;

	}
};

class queue{

	Process *front,*rear;
	
	public:
	int count;	

	
	queue(){

		count= 0;
		front= NULL;
		rear = NULL;
	}
	void enqueue(Process x)	{
		
		Process* newnode;
		newnode = new Process;

		newnode->arrival_time 	= x.arrival_time;
		newnode->burst_time 	= x.burst_time;
		newnode->quantum_time 	= x.quantum_time;
		newnode->number 	= x.number;
		newnode->priority 	= x.priority;
		newnode->waiting_time 	= x.waiting_time;
		newnode->algo 		= x.algo;
		newnode->blocking_time	= x.blocking_time;
		newnode->burst_time2 	= x.burst_time2;
		newnode->turnat		= x.turnat;

		if (count == 0)
		{
			rear = newnode;
			front = newnode;
			count++;
		}
		else
		{

			rear->next = newnode;
			rear = rear->next;
			rear->next=NULL;
			count++;
		}

	}

	bool isEmpty(){
		if (!count)
		return 1;
		else
		return 0;
	}

	Process dequeue(){

		if (front != NULL)
		{
			Process* temp = front;
			Process dat;

			dat.arrival_time 	= temp->arrival_time;
			dat.burst_time 		= temp->burst_time;
			dat.quantum_time 	= temp->quantum_time;
			dat.number 		= temp->number;
			dat.priority 		= temp->priority;
			dat.algo		= temp->algo;
			dat.waiting_time 	= temp->waiting_time;
			dat.blocking_time	= temp->blocking_time;
			dat.next 		= temp->next;
			dat.burst_time2 	= temp->burst_time2;
			dat.turnat		= temp->turnat;

			front = front->next;
			delete temp;
			count--;

			return dat;
		}
	}
	void decrease_blocking_time(int x)
	{
		Process* front2 = front;
		while (front2)
		{
			front2->blocking_time-=x;
			front2=front2->next;
		}
	}
	void add_turn_time(int x)//used with blocked queue 
	{
		Process* front2 = front;
		while (front2)
		{
			front2->turnat+=x;
			front2=front2->next;
		}
	}
	void add_waiting_time(int x)//in Ready State
	{
		Process* front2 = front;
		while (front2)
		{
			front2->waiting_time+=x;
			front2=front2->next;
		}
	}
	bool zeroTracker(int x)//brings the value to the front. then dequeue. 1=burst_time, 2=arrival_time, 3=blocked_time
	{
		if(x==1)// bring process with min burst time to the front
		{
			Process* front2 = front;
			int min=front2->burst_time;
			while (front2)//finding min arrival time
			{
				if(front2->burst_time<=min)
				min=front2->burst_time;

				front2=front2->next;
			}
			front2 = front;
			while (front2)//Now bringing it to the front
			{
				if(front2->burst_time==min)
				{
					Process* temp = new Process;
					Process* dat = 	new Process;

					dat->arrival_time 	= front2->arrival_time;
					dat->burst_time 	= front2->burst_time;
					dat->quantum_time 	= front2->quantum_time;
					dat->number 		= front2->number;
					dat->priority 		= front2->priority;
					dat->algo		= front2->algo;
					dat->waiting_time 	= front2->waiting_time;
					dat->blocking_time	= front2->blocking_time;
					dat->burst_time2 	= front2->burst_time2;
					dat->turnat		= front2->turnat;
				//	dat->next 		= front2->next;	

					front2->arrival_time 	= front->arrival_time;
					front2->burst_time 	= front->burst_time;
					front2->burst_time2 	= front->burst_time2;
					front2->quantum_time 	= front->quantum_time;
					front2->number 		= front->number;
					front2->priority 	= front->priority;
					front2->algo		= front->algo;
					front2->waiting_time 	= front->waiting_time;
					front2->blocking_time	= front->blocking_time;
					front2->turnat		= front->turnat;
				//	front2->next 		= front->next;	

					front->arrival_time 	= dat->arrival_time;
					front->burst_time 	= dat->burst_time;
					front->burst_time2 	= dat->burst_time2;
					front->quantum_time 	= dat->quantum_time;
					front->number 		= dat->number;
					front->priority 	= dat->priority;
					front->algo		= dat->algo;
					front->waiting_time 	= dat->waiting_time;
					front->blocking_time	= dat->blocking_time;
					front->turnat		= dat->turnat;
				//	front->next 		= dat->next;
				return 1;
				}			
				front2=front2->next;
			}
		}
		if(x==2)// bring process with minimum arrival time to the front
		{
			Process* front2 = front;
			int min=front2->arrival_time;
			while (front2)//finding min arrival time
			{
				if(front2->arrival_time<=min)
				min=front2->arrival_time;

				front2=front2->next;
			}
			front2 = front;
			while (front2)//Now bringing it to the front
			{
				if(front2->arrival_time==min)
				{
					Process* temp = new Process;
					Process* dat = 	new Process;

					dat->arrival_time 	= front2->arrival_time;
					dat->burst_time 	= front2->burst_time;
					dat->quantum_time 	= front2->quantum_time;
					dat->number 		= front2->number;
					dat->priority 		= front2->priority;
					dat->algo		= front2->algo;
					dat->waiting_time 	= front2->waiting_time;
					dat->blocking_time	= front2->blocking_time;
					dat->burst_time2 	= front2->burst_time2;
					dat->turnat		= front2->turnat;
				//	dat->next 		= front2->next;	

					front2->arrival_time 	= front->arrival_time;
					front2->burst_time 	= front->burst_time;
					front2->burst_time2 	= front->burst_time2;
					front2->quantum_time 	= front->quantum_time;
					front2->number 		= front->number;
					front2->priority 	= front->priority;
					front2->algo		= front->algo;
					front2->waiting_time 	= front->waiting_time;
					front2->blocking_time	= front->blocking_time;
					front2->turnat		= front->turnat;
				//	front2->next 		= front->next;	

					front->arrival_time 	= dat->arrival_time;
					front->burst_time 	= dat->burst_time;
					front->burst_time2 	= dat->burst_time2;
					front->quantum_time 	= dat->quantum_time;
					front->number 		= dat->number;
					front->priority 	= dat->priority;
					front->algo		= dat->algo;
					front->waiting_time 	= dat->waiting_time;
					front->blocking_time	= dat->blocking_time;
					front->turnat		= dat->turnat;
				//	front->next 		= dat->next;
				return 1;
				}
				front2=front2->next;
			}
		}
		if(x==3)//bring process with 0 block time to the front
		{
			Process* front2 = front;
			while (front2->blocking_time && front2)
			{
				if(!front2->blocking_time)	
				{
					Process* temp = new Process;
					Process* dat = 	new Process;

					dat->arrival_time 	= front2->arrival_time;
					dat->burst_time 	= front2->burst_time;
					dat->quantum_time 	= front2->quantum_time;
					dat->number 		= front2->number;
					dat->priority 		= front2->priority;
					dat->algo		= front2->algo;
					dat->waiting_time 	= front2->waiting_time;
					dat->blocking_time	= front2->blocking_time;
					dat->burst_time2 	= front2->burst_time2;
					dat->turnat		= front2->turnat;
				//	dat->next 		= front2->next;	

					front2->arrival_time 	= front->arrival_time;
					front2->burst_time 	= front->burst_time;
					front2->burst_time2 	= front->burst_time2;
					front2->quantum_time 	= front->quantum_time;
					front2->number 		= front->number;
					front2->priority 	= front->priority;
					front2->algo		= front->algo;
					front2->waiting_time 	= front->waiting_time;
					front2->blocking_time	= front->blocking_time;
					front2->turnat		= front->turnat;
				//	front2->next 		= front->next;	

					front->arrival_time 	= dat->arrival_time;
					front->burst_time 	= dat->burst_time;
					front->burst_time2 	= dat->burst_time2;
					front->quantum_time 	= dat->quantum_time;
					front->number 		= dat->number;
					front->priority 	= dat->priority;
					front->algo		= dat->algo;
					front->waiting_time 	= dat->waiting_time;
					front->blocking_time	= dat->blocking_time;
					front->turnat		= dat->turnat;
				//	front->next 		= dat->next;
				return 1;
				}
				front2=front2->next;
			}
		}
		
	}	
	void Display(){

		Process* front2 = front;
		while (front2)
		{
			cout<<"Process      	:"<<front2->number<<endl;
			cout<<"Arrival Time 	:"<<front2->arrival_time	<<endl;
			cout<<"Burst Time   	:"<<front2->burst_time2		<<endl;
			//cout<<"Quantum TIme 	:"<<front2->quantum_time	<<endl;
			//cout<<"Priority     	:"<<front2->priority		<<endl;
			cout<<"Waiting Time	:"<<front2->waiting_time	<<endl;
			cout<<"Turn Around Time :"<<front2->turnat		<<endl<<endl;
			//cout<<"Blocking Time	:"<<front2->blocking_time	<<endl;
			//cout<<"Algo         	:"<<front2->algo		<<endl<<endl;
			front2=front2->next;
		}
	}
	~queue(){
		
	}
};

class queue2
{

public:

	int size;
	int start;
	int end;
	Process *data;

public:
    
	queue2()
	{
		size=50;
		start=end=0;
		data=new Process[size];
	}
 
	queue2(int n)
	{
		size=n;
		start=0;
		data=new Process[size];
	}

	void put(Process value)
	{
		data[start++]=value;
		return;
	}
	//utility function to decrease blockingTime of procs
	void dec()
	{
		for(int x = 0 ;x < start ; x++)
		data[x].blocking_time--;
	}
	
	//utility function to increase completion time of proc
	void incCompletionTime()
	{
		for(int x = 0 ; x < start ; x ++)
		data[x].turnat++;
	}
	
	void remove(int pos)
	{
		for(int x = pos ; x < start ; x++ )
		data[x]=data[x+1];

		start--;
	}
	//utility function to check if  time get zero of proc, it return the index, indicating that now a proc has
	//finished blocking state;
	int dispatch()
	{
		for(int x = 0 ;x < start ; x++)
		{
			if(data[x].blocking_time<=0)
			return x;
		}
		return -1;
	}

	//utility function to increase the waiting time of procs in queue
	void increaseWaitingTime()
	{
		for(int x = 0 ; x < start ; x++)
		data[x].waiting_time++;
	}
  
};
