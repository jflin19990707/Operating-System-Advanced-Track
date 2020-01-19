#include "u_print.h"

volatile unsigned char *const UART0DR = (unsigned char *)0x3f201000;

void u_prints(const char *s)
{
    while (*s)
    {
        *UART0DR = (unsigned char)(*s);
        s++;
    }
}

void u_printd(int i)
{
	int digit;
	digit=1;
	char c;
	
	if (i==0) *UART0DR='0';

	while (i/digit>0)
	{
		digit*=10;
	}
	digit/=10;

	while (digit>=1)
	{
		c=i/digit+'0';
		i=i%digit;
		*UART0DR=c;
		digit/=10;
	}
}

void u_println()
{
	u_prints("\n");
}

void u_printsln(const char *s)
{
    while (*s)
    {
        *UART0DR = (unsigned char)(*s);
        s++;
    }
	u_println();
}

void u_printdln(int i)
{
	u_printd(i);
	u_println();
}

void u_printx(int i)
{
	int digit;
	digit=1;
	char c;
	
	u_prints("0x");

	if (i==0) *UART0DR='0';

	while (i/digit>0)
	{
		digit*=16;
	}
	digit/=16;

	while (digit>=1)
	{
		if (i/digit>=10) 
		{
			c=i/digit-10+'a';
		}
		else
			c=i/digit+'0';
		i=i%digit;
		*UART0DR=c;
		digit/=16;
	}
}

void u_printxln(int i)
{
	u_printx(i);
	u_println();
}
