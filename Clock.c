#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

typedef struct
{
	unsigned short sec : 6;
	unsigned short min : 6;
	unsigned short hr : 4;
} timeRecord;
#define MAXMS0 500
#define MAXMS 500
#define MAMSCOUNT 300 // changed from 1000 to 500 to reduce delay
#define MAXSEC 59	  // Max value of sec and so on
#define MAXMIN 59
#define MAXHR 12
/*To store time which was last retrieved*/
timeRecord currTime, setAlarm, lastTime;
unsigned short msCount = 0;
bool processFlag = 1;
bool setAlarmFlag = 0;
enum states {IDLE, ALARMSET, START};
enum states currState, prevState;

enum timestates {HOUR, MIN, SEC};
enum timestates timeopt; //variable timeopt of timestates type created, this variable can hold one of the three values HOUR, MIN, or SEC

int check = 0;

int option;

// display data for displaying numbers
const char dispData[10][8] = {{0xFF, 0x41, 0x41, 0x41, 0x41, 0x41, 0xFF},
							  {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 /*1*/},
							  {0xFF, 0x01, 0x01, 0xFF, 0x40, 0x40, 0xFF},
							  {0xFF, 0x01, 0x01, 0xFF, 0x01, 0x01, 0xFF /*3*/},
							  {0x41, 0x41, 0x41, 0xFF, 0x01, 0x01, 0x01},
							  {0xFF, 0x40, 0x40, 0xFF, 0x01, 0x01, 0xFF},
							  {0xFF, 0x40, 0x40, 0xFF, 0x41, 0x41, 0xFF /*6*/},
							  {0xFF, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20},
							  {0xFF, 0x41, 0x41, 0xFF, 0x41, 0x41, 0xFF /*8*/},
							  {0xFF, 0x41, 0x41, 0xFF, 0x01, 0x01, 0xFF}};

// colon printer
void colon_printer(int row1, int col, int row2)
{
	for (int i = row1; i < row1 + 2; i++)
	{
		for (int j = col; j < col + 2; j++)
		{
			printf("\e[%d;%dH", i, j);
			putchar('*');
		}
	}

	for (int i = row2; i < row2 + 2; i++)
	{
		for (int j = col; j < col + 2; j++)
		{
			printf("\e[%d;%dH", i, j);
			putchar('*');
		}
	}
}

// directs the display_digit function to display the digit(s)
void display_controller()
{
	if (lastTime.hr != currTime.hr)
	{
		display_digit(currTime.hr / 10, 10, 20);
		display_digit(currTime.hr % 10, 10, 30);
		lastTime.hr = currTime.hr;
	}

	if (lastTime.min != currTime.min)
	{
		colon_printer(11, 40, 14);
		display_digit(currTime.min / 10, 10, 45);
		display_digit(currTime.min % 10, 10, 55);
		lastTime.min = currTime.min;
	}

	if (lastTime.sec != currTime.sec)
	{
		colon_printer(11, 65, 14);
		display_digit(currTime.sec / 10, 10, 70);
		display_digit(currTime.sec % 10, 10, 80);
	}
}

// This function handles displaying of individual digits on the console
void display_digit(char number, char row, char col)
{
	int i, j;
	char x, y;
	unsigned char mask = 0x80;
	x = row;
	y = col;
	for (i = 0; i < 7; i++) // rows
	{
		mask = 0x40;
		printf("\e[%d;%dH", x++, y);
		for (j = 0; j < 8; j++) // columns
		{
			if (dispData[number][i] & mask)
				putchar('*');
			else
				putchar(0x20);
			mask >>= 1;
			fflush(stdout); /*fflush ensures that data is displayed immediately generally
							   not used unless immediate character output requirement is there*/
		}
	}
}

// Manages current time
void processTime()
{
	// int i;
	Sleep(0.5); // Time delay will be changed according to the requirement
	msCount++;
	if (msCount > MAMSCOUNT)
	{
		msCount = 0;
		currTime.sec++;
		if (currTime.sec > MAXSEC)
		{
			currTime.sec = 0;
			currTime.min++;
			if (currTime.min > MAXMIN)
			{
				currTime.min = 0;
				currTime.hr++;
				if (currTime.hr > MAXHR)
					currTime.hr = 0;
				// option3
			}
			// option2
		}
		display_controller();
		// option 1
		if ((currTime.hr == setAlarm.hr) && (currTime.min == setAlarm.min) && (currTime.sec == setAlarm.sec))
		{
			processFlag = 0; // halt the counting
			// beep
			currState = IDLE;
		}
	}
}

void getoption()
{

	printf("Select one of the following option:\n");
	printf("1: Press e to edit settime \n");
	printf("2: Press s to Start \n");
	option = getchar();
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
	 currState = IDLE;
	 prevState = ALARMSET;
}

void processState()
{
	switch (currState)
	{
	case IDLE:
		getoption();
		if (option == 's')
		{
			processFlag = 1;
			currState = START;
		}
		else if (option == 'e')
		{
			timeopt = HOUR;
			setAlarmFlag = 0;
			currState = ALARMSET;
		}

		break;
	case ALARMSET:
		if (!setAlarmFlag)
			setAlarmTime();
		break;
	case START:
		//getoption();
		if (option == 's')
		{
			processFlag = 0;
			currState = START;
			prevState = START;
			display_digit(0, 10, 20);
			display_digit(0, 10, 30);
			colon_printer(11, 40, 14);
			display_digit(0, 10, 45);
			display_digit(0, 10, 55);
			colon_printer(11, 65, 14);
			display_digit(0, 10, 70);
			display_digit(0, 10, 80);
		}
		break;
	default:
		break;
	}
}

void main()
{
	//currState = prevState = IDLE;
	//processState();
	// display_digit(1,10,20);
	// display_digit(9,10,20);
	// display_digit(7,10,30);
	/*while (1)
	{
		if (check == 0)
		{
			getoption();
			if (option == 's')
			{
				display_digit(0, 10, 20);
				display_digit(0, 10, 30);
				colon_printer(11, 40, 14);
				display_digit(0, 10, 45);
				display_digit(0, 10, 55);
				colon_printer(11, 65, 14);
				display_digit(0, 10, 70);
				display_digit(0, 10, 80);
			}
			check++;
		}
		processTime();
	}*/
	
	currState = prevState = IDLE;
	processState();

	while (1)
	{
		if(currState != prevState)
		{
			processState();
		}
		
		if(currState == prevState)
		{
			processTime();
		}
	}
	
	return;
}
