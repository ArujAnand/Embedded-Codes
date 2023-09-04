/*
 ============================================================================
 Name        : WinProject1.c
 Author      : xyz
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include "display.h"

//extern int currVal;
//extern void display(char*, int);
typedef struct{
	unsigned short sec:6;
	unsigned short min:6;
	unsigned short hr:4;
} timeRecord;
#define MAXMS0 500
#define MAXMS  500
#define MAMSCOUNT 1000
#define MAXSEC 59
#define MAXMIN 59
#define MAXHR 12

timeRecord currTime,setAlarm ;
unsigned short msCount =0;
bool processFlag = 0;
bool setAlarmFlag =0;
enum states{IDLE, ALARMSET, START} ;
enum states currState, prevState;
enum timestates{HOUR, MIN, SEC} ;
enum timestates timeopt;


int option;
void delayms()
{
	int i,j;
	int data=0;

	for(i=0;i<MAXMS ; i++)
	{
		for(j=0;j<MAXMS0 ; j++)
		{
			data++;
		}
	}
	msCount++;
}
/*                */
void processTime()
{
  // int i;
   delayms();
   if(msCount > MAMSCOUNT)
   {
	   msCount=0;
	   currTime.sec++;
	   if(currTime.sec > MAXSEC)
	   {
		   currTime.sec =0;
		   currTime.min++;
		   if (currTime.min >MAXMIN)
		   {
			   currTime.min=0;
			   currTime.hr ++;
			   if (currTime.hr >MAXHR)
				   currTime.hr =0;
			   //option3
		   }
           //option2
	   }

      //option 1
	   if( (currTime.hr==setAlarm.hr) && (currTime.min==setAlarm.min) && (currTime.sec == setAlarm.sec))
	   {
		   processFlag = 0; //halt the counting
		   //beep
	   }

   }

}

void getoption()
{

	printf("Select one of the following option:\n") ;
	printf("1: Press e to edit settime \n");
	printf("2: Press s to Start \n");
     option=getchar();

}

void setAlarmTime()
{
     switch(timeopt)
     {
        case HOUR:
        	printf("Enter 0-12 for HH:\n") ;
        	option=getchar();
        	if((option>=0) && (option<=12))
        	{
        	 setAlarm.hr = option;
        	 timeopt=MIN;
        	}
    	    break;
        case MIN:
        	  break;
        case SEC:
        	  setAlarmFlag=1;
        	  break;
        default:
        	  break;

     }
}

void processState()
{
	switch (currState)
	{
	  case IDLE:
            getoption();
            if (option == 's')
            {
              processFlag =1;
              currState=START;
            }
            else if (option == 'e')
            {
            	timeopt=HOUR;
            	setAlarmFlag =0;
            	currState=ALARMSET;
            }

            break;
	  case ALARMSET:
		   if(!setAlarmFlag)
            setAlarmTime();
	        break;
	  case START:
		  getoption();
		  if (option == 's')
		  {
		    processFlag =0;
		    currState=IDLE;
		  }
		    break;
	  default:
		   break;
	}
}

int main(){

currState = prevState = IDLE;
currVal++;
display("test\n", 0);

 while(1);
 {
   if(processFlag)
	  processTime();
	processState();
 }

return 0;
}