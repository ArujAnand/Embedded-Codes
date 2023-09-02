#include <stdio.h>
//#include "display.h"

#define ROW 2
#define COL 3

const char dispData[10][8] = {{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF}, {4,5,6}};

void display_digit(char number, char row, char col)
{
	int i,j;
	char x,y;
	unsigned char mask = 0x80;
	x=row;
	y=col;
	for(i=0;i<8;i++) //rows
	{
	   mask = 0x80;
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
	//currVal++;
	display_digit(0,10,20);
	//display_digit(val%10,10,30);
	return;
}
