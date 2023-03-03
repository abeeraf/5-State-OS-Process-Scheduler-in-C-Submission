#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
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

int main()
{
	int p1[2],p2[2],p3[2],p4[2],p5[2],p6[2],p7[2],p8[2],p9[2],p10[2],status=0;

 	pipe(p1);// communication between New 		-> Ready 	
	pipe(p2);// communication between Ready 	-> Running 	
	pipe(p3);// communication between Running 	-> Ready	
	pipe(p4);// communication between Running	-> Exit		
	pipe(p5);// communication between Running 	-> Blocked
	pipe(p6);// communication between Exit		-> Running
	pipe(p7);// communication between Running 	-> Ready 	status to send new process
	pipe(p8);// communication between Blocked	-> Running
	pipe(p9);// communication between Ready		-> Exit

	fcntl(p1[0],F_SETFL,O_NONBLOCK);
	fcntl(p2[0],F_SETFL,O_NONBLOCK);
	fcntl(p3[0],F_SETFL,O_NONBLOCK);
	fcntl(p4[0],F_SETFL,O_NONBLOCK);
	fcntl(p5[0],F_SETFL,O_NONBLOCK);
	fcntl(p6[0],F_SETFL,O_NONBLOCK);
	fcntl(p7[0],F_SETFL,O_NONBLOCK);
	fcntl(p8[0],F_SETFL,O_NONBLOCK);
	fcntl(p9[0],F_SETFL,O_NONBLOCK);

	pid_t pid1=fork();

	if( pid1 )
	{
		/*parent code*/
		wait(&status);
		pid_t pid2=fork();

		if( pid2 )
		{
			/*parent code*/
			//wait(&status);
			pid_t pid3=fork();
			
			if( pid3 )
			{
				/*parent code*/
				//wait(&status);
				pid_t pid4=fork();
				if( pid4 )
				{
					/*parent code*/
					//wait(&status);
					pid_t pid5=fork();
					if( pid5 )
					{
						/*parent code*/
						//wait(&status);
					}
					else 
					{
						/*child code5*/	
						string str  =to_string(p4[0]);//read proc from Running
						string str1 =to_string(p6[1]);//write status to Running
						string str2 =to_string(p9[0]);// read from ready 
						execlp("./Exit","Exit",str.c_str(),str1.c_str(),str2.c_str(),NULL);			
						//exit(status);
					}

				}
				else 
				{
					/*child code4*/
					string str =to_string(p5[0]);// read from running
					string str1 =to_string(p8[1]);// write to ready
					execlp("./Blocked","Blocked",str.c_str(),
						str1.c_str(),NULL);				
					//exit(status);
				}

			}
			else 
			{
				/*child code3*/	
				string str  =to_string(p2[0]);//recieving the processes from Ready
				string str1 =to_string(p4[1]);//write proc to exit
				string str2 =to_string(p5[1]);//write to blocked queue
				string str3 =to_string(p3[1]);//write to ready time splice completed
				string str4 =to_string(p6[0]);//receive from exit status
				string str5 =to_string(p7[1]);//send status to ready to send new process
				execlp("./Running","Running",str.c_str(),str1.c_str(),str2.c_str(),str3.c_str(),
					str4.c_str(),str5.c_str(),NULL);			
				//exit(status);
			}

		}
		else 
		{
			/*child code2*/	
	
			string str =to_string(p1[0]); // recieving the processes from New that are being made
			string str1=to_string(p2[1]); // to send process to Running so will write from pipe
			string str2=to_string(p3[0]); // to read process from running
			string str3=to_string(p7[0]); // read status to send new process
			string str4=to_string(p8[0]); // receive from blocked queue
			string str5=to_string(p9[1]); // send number of processes
			execlp("./Ready","Ready",str.c_str(),str1.c_str(),str2.c_str(),
				str3.c_str(),str4.c_str(),str5.c_str(),NULL);		
			//exit(status);
		}
	}
	else 
	{
		/*child code1*/
		string str=to_string(p1[1]);// will send the process to Ready from New
		execlp("./New","New",str.c_str(),NULL);
		exit(status);
	}	

}
