#include <iostream.h>
#include "/CODE/mouse.cpp"
#include <conio.h>
#include <ctype.h>
#include <graphics.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <fstream.h>
#include <stdlib.h>
#include <iomanip.h>
#include <dos.h>
//All header files declared
int introchck = 1; //Variable used to check for the first intro
#define crcx 320
#define crcy 265
#define crrd 170
//x coordinate, y coordinate and the radius of the main circles in the game
class atom // Class used to define the atoms in the game
{
protected:
	char name[20], symb[3]; //Name and symbol of atom
	int at_no, co;			//Atomic number and colour of atom
public:
	atom() //Constructor used to set null values
	{
		name[0] = '\0';
		symb[0] = '\0';
		at_no = 0;
		co = 0;
	}
	int return_atno() //Returns atomic number
	{
		return at_no;
	}
	char *retrn_na() //Returns name of the atom
	{
		return name;
	}
	void print() //Prints details of the atoms
	{
		cout << ' ' << name << ' ' << symb << ' ' << at_no << ' ' << co << endl;
	}
	void atmdraw(int x, int y, int r) //Used to draw the atoms
	{
		setcolor(co);
		setfillstyle(SOLID_FILL, co);
		settextstyle(0, 0, 1);
		circle(x, y, r);
		floodfill(x, y, co);
		if (co != 1)
			setcolor(BLACK);
		else
			setcolor(WHITE);
		settextjustify(1, 1);
		outtextxy(x, y - 5, symb);
		char st[5];
		itoa(at_no, st, 10);
		outtextxy(x, y + 10, st);
	}
	void set(char na[], char sym[], int at, int c) //Used to assign values for the variables of class atom
	{
		strcpy(name, na);
		strcpy(symb, sym);
		at_no = at;
		co = c;
	}
};
class specatom : public atom //Class for the special atoms used in the game
{
public:
	specatom *flink;
	specatom *blink;				   //Variables to use the atom class as a linked list
	void specdraw(int x, int y, int r) //Draw the special atom
	{
		setcolor(co);
		setfillstyle(SOLID_FILL, co);
		settextstyle(0, 0, 2);
		circle(x, y, r);
		floodfill(x, y, co);
		setcolor(BLACK);
		settextjustify(1, 1);
		outtextxy(x, y + 1, symb);
	}

public:
	void draw(int x, int y, int r = 20) //Function used to draw either atom or special atom
	{
		if (at_no <= 0)
			specdraw(x, y, r); //Draw normal atom w.r.t atomic number
		else
			atmdraw(x, y, r); //Draw special atom
	}
} at[114];
specatom play[17], mid, tempatm;											//Variables used in game
int noatm = 5, base = 0, score = 0, rounds = 1, ntrnrd = -1, antmttrd = -1; //Variables used in game
/* play 	- Holds the atoms in playing board
   mid		- Holds the mid atom
   tempatm	- Used to hold any temporary atom
   noatm	- No of atoms on the board
   base         - Holds the base basic value of the atoms to spawn
   score	- Score of the player
   rounds	- No. of rounds the player has played
   ntrnrd	- Checks whether neutrino is available
   antmttrd	- Checks whether antimatter is available
*/
struct hiscr //Structure for storing highscore
{
	char na[20];										   //Name
	float scr;											   //Score
} player[10];											   //Array holding player details
void text(int x, int y, char a[], int tf, int ts, int col) // Function to print text
{
	setcolor(col);
	settextstyle(tf, 0, ts);
	settextjustify(1, 1);
	outtextxy(x, y, a);
}
void box(int x1, int y1, int x2, int y2, int c1, int c2) //Function to draw box
{
	setcolor(c1);
	rectangle(x1, y1, x2, y2);
	setfillstyle(1, c2);
	floodfill((x1 + x2) / 2, (y1 + y2) / 2, c1);
}
void polydraw(int n, int a[], int c1, int c2) //Fuction to draw polygon
{
	setcolor(c1);
	setfillstyle(SOLID_FILL, c2);
	fillpoly(n, a);
}
void bardraw(int x1, int y1, int x2, int y2, int co) //Function to draw bar
{
	setfillstyle(SOLID_FILL, co);
	bar(x1, y1, x2, y2);
}
void circdraw(int x, int y, int r, int col) //Function to draw circle
{
	setcolor(col);
	setfillstyle(SOLID_FILL, col);
	circle(x, y, r);
	floodfill(x, y, col);
}
float sound_freq(int a) //Function to return the frequency of given note
{
	float f;
	f = 440.0 * pow(2.0, ((a - 49) / 12.0)); //Formula to calculate frequency
	return f;
}
void bigbang() //Function to print Big Bang in initial scene
{
	int a[3] = {45, 48, 51};
	int l = getmaxx(), m = getmaxy();
	for (int i = 153, j = 52; i < l / 2 - 25 && j < m / 2 - 25; i += 4, j += 3)
	{
		sound(sound_freq(a[i % 3]));
		circdraw(i, j, 60, RED);
		circdraw(l - i, j, 60, YELLOW);
		circdraw(i, m - j - 100, 60, RED);
		circdraw(l - i, m - j - 100, 60, YELLOW);
		delay(75);
		nosound();
	}
}
void intscene() //Printing the initial scene
{
	int f[8] = {40, 42, 44, 45, 47, 49, 51, 52};
	char a[50] = "EVERYTHING BEGAN   \nWITH NOTHING.......", h[3];
	settextstyle(8, 0, 0);
	setbkcolor(RED);
	textcolor(0);
	moveto(150, 100);
	for (int i = 0, j = 0; a[i] != '\0'; i++)
	{
		if (i % 5 != 0)
			delay(50);
		if (a[i] == '\n')
			moveto(150, 140);
		else
		{
			h[0] = a[i];
			h[1] = '\0';
		}
		outtext(h);
		if (i % 6 == 0)
		{
			sound(sound_freq(f[j]));
			delay(50);
			nosound();
			j++;
		}
	}
	delay(100);
	sound(sound_freq(f[7]));
	delay(50);
	nosound();
	delay(450);
	cleardevice();
	char b[60] = "THEN THE BIG BANG CREATED \nTHE FIRST ELEMENTS...";
	moveto(50, 200);
	setbkcolor(BROWN);
	settextstyle(8, 0, 0);
	for (i = 0; b[i] != '\0'; i++)
	{
		if (i % 5 != 0)
			delay(50);
		if (b[i] == '\n')
			moveto(80, 240);
		else
		{
			h[0] = b[i];
			h[1] = '\0';
		}
		outtext(h);
		if (i % 6 == 0)
		{
			sound(sound_freq(f[j % 8]));
			delay(50);
			nosound();
			j -= 1;
			if (j == 0)
				j = 8;
		}
	}
	delay(500);
	cleardevice();
	int g[7] = {40, 44, 47, 44, 40, 44, 40};
	char c[50] = "YOUR JOB IS TO  \n CREATE THE REST.....";
	moveto(130, 300);
	setbkcolor(BLUE);
	settextstyle(8, 0, 0);
	for (i = 0, j = 0; c[i] != '\0'; i++)
	{
		if (i % 5 != 0)
			delay(50);
		if (c[i] == '\n')
			moveto(150, 340);
		else
		{
			h[0] = c[i];
			h[1] = '\0';
		}
		outtext(h);
		if (i % 6 == 0)
		{
			sound(sound_freq(g[j % 7]));
			delay(50);
			nosound();
			j += 1;
		}
	}
	delay(500);
	setbkcolor(0);
	cleardevice();
	bigbang();
	cleardevice();
}
void midset() //Random generation of middle atom
{
	int *p = new int(1 + random(100)); //Random variable
	if (score >= 400 && rounds % 25 == antmttrd)
		mid = at[3]; //Sets antimatter
	else if (score >= 300 && rounds % 20 == ntrnrd)
		mid = at[2]; //Sets neutrino
	else if (*p <= 20)
		mid = at[0]; //Sets plus atom
	else if (*p <= 30)
		mid = at[1]; //Sets minus atom
	else if (*p <= 55)
		mid = at[base + 4]; //Sets Base atom
	else if (*p <= 88)
		mid = at[base + 5]; //Sets Base atom + 1
	else if (*p <= 100)
		mid = at[base + 6]; //Sets Base atom + 2
	delete p;
	if (rounds % 30 == 0)
		base++; //Changes Base atom every 30 rounds
}
void boarddraw(int opt = -5) //Function to draw the board
{
	void circdraw(int, int, int, int);
	void bardraw(int, int, int, int, int);
	char a[10];
	if (noatm <= 17)
	{
		mousehide();
		bardraw(75, 75, 180, 150, YELLOW);  //Clear previous score
		itoa(score, a, 10);					//Convert score to string
		text(100, 100, a, 0, 2, 2);			//Print Score
		bardraw(475, 75, 580, 150, YELLOW); //Clear previous rounds
		itoa(rounds, a, 10);				//Convert rounds to string
		text(530, 100, a, 0, 2, 2);			//Print No. Of Rounds
		if (17 - noatm == 3)				//Show warning for last 3 atoms
		{
			bardraw(290, 450, 350, 470, BLUE);
			for (int i = 0; i < 3; i++)
			{
				circdraw(305 + i * 15, 460, 4, LIGHTGREEN);
			}
		}
		else if (17 - noatm == 2) //Show warning for last 2 atoms
		{
			bardraw(290, 450, 350, 470, BLUE);
			for (int i = 0; i < 2; i++)
			{
				circdraw(310 + i * 20, 460, 4, CYAN);
			}
		}
		else if (17 - noatm == 1) //Show warning for last atoms
		{
			bardraw(290, 450, 350, 470, BLUE);
			circdraw(320, 460, 4, RED);
		}
		else //Clear previous warnings
		{
			bardraw(290, 450, 350, 470, YELLOW);
		}
		if (noatm <= 16) //Check for game over
		{

			if (opt == -5)
				midset(); //Sets midatom randomly
			else
				mid = tempatm; //Sets midatom according to minus or neutrino function
			circdraw(crcx, crcy, 25, LIGHTCYAN);
			mid.draw(crcx, crcy); //Draw midatom
			setcolor(WHITE);
			circle(crcx, crcy, 30); //Draw halo
		}
		for (int i = 0; i < noatm; i++) //Draw the atoms in the game
		{
			play[i].draw(crcx + (crrd - 30) * cos(-(360.0 / noatm) * i * (M_PI / 180)), crcy + (crrd - 30) * sin(-(360.0 / noatm) * i * (M_PI / 180)));
		}
	}
	callmouse();
	if (opt != 10) //Puts sound if game doesn't end
	{
		sound(sound_freq(64));
		delay(100);
		nosound();
	}
}
void boardclr(int n) //Function to clear board
{
	mousehide();
	for (int i = 0; i < n; i++) //Overwrites all the existing elements on board
	{
		circdraw(crcx + (crrd - 30) * cos(-(360.0 / n) * i * (M_PI / 180)), crcy + (crrd - 30) * sin(-(360.0 / n) * i * (M_PI / 180)), 22, LIGHTCYAN);
	}
	callmouse();
}
void insertatm(int n) //Insert new atoms into the board
{
	for (int j = noatm; j >= n; j--) // Makes way for the new atom
	{
		play[j] = play[j - 1];
	}
	play[j + 1] = mid; // Sets new atom
	boardclr(noatm);   // Clear board
	noatm++;		   //Increase no. of atoms
}
void initplayset() //Initializes starting atoms on board
{
	for (int i = 0; i < noatm; i++) //Sets playatoms
	{
		play[i] = at[4 + random(3)];
	}
}
int pluscheck() //Checks whether the plus atom can combine the atoms
{
	specatom *p = NULL, *base = NULL, *back = NULL, *front = NULL, *temp = NULL, *pre = NULL;
	int maxatm = 0, combine = 0;
	/*
		 Pointers uses
		 p 	- General pointer used mainly for creating linked list
		 base	- Stores base address
		 back	- Stores the address of previous atom
		 front	- Stores the address of next atom
		 temp 	- Used to store address of temporary atoms
		 pre 	- Used to store address of previous atom while creating linked list

		 Variables uses
		 maxatm - Used to store atomic no. of amximum atom
		 combine- Check whether any combination of atoms has taken place
	*/
	for (int i = 0, chk = 0; chk < noatm; i++, chk++) //Creates linked list
	{
		p = new specatom;
		*p = play[i];
		p->flink = p->blink = NULL;
		if (base == NULL)
			base = p;
		else
		{
			pre->flink = p;
			p->blink = pre;
		}
		pre = p;
	}
	base->blink = pre;
	pre->flink = base;
	p = base; // Commands connect the end points to create a circular linked list
	do
	{
		int comb = 0;
		back = p->blink;			  //Stores previous atom address
		front = p->flink;			  //Stores next atom address
		if ((p->return_atno()) == -3) //Checks whether the atom is plus
		{
			int flag = 1; //Used to check whether the symmetry is complete
			do
			{
				if ((back->return_atno() == front->return_atno()) && (back->return_atno() != -3) && (noatm >= 3))
				//Condition checks whether symmetry exists, and whether the previous element is not plus
				{
					if (p->return_atno() > back->return_atno())
						maxatm = p->return_atno(); //Sets the atomic number of highest atom
					else
						maxatm = back->return_atno();
					*p = at[maxatm + 4];			 //Stores the value of maximum atom
					score += 4 + (p->return_atno()); //Increases score
					p->flink = front->flink;		 //Changes the location of front atom
					front->flink->blink = p;		 //Sets the front atom
					temp = front;					 //Temporarily stores the old front atom
					if (temp == base)
						base = p;		  //Changes base if base was the old atom
					front = front->flink; //Does the other front changes accordingly
					delete temp;		  //Deletes old front atom
					p->blink = back->blink;
					back->blink->flink = p;
					temp = back;
					if (temp == base)
						base = p;
					back = back->blink;
					delete temp;	 //Same for previous atoms as done for front ones
					boardclr(noatm); //Clears the board
					noatm -= 2;		 //Reduces no. of atoms
					combine = 1;
					comb = 1; //Variables checks whether cominations have occured
				}
				else
					flag = 0; // Declares no more symmetry left
			} while (flag == 1);
		}
		if (comb == 1 && p->blink->return_atno() == -3) //Goes back if there is a plus atom before
		{
			p = p->blink;
			comb = 0;
		}
		else
			p = p->flink; //Goes to next atom
	} while (p != base);
	p = base;
	for (i = 0; i < noatm; i++, p = p->flink) //Resets play aom
	{
		play[i] = *p;
	}
	do //Deletes all dynamic variables
	{
		delete (p);
		p = p->flink;
	} while (p != base);
	return (combine);
}
void antimatter()
{
	specatom *p = NULL, *base = NULL, *back = NULL, *front = NULL, *temp = NULL, *pre = NULL;
	int maxatm = 0, init = 0;
	/*
			Pointers and maxatm same as above
			init	- Checks for th first combination
		*/
	//Most functions same as plus
	for (int i = 0, chk = 0; chk < noatm; i++, chk++)
	{
		p = new specatom;
		*p = play[i];
		p->flink = p->blink = NULL;
		if (base == NULL)
			base = p;
		else
		{
			pre->flink = p;
			p->blink = pre;
		}
		pre = p;
	}
	base->blink = pre;
	pre->flink = base;
	p = base;
	do
	{
		back = p->blink;
		front = p->flink;
		if ((p->return_atno()) == 0)
		{
			int flag = 1;
			do
			{
				if (init == 0) //Checks for initial combination, where any two can combine
				{
					if (front->return_atno() > back->return_atno())
						maxatm = front->return_atno();
					else
						maxatm = back->return_atno();
					*p = at[maxatm + 7];
					score += 4 + (p->return_atno());
					p->flink = front->flink;
					front->flink->blink = p;
					temp = front;
					if (temp == base)
						base = p;
					front = front->flink;
					delete temp;
					p->blink = back->blink;
					back->blink->flink = p;
					temp = back;
					if (temp == base)
						base = p;
					back = back->blink;
					delete temp;
					boardclr(noatm);
					noatm -= 2;
					init = 1;
				}
				else if ((back->return_atno() == front->return_atno()) && (back->return_atno() != -3) && (noatm >= 3))
				{
					if (p->return_atno() > back->return_atno())
						maxatm = p->return_atno();
					else
						maxatm = back->return_atno();
					*p = at[maxatm + 4];
					score += 4 + (p->return_atno());
					p->flink = front->flink;
					front->flink->blink = p;
					temp = front;
					if (temp == base)
						base = p;
					front = front->flink;
					delete temp;
					p->blink = back->blink;
					back->blink->flink = p;
					temp = back;
					if (temp == base)
						base = p;
					back = back->blink;
					delete temp;
					boardclr(noatm);
					noatm -= 2;
				}
				else
					flag = 0;
			} while (flag == 1);
		}
		p = p->flink;
	} while (p != base);
	p = base;
	for (i = 0; i < noatm; i++, p = p->flink)
	{
		play[i] = *p;
	}
	do
	{
		delete (p);
		p = p->flink;
	} while (p != base);
}
void createhiscr() //Creates high score file if not present
{
	ofstream f("/CODE/HISCR.DAT", ios::binary);
	hiscr temp = {"PLAYER", 0};
	for (int i = 0; i < 10; i++)
	{
		f.write((char *)&temp, sizeof(temp));
	}
	f.close();
}
void exiterr() //Shows error message if files not found and exits game
{
	bardraw(0, 0, 640, 480, WHITE);
	char a[60] = " Sorry, the required files for the game was not found.", b[50] = "Please load all the files to play the game.", c[50] = "THANK YOU FOR CHOOSING TO PLAY OUR GAME!!";
	text(320, 180, a, 1, 2, 4);
	text(320, 210, b, 1, 2, 4);
	text(320, 270, c, 7, 2, 1);
	circdraw(320, 100, 50, YELLOW);
	setcolor(BLACK);
	circle(320, 100, 51);
	circdraw(300, 85, 5, BLACK);
	circdraw(340, 85, 5, BLACK);
	setcolor(BLACK);
	arc(320, 140, 30, 150, 30);
	text(320, 400, "Press any key to continue.....", 3, 1, 2);
	getch();
	cleardevice();
	exit(0);
}
void set_atomfiles() //Initializes the files required for game.
{
	ifstream f("/CODE/ATOM.txt");				//File which contains atoms data
	ifstream g("/CODE/HISCR.DAT", ios::binary); //File which contains highscores
	if (!f)
		exiterr();
	if (!g)
		createhiscr();
	char na[15], sym[5];
	int atno, co;
	for (int i = 0; i <= 113; i++)
	{
		f >> atno >> sym >> na >> co;
		at[i].set(na, sym, atno, co);
	}
	f.close();
	for (i = 0; i < 10; i++)
	{
		g.read((char *)&player[i], sizeof(player[i]));
	}
	g.close();
}
void sides() //Used to draw part of the background
{
	int m, t[10], i = 0, flag = 0;
	bardraw(0, 0, 50, 500, 15);
	bardraw(590, 0, 640, 500, 15);
	for (i = 1; i < 10; i++)
	{
		do
		{
			flag = 0;
			m = 4 + random(20);
			for (int k = 0; k < i && flag == 0; k++)
			{
				if (t[k] == m)
					flag = 1;
			}
		} while (flag == 1);
		t[i] = m;
		at[m].draw(25, 50 * i);
		at[m].draw(615, 50 * i);
	}
}
void neutrino(int x, int y) //Function for neutrino
{
	int flag = 0;
	for (int i = 0; i < noatm && flag == 0; i++)
	{
		//Condition used to check for the atom clicked.
		if ((pow(x - (crcx + (crrd - 30) * cos(-(360.0 / noatm) * i * (M_PI / 180))), 2) + pow(y - (crcy + (crrd - 30) * sin(-(360.0 / noatm) * i * (M_PI / 180))), 2)) <= 400)
		{
			tempatm = play[i]; //Sets the temporary atom as the atom clicked
			boarddraw(1);
			flag = 1;
		}
	}
}
void minus(int x, int y) //Function for minus atom
{
	int flag = 0;
	for (int i = 0; i < noatm && flag == 0; i++)
	{
		//Condition to check for what atom is clicked.
		if ((pow(x - (crcx + (crrd - 30) * cos(-(360.0 / noatm) * i * (M_PI / 180))), 2) + pow(y - (crcy + (crrd - 30) * sin(-(360.0 / noatm) * i * (M_PI / 180))), 2)) <= 400)
		{
			flag = 1;
			tempatm = play[i]; //Sets temporary atom as the atom clicked
		}
	}
	if (flag == 1)
	{
		i--;
		for (int j = i; j < noatm; j++) //Deleted the selected atom from the board
		{
			play[j] = play[j + 1];
		}
		boardclr(noatm); //Clear board
		noatm--;		 //Reduces no. of atoms
		pluscheck();	 //Checks for any combinations
		boarddraw(1);	//Draw board
	}
}
void nameget(char a[]) //Function to get the name of the player with highscore
{
	char j;
	int f[4][2] = {64, 450, 62, 300, 61, 150, 64, 450};
	mousehide();
	bardraw(50, 0, 590, 480, LIGHTGREEN);
	text(320, 150, "Congrats!! You got a highscore!!", 5, 4, RED);
	for (int i = 0; i < 4; i++)
	{
		sound(sound_freq(f[i][0]));
		delay(f[i][1]);
		nosound();
	}
	text(320, 200, "Enter You Name : ", 5, 4, RED);
	for (i = 0; j != '\r';) //Function to get name
	{
		j = getch();
		if (i < 10 && j != '\b')
		{

			sound(1000);
			delay(50);
			nosound();
			a[i] = j;
			i++;
		}
		else if (j == '\b' && i > 0)
			i--;
		a[i] = '\0';
		bardraw(50, 240, 500, 300, LIGHTGREEN);
		text(320, 250, a, 5, 4, BLUE);
	}
}
void writescr() //Rewrites the highscore data
{
	ofstream f("/CODE/HISCR.DAT", ios::binary);
	f.write((char *)player, 10 * sizeof(player[0]));
	f.close();
}
void append(char a[], char b[]) //Function to append a given string to another string
{
	int n = strlen(a);
	a[n] = ' ';
	for (int i = 0, j = n + 1; b[i] != '\0'; i++, j++)
	{
		a[j] = b[i];
	}
	a[j] = '\0';
}
void help() //Prints help
{
	int x = 0, y = 0, cl = 0, flag = 1;
	mousehide();
	sound(1000);
	delay(50);
	nosound();
	bardraw(50, 0, 590, 480, YELLOW);
	text(crcx, 20, "HELP", 7, 5, BROWN);
	setlinestyle(1, 1, 5);
	line(240, 45, 390, 45);
	box(610, 3, 630, 23, MAGENTA, RED);
	text(621, 10, "�", 2, 8, 0);
	char txt[200] = "* Objective";
	settextjustify(0, 0);
	setcolor(1);
	settextstyle(7, 0, 3);
	outtextxy(70, 70, txt);
	strcpy(txt, "The main objective of this game is to combine the atoms and get ");
	setcolor(0);
	settextstyle(0, 0, 1);
	outtextxy(80, 90, txt);
	strcpy(txt, "the highest score possible.");
	outtextxy(80, 105, txt);
	strcpy(txt, "* How to play");
	setcolor(1);
	settextstyle(7, 0, 3);
	outtextxy(70, 130, txt);
	strcpy(txt, "Simple, you just place the atom which you get anywhere on the ");
	setcolor(0);
	settextstyle(0, 0, 1);
	outtextxy(80, 150, txt);
	strcpy(txt, "board, but make sure you keep the arrangement of the atoms ");
	outtextxy(80, 165, txt);
	strcpy(txt, "symmetrical! And the game ends when the board has more than");
	outtextxy(80, 180, txt);
	strcpy(txt, "17 atoms. So keep calm and go play the game!!");
	outtextxy(80, 195, txt);
	strcpy(txt, "* Special Atoms");
	setcolor(1);
	settextstyle(7, 0, 3);
	outtextxy(70, 220, txt);
	for (int i = 0; i <= 3; i++)
	{
		at[i].draw(80, 285 + 40 * i, 15);
		setcolor(RED);
		settextjustify(0, 0);
		settextstyle(3, 0, 1);
		outtextxy(100, 290 + 40 * i, at[i].retrn_na());
	}
	at[4].draw(80, 245, 15);
	settextjustify(0, 0);
	settextstyle(3, 0, 1);
	setcolor(RED);
	outtextxy(100, 250, "Atom");
	setcolor(0);
	settextstyle(2, 0, 4);
	strcpy(txt, "-Main source of points in the game. Place them symmetrical to get more points");
	outtextxy(146, 245, txt);
	strcpy(txt, "-When placed in between similar atoms, will cause a chain reaction depending ");
	outtextxy(136, 280, txt);
	strcpy(txt, "on symmetrical placement to give a higher atom and some points");
	outtextxy(137, 295, txt);
	strcpy(txt, "-Used to remove an atom from the board, by clicking on it. Can convert the ");
	outtextxy(147, 320, txt);
	strcpy(txt, "removed atom into Plus by clicking on it again.");
	outtextxy(150, 335, txt);
	strcpy(txt, "-Used to copy an atom in the board by clicking on it. Comes after");
	outtextxy(175, 360, txt);
	strcpy(txt, " you get 300 points.");
	outtextxy(175, 375, txt);
	strcpy(txt, "-Can combine any to atoms when placed between, then acts like");
	outtextxy(195, 400, txt);
	strcpy(txt, " the Plus atom. Comes after 400 points");
	outtextxy(195, 415, txt);
	setcolor(GREEN);
	settextstyle(10, 0, 1);
	outtextxy(70, 470, "EASY TO PLAY, HARD TO MASTER");
	callmouse();
	do //Calling mouse
	{
		mouseposi(x, y, cl);
		if (cl == 1)
		{
			if (x >= 610 && x <= 630 && y >= 3 && y <= 23)
			{
				flag = 0;
				sound(1000);
				delay(50);
				nosound();
			}
		}
	} while (flag == 1);
}
void spchck(char *a) //Checks the spaces in the text
{
	int l = strlen(a), j = 5;
	for (int i = 0; (l - i) >= 0; i++)
	{
		a[j - i] = a[l - i];
	}
	for (int k = 0; k <= j - i; k++)
	{
		a[k] = ' ';
	}
}
void hscr() //Function to print highscores.
{
	char a[2], txt[100] = "\0";
	int x = 0, y = 0, cl = 0, flag = 1;
	mousehide();
	sound(1000);
	delay(50);
	nosound();
	bardraw(50, 0, 590, 480, YELLOW);
	text(crcx, 20, "HIGH SCORES", 3, 7, MAGENTA);
	setlinestyle(1, 1, 5);
	line(120, 55, 510, 55);
	box(610, 3, 630, 23, MAGENTA, RED);
	text(621, 10, "�", 2, 8, 0);
	for (int i = 0; i < 10; i++)
	{
		moveto(70, 100 + 40 * i);
		settextjustify(0, 0);
		setcolor(BROWN);
		settextstyle(9, 0, 1);
		itoa(i + 1, a, 10);
		append(txt, a);
		if (strlen(txt) == 2)
		{
			txt[2] = ' ';
			txt[3] = '\0';
		}
		append(txt, player[i].na);
		outtext(txt); //Prints name
		strcpy(txt, "\0");
		itoa(player[i].scr, txt, 10);
		spchck(txt);
		outtextxy(430, 100 + 40 * i, txt); //Prints score
		setcolor(BLACK);
		line(65, 70 + 40 * i, 530, 70 + 40 * i); //Prints lines for table
		strcpy(txt, "\0");
	}
	//Function to print lines for table
	line(65, 70 + 40 * i, 530, 70 + 40 * i);
	line(65, 70, 65, 70 + 40 * i);
	line(130, 70, 130, 70 + 40 * i);
	line(420, 70, 420, 70 + 40 * i);
	line(530, 70, 530, 70 + 40 * i);
	callmouse();
	do //Calling mouse
	{
		mouseposi(x, y, cl);
		if (cl == 1)
		{
			if (x >= 610 && x <= 630 && y >= 3 && y <= 23)
			{
				flag = 0;
				sound(1000);
				delay(50);
				nosound();
			}
		}
	} while (flag == 1);
}
void game_over() //Funtion to display gameover
{
	mousehide();
	int f[5] = {40, 39, 38, 37, 36};
	circdraw(crcx, crcy, 40, YELLOW);
	circdraw(crcx, crcy, 40, LIGHTCYAN);
	text(320, 250, "GAME OVER", 4, 4, MAGENTA);
	for (int i = 0; i <= 3; i++)
	{
		sound(sound_freq(f[i]));
		delay(500);
		nosound();
	}
	callmouse();
}
int back_menu() //Function to check whether user really wishes to quit
{
	mousehide();
	char a;
	text(320, 470, "Are You Sure To Quit? (Y/N)", 0, 2, 4);
	do
	{
		a = getch();
		a = tolower(a);
	} while (a != 'y' && a != 'n');
	callmouse();
	text(320, 470, "Are You Sure To Quit? (Y/N)", 0, 2, 14);
	if (a == 'y')
	{
		game_over();
		return 1;
	}
	else
		return 0;
}
void game() //Function which is used to call the game
{
	void intro();
	int x = 0, y = 0, xpos = 0, ypos = 0, cl = 0, b = 0, minuschck = 0, antmttrchck = 0, maxchck = 1;
	double rad, dis;
	char na[20];
	/*
			Variables:
			x	- Get the horizontal position of the mouse
			y	- Get the vertical position of the mouse
			xpos	- Holds the x position of the mouse
			ypos 	- Holds the y position of the mouse
			cl	- Checks for click
			b	- Checks for user option
		       minuschck- Checks for negative atom status
		       ant...hck- Checks for antimatter status
		       maxchck	- Checks for maximum atom
		       rad	- Angle made by atom with x-axis
		       dis	- Distance of the atom from origin
		*/
	specatom max; //Maxatom
	base = 0;
	rounds = 1;
	mousehide();
	score = 0;
	bardraw(50, 0, 590, 500, 14);
	circdraw(crcx, crcy, crrd + 5, LIGHTBLUE);
	box(610, 3, 630, 23, WHITE, WHITE);
	for (int i = 0; i <= crrd; i++) //Draws the board with effect
	{
		circdraw(crcx, crcy, i, LIGHTCYAN);
		sound(sound_freq(45) + 2 * i);
		delay(10);
		nosound();
	}
	noatm = 5;
	initplayset();
	bardraw(50, 0, 590, 500, 14);
	circdraw(crcx, crcy, crrd + 5, LIGHTBLUE);
	circdraw(crcx, crcy, crrd, LIGHTCYAN);
	text(105, 50, "Score", 0, 2, 2);
	text(535, 50, "Rounds", 0, 2, 2);
	box(610, 3, 630, 23, MAGENTA, RED);
	text(621, 10, "�", 2, 8, 0); //All the previous lines used to draw background
	callmouse();
	int a[3] = {40, 48, 64}, fl; //Variable containing sound frequencies and flag
	boarddraw();
	do
	{
		if (maxchck == 1) //Checks for maximum atom
		{
			for (int i = 0; i < noatm; i++)
			{
				if (max.return_atno() < play[i].return_atno())
				{
					max = play[i]; //Sets max atom
					fl = 1;		   //Resets flag
				}
			}
			if (noatm > 16) //Condition to check for game over
			{
				game_over();
				b = 1;
			}
			bardraw(200, 0, 450, 80, YELLOW);
			text(320, 20, max.retrn_na(), 7, 4, 3); //To print maxatom
			delay(100);
			if (fl == 1) //Sound for the new maxatom
			{
				for (int j = 0; j < 3; j++)
				{
					sound(sound_freq(a[j]));
					delay(100);
					nosound();
				}
				fl = 0;
			}
			maxchck = 0;
		}
		delay(20);
		if (score >= 300 && ntrnrd == -1)
			ntrnrd = (rounds % 20) + 1; //Sets the neutrino
		if (score >= 400 && antmttrd == -1)
			antmttrd = (rounds % 25) + 1; //Sets the antimatter
		else
		{
			mouseposi(x, y, cl);					 //Gets mouse data
			xpos = (x - crcx);						 //Gets x position
			ypos = -(y - crcy);						 //Gets y position
			dis = sqrt(pow(xpos, 2) + pow(ypos, 2)); //Gets distance from center
			if (dis != 0)
				rad = (180.0 / M_PI) * acos(xpos / dis); //Angle from x-axis
			if (ypos < 0)
				rad = 360 - rad; // Changes angle for negative axis
			if (cl == 1)		 // If user clicks...
			{

				if (x >= 610 && x <= 630 && y >= 3 && y <= 23) //Exit
				{
					sound(1000);
					delay(100);
					nosound();
					b = back_menu();
				}
				else if (mid.return_atno() == -1)
					neutrino(x, y);				  //Neutrino
				else if (mid.return_atno() == -2) //Minus
				{
					minus(x, y);
					minuschck = 1;
				}
				else if (minuschck == 1 && (pow((x - crcx), 2) + pow((y - crcy), 2) <= 400)) //Minus check
				{
					tempatm = at[0];
					boarddraw(5);
					minuschck = 0;
				}
				else if (!(x >= 610 && x <= 630 && y >= 3 && y <= 23)) //Puts the atom on screen
				{
					rounds++;
					int *p = new int(0);
					if (mid.return_atno() == 0)
						antmttrchck = 1;
					for (int i = 1; i - 1 <= noatm && *p == 0; i++) //Checks for position where atom to be inserted
					{
						if (noatm == 0) //If no atoms present
						{
							insertatm(0);
							boarddraw();
							*p = 1;
						}
						else if ((rad >= (360 / noatm) * (i - 1)) && (rad <= (360 / noatm) * (i))) //If atom is present
						{
							insertatm(i);
							boarddraw();
							*p = 1;
						}
					}
					delete (p);
					if (antmttrchck == 1) //Antimatter check
					{
						antimatter();
						boarddraw();
						antmttrchck = 0;
					}
					if (pluscheck() == 1)
						boarddraw(); //Pluscheck
					maxchck = 1;
				}
				else
					b = 0;
			}
		}
	} while (b != 1);
	if (score > player[9].scr) //Check for highscore
	{
		nameget(na);
		for (int i = 0, flag = 0; i < 10 && flag == 0; i++)
		{
			if (score > player[i].scr)
				flag = 1;
		}
		i--;
		for (int j = 9; j > i; j--)
		{
			player[j] = player[j - 1];
		}
		strcpy(player[j].na, na);
		player[j].scr = score;
		writescr();
	}
	callmouse();
}
void center() //Draw the "ATOMAS" center
{
	bardraw(50, 0, 590, 500, 0);
	if (introchck == 1)
	{
		for (int i = 0; i <= 125; i += 1)
		{
			circdraw(crcx, 155, i, 4);
			sound(sound_freq(45) + 2 * i);
			delay(20);
			nosound();
		}
	}
	bardraw(50, 0, 590, 500, 14);
	setcolor(0);
	ellipse(320, 155, 0, 360, 150, 68);
	ellipse(320, 155, 0, 360, 68, 150);
	circdraw(320, 155, 130, 2);
	circdraw(320, 155, 125, 4);
	text(320, 140, "ATOMAS", 9, 4, 7);
	setcolor(0);
	ellipse(320, 155, 52, 309, 68, 150);
	ellipse(320, 155, 0, 218, 150, 68);
	circdraw(470, 150, 15, 3);
	circdraw(320, 305, 15, 3);
	if (introchck == 1)
	{
		sound(1000);
		delay(250);
		nosound();
	}
	introchck = 0;
}
void introscreen() //Introduction screen
{
	mousehide();
	center();
	bardraw(210, 350, 450, 400, 10);
	text(330, 370, "PLAY", 7, 0, 6);
	box(210, 350, 450, 400, 6, 10);
	sides();
	box(610, 3, 630, 23, MAGENTA, RED);
	text(621, 9, "X", 3, 3, 0);
	box(60, 10, 200, 40, LIGHTBLUE, LIGHTRED);
	text(130, 23, "HIGH SCORE", 2, 7, BLUE);
	box(440, 10, 580, 40, LIGHTRED, LIGHTCYAN);
	text(515, 23, "HELP", 2, 7, RED);
	callmouse();
}
void endmusic() //Plays the exit music
{
	int f[5][2] = {64, 300, 64, 200, 59, 100, 56, 300, 64, 300};
	for (int i = 0; i < 5; i++)
	{
		sound(sound_freq(f[i][0]));
		delay(f[i][1]);
		nosound();
	}
}
int exitfn() //Checks for exit of user
{
	int x, y, cl, flag = -1;
	mousehide();
	bardraw(0, 0, 640, 480, 14);
	text(320, 150, "Do You Want To Exit?", 4, 0, 1);
	box(100, 250, 250, 300, BLUE, LIGHTMAGENTA);
	box(400, 250, 550, 300, BLUE, LIGHTMAGENTA);
	text(175, 270, "YES", 1, 0, 6);
	text(480, 270, "NO", 1, 0, 6);
	callmouse();
	do
	{
		mouseposi(x, y, cl);
		if (cl == 1)
		{
			if (x >= 100 && x <= 250 && y >= 250 && y <= 300) //YES
			{
				mousehide();
				flag = 1;
				bardraw(0, 0, 640, 480, 14);
				text(320, 240, "THANK YOU FOR PLAYING!!!", 7, 0, 6);
				endmusic();
			}
			else if (x >= 400 && x <= 550 && y >= 250 && y <= 300) //NO
			{
				sound(1000);
				delay(50);
				nosound();
				flag = 5;
			}
		}
	} while (flag == -1);
	return flag;
}
void menu() //Menu
{
	int x = 0, y = 0, cl = 0;
	int set = 1, b = 0;
	do
	{
		if (set == 1)
			introscreen();
		mouseposi(x, y, cl);
		if (cl == 1)
		{
			if (x >= 610 && x <= 630 && y >= 3 && y <= 23) //Exit
			{
				sound(1000);
				delay(50);
				nosound();
				b = 1;
			}
			else if (x >= 210 && x <= 450 && y >= 350 && y <= 400)
				b = 2; //Play
			else if (x >= 60 && x <= 200 && y >= 10 && y <= 40)
				b = 3; //Highscore
			else if (x >= 440 && x <= 580 && y >= 10 && y <= 40)
				b = 4; //Help
		}
		switch (b)
		{
		case 1:
			delay(200);
			b = exitfn();
			break; //Exit
		case 2:
			game();
			delay(200);
			break; //Gameplay
		case 3:
			hscr();
			delay(200);
			break; //Highscore
		case 4:
			help();
			delay(200);
			break; //Help
		}
		//Checking whether to draw introscreen again
		if (b < 2)
			set = 0;
		else
			set = 1;
		if (b != 1)
			b = 0;
		cl = 0;
	} while (b != 1);
	if (b == 1) //Exit when b=1
	{
		delay(200);
		cleardevice();
		closegraph();
		exit(0);
	}
}
void main() //Calling all required functions
{
	int gd = DETECT, gm = 2, x = 400, y = 100;
	initgraph(&gd, &gm, "\\TURBOC3\\BGI");
	initmouse();
	delay(100);
	randomize();
	set_atomfiles();
	cleardevice();
	intscene();
	setbkcolor(0);
	callmouse();
	setposi(x, y);
	menu();
}