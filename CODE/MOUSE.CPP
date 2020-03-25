#include<iostream.h>
#include<conio.h>
#include<ctype.h>
#include<graphics.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<fstream.h>
#include<stdlib.h>
#include<iomanip.h>
#include<dos.h>
union REGS in,out;
int initmouse()
{
	in.x.ax=0;
	int86(0X33,&in,&out);
	return(out.x.ax);
}
int callmouse()
{
	in.x.ax=1;
	int86(0X33,&in,&out);
	return 1;
}
void mouseposi(int &xpos,int &ypos, int &click)
{
	in.x.ax=3;
	out.x.bx=0;
	int86(0X33,&in,&out);
	click=out.x.bx;
	xpos=out.x.cx;
	ypos=out.x.dx;
}
int mousehide()
{
	in.x.ax=2;
	int86(0X33,&in,&out);
	return 1;
}
void setposi(int &xpos,int &ypos)
{
	in.x.ax=4;
	in.x.cx=xpos;
	in.x.dx=ypos;
	int86(0X33,&in,&out);
}

