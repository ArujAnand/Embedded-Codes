#include <stdio.h>
//#include "display.h"

#define ROW 2
#define COL 3

const char dispData[10][8] = {{0xFF,0x81,0x81,0x81,0x81,0x81,0x81,0xFF}, {0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01}, {0xFF,0x01,0x01,0xFF,0x40,0x40,0xFF,0xFF}, {0xFF,0x01,0x01,0xFF,0x01,0x01,0xFF} };

void display_digit(char number, char row, char col)
{
	int i,j;
	char x,y;
	unsigned char mask = 0x80;
	x=row;
	y=col;
	for(i=0;i<7;i++) //rows
	{
	   mask = 0x40;
	   printf("\e[%d;%dH",x++,y);
	   for(j=0;j<8;j++) //columns
	   {
	      if(dispData[number][i] & mask)
		     putchar('*');
	      else
		     putchar(0x20);
	         mask>>=1;
	         fflush(stdout);
	   }


	}
}

void main()
{
	//display_digit(1,10,20);
	//display_digit(2,10,20);
	display_digit(3,10,30);
	return;
}
