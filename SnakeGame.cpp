#include<iostream>
#include<conio.h>
#include <windows.h>
#include<time.h>
#include<ctime>
#define BLACK 0
#define WHITE 15
#define GREEN 2
#define RED 4
#define LBLUE 9
#define GRAY 8
#define BLUE 1
#define YELLOW 6
#define PURPLE 5
#define AQUA 3
#define CLASSIC 1
#define CAMPAIGN 2
#define MARATHON 3
#define RACE 4
#define WALLED 5
#define LVL1 1
#define LVL2 2
#define LVL3 3
#define LVL4 4
#define LVL5 5
#define OnePlayerSleep 60
#define TwoPlayerSleep 30
#define ThreePlayerSleep 20
#define FourPlayerSleep 10
const int UP = 1, DOWN = 2, LEFT = 3, RIGHT = 4;
const int BGC = BLACK;
using namespace std;
void SetClr(int tcl, int bcl)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (tcl + (bcl * 16)));
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void printRectangle(int ri, int ci, string function)
{
	char sym = -37;
	gotoRowCol(ri - 1, ci);
	cout << function;
	for (int i = ri; i < ri + 2; i++)
	{
		for (int j = ci; j < ci + 9; j++)
		{
			gotoRowCol(i, j);
			cout << sym;
		}
	}
}
struct pos
{
	int ri, ci;
};
struct snake
{
	pos* ps;
	char sym;
	char hsym;
	int size;
	int direction;
	int score;
	int LKey, RKey, UPKey, DKey;
	int color;
	int playerNo;
};
struct fruit
{
	pos ps;
	char sym;
	int color;
};
struct brick
{
	char sym;
	pos* ps;
	char bhsym;
	char bvsym;
	int bclr;
	int size;
	bool *porous;
	bool* penetrated;
};
void displayGround(int row, int col)
{
	for (int i = 20; i < row + 20; i++)
	{
		for (int j = 40; j < col + 40; j++)
		{
			gotoRowCol(i, j);
			SetClr(BGC, BGC);
			cout << (char)-37;
		}
	}
	SetClr(WHITE, BLACK);
}
void displayBox(char* msg, int row, int col, int boxColor, int msgColor)
{
	for (int i = row; i < row + 3; i++)
	{
		for (int j = col; j < col + 10; j++)
		{
			gotoRowCol(i,j);
			SetClr(boxColor, 0);
			if (i - row == 1)
			{
				cout << (char)-37;
				SetClr(msgColor, boxColor);
				cout << msg;
				SetClr(boxColor, 0);
				cout << (char)-37;
				break;
			}
			SetClr(boxColor, 0);
			cout << (char)-37;

		}
	}
	SetClr(15, 0);
}
bool inRange(int rmax, int rmin, int cmax, int cmin, int r, int c)
{
	return r >= rmin && r <= rmax && c >= cmin && c <= cmax;
}
void displaySnake(const snake& S)
{
	
	for (int i = 0; i < S.size; i++)
	{
		if (i == 0)
		{
			gotoRowCol(S.ps[0].ri, S.ps[0].ci);
			SetClr(S.color, BLACK);
			
			cout << S.hsym;
			continue;
		}
		gotoRowCol(S.ps[i].ri, S.ps[i].ci);
		SetClr(BLACK, S.color);
		cout << S.sym;
	}
	SetClr(WHITE, BLACK);
	gotoRowCol(42, 0);
}
void changeDirection(char button, snake*& S, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (button == S[i].LKey && S[i].direction != RIGHT)
		{
			S[i].direction = LEFT;
		}
		if (button == S[i].RKey && S[i].direction != LEFT)
		{
			S[i].direction = RIGHT;
		}
		if (button == S[i].DKey && S[i].direction != UP)
		{
			S[i].direction = DOWN;
		}
		if (button == S[i].UPKey && S[i].direction != DOWN)
		{
			S[i].direction = UP;
		}
	}
}
void MoveSnake(snake& S, int row, int col)
{
	for (int i = S.size - 1; i >= 1; i--)
	{
		S.ps[i] = S.ps[i - 1];
	}
	switch (S.direction)
	{
	case UP:
		S.hsym = 30;
		S.ps[0].ri--;
		if (S.ps[0].ri == 19)
		{
			S.ps[0].ri = row+20 - 1;
		}
		break;
	case DOWN:
		S.ps[0].ri++;
		S.hsym = 31;
		if (S.ps[0].ri == row+20)
		{
			S.ps[0].ri = 20;
		}
		break;
	case LEFT:
		S.hsym = 17;
		S.ps[0].ci--;
		if (S.ps[0].ci == 39)
		{
			S.ps[0].ci = col + 40 - 1;
		}
		S.hsym = 17;
		break;
	case RIGHT:
		S.hsym = 16;
		S.ps[0].ci++;
		if (S.ps[0].ci == col+40)
		{
			S.ps[0].ci = 40;
		}
		break;
	}
}
void clearSnake(const snake& S)
{
	//gotoRowCol(S.ps[0].ri, S.ps[0].ci);
	for (int i = 0; i < S.size; i++)
	{
		gotoRowCol(S.ps[i].ri, S.ps[i].ci);
		SetClr(BGC, BGC);
		cout << (char)-37;
	}
	SetClr(WHITE, BLACK);
}
bool validSpawn(const fruit& f, snake* S, int size, const brick& wall, const fruit& f1, const fruit& f2)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < S[i].size; j++)
		{
			if (S[i].ps[j].ri == f.ps.ri && S[i].ps[j].ci == f.ps.ci)
			{
				return false;
			}
		}
	}
	for (int i = 0; i < wall.size; i++)
	{
		if (wall.ps[i].ri == f.ps.ri && wall.ps[i].ci == f.ps.ci)
		{
			return false;
		}
	}
	if (f.ps.ri == f1.ps.ri && f.ps.ci == f1.ps.ci)
	{
		return false;
	}
	return true;
}
void fruitRegenerate(fruit& f, snake* S, int size, int row, int col, const brick& wall, const fruit& f1, const fruit& f2)
{
	srand(time(0));
	do
	{
		f.ps.ri = rand() % (row - 1) + 20;
		f.ps.ci = rand() % (col - 1) + 40;
	} while (!validSpawn(f, S, size, wall, f1, f2));
}
void fruitDisplay(fruit f)
{
	gotoRowCol(f.ps.ri, f.ps.ci);
	SetClr(GREEN, BGC);
	cout << f.sym;
	SetClr(WHITE, BLACK);
	gotoRowCol(42, 0);

}
void BigFruitDisplay(fruit bigFood)
{
	gotoRowCol(bigFood.ps.ri, bigFood.ps.ci);
	SetClr(GREEN, BGC);
	cout << bigFood.sym;
	SetClr(WHITE, BLACK);
	gotoRowCol(42, 0);
}
void BigFruitClear(fruit&bigFood)
{
	gotoRowCol(bigFood.ps.ri, bigFood.ps.ci);
	SetClr(BGC, BGC);
	cout << (char)-37;
	bigFood.ps.ri = 0;
	bigFood.ps.ci = 0;
}
void bigFoodTimer(time_t span)
{
	time_t chk;
	time(&chk);
	gotoRowCol(40, 170);
	cout << "          ";
	gotoRowCol(40, 170);
	for (int i = 9 - (chk - span); i >= 0; i--)
	{
		SetClr(RED, BLACK);
		cout << (char)-37;
	}
	gotoRowCol(0, 0);
}
void bigFoodSpawn(fruit&bigFood, snake* S, int size, int row, int col, time_t start, time_t& span, bool& spawned, const brick& wall, const fruit& f1, const fruit& f2)
{
	time_t chk;
	time(&chk);
	if (!spawned && (chk - start) != 0 && (chk - start) % 20 == 0)
	{
		fruitRegenerate(bigFood, S, size, row, col, wall, f1, f2);
		BigFruitDisplay(bigFood);
		span = chk;
		spawned = true;
	}
	if (spawned && (chk - span) == 10)
	{
		spawned = false;
		BigFruitClear(bigFood);
		time(&chk);
		gotoRowCol(40, 170);
		cout << "          ";
	}
	if (spawned)
	{
		bigFoodTimer(span);
	}
}
void GFruitDisplay(fruit GFood)
{
	gotoRowCol(GFood.ps.ri, GFood.ps.ci);
	SetClr(RED, BGC);
	cout << GFood.sym;
	SetClr(WHITE, BLACK);
	gotoRowCol(42, 0);
}
void GFruitClear(fruit& GFood)
{
	gotoRowCol(60, 170);
	cout << "          ";
	gotoRowCol(GFood.ps.ri, GFood.ps.ci);
	SetClr(BGC, BGC);
	cout << (char)-37;
	GFood.ps.ri = 0;
	GFood.ps.ci = 0;
}
void GFoodTimer(time_t span)
{
	time_t chk;
	time(&chk);
	gotoRowCol(60, 170);
	cout << "          ";
	gotoRowCol(60, 170);
	for (int i = 9 - (chk - span); i >= 0; i--)
	{
		SetClr(AQUA, BLACK);
		cout << (char)-37;
	}
	gotoRowCol(0, 0);
}
void GFoodSpawn(fruit& GFood, snake* S, int size, int row, int col, time_t start, time_t& span, bool& spawned, const brick& wall, const fruit& f1, const fruit& f2)
{
	time_t chk;
	time(&chk);
	if (!spawned && (chk - start) != 0 && (chk - start) % 50 == 0)
	{
		fruitRegenerate(GFood, S, size, row, col, wall, f1, f2);
		GFruitDisplay(GFood);
		span = chk;
		spawned = true;
	}
	if (spawned && (chk - span) == 10)
	{
		spawned = false;
		GFruitClear(GFood);
		time(&chk);
		gotoRowCol(40, 170);
		cout << "          ";
	}
	if (spawned)
	{
		GFoodTimer(span);
	}
}
bool fruitCapture(const snake& S, fruit& f, int row, int col)
{
	return S.ps[0].ri == f.ps.ri && S.ps[0].ci == f.ps.ci;
}
void snakeExpand(snake& S, int expandBy)
{
	for (int i = 0; i < expandBy; i++)
	{
		pos* HA = new pos[S.size + 1];
		for (int i = 0; i < S.size; i++)
		{
			HA[i] = S.ps[i];
		}
		HA[S.size] = S.ps[S.size - 1];
		S.size++;
		delete[]S.ps;
		S.ps = HA;
	}
}
bool snakeKill(const snake& S, const brick& wall)
{
	for (int i = 1; i < S.size; i++)
	{
		if (S.ps[i].ri == S.ps[0].ri && S.ps[i].ci == S.ps[0].ci)
		{
			return true;
		}
	}
	for (int i = 0; i < wall.size; i++)
	{
		if (wall.ps[i].ri == S.ps[0].ri && wall.ps[i].ci == S.ps[0].ci && !wall.porous[i])
		{
			return true;
		}
	}
	return false;
}
bool snakeKillC(const snake& S, const brick& wall)
{
	for (int i = 1; i < S.size; i++)
	{
		if (S.ps[i].ri == S.ps[0].ri && S.ps[i].ci == S.ps[0].ci)
		{
			return true;
		}
	}
	for (int i = 0; i < wall.size; i++)
	{
		if (wall.ps[i].ri == S.ps[0].ri && wall.ps[i].ci == S.ps[0].ci)
		{
			return true;
		}
	}
	return false;
}
void displayPorousWall(brick wall, int row, int col)
{
	for (int i = 0; i < wall.size; i++)
	{
		SetClr(wall.bclr, BLACK);
		gotoRowCol(wall.ps[i].ri, wall.ps[i].ci);
		if (wall.porous[i])
		{
			SetClr(YELLOW, BLACK);
			cout << wall.bhsym;
		}
		else
		{
			cout << wall.bhsym;
		}

	}
	SetClr(WHITE, BLACK);
}
void porousRegenerate(brick& wall)
{
	for (int i = 0; i < wall.size; i++)
	{
		if (wall.penetrated[i] && wall.porous[i])
		{
			gotoRowCol(wall.ps[i].ri, wall.ps[i].ci);
			SetClr(YELLOW, BLACK);
			cout << (char)-37;
		}
	}
}
bool porousWallHit(snake& S, const brick& wall, int row, int col)
{
	for (int i = 0; i < wall.size; i++)
	{
		if (wall.ps[i].ri == S.ps[0].ri && wall.ps[i].ci == S.ps[0].ci && wall.porous[i])
		{
			return true;
		}
	}
	for (int i = 0; i < wall.size; i++)
	{
		for (int j = 0; j < S.size; j++)
		{
			if (S.ps[j].ri == wall.ps[i].ri && S.ps[j].ci == wall.ps[i].ci)
			{
				wall.penetrated[i] = true;
			}
		}
	}
	return false;
	
}
void initSnake(snake*& S, int& size, int row, int col)
{

	for (int i = 0; i < size; i++)
	{
		S[i].sym = 4;
		S[i].hsym = 16;
		S[i].size = 3;
		S[i].score = 0;
		S[i].ps = new pos[S[i].size];
	}
	S[0].LKey = 'a';
	S[0].RKey = 'd';
	S[0].UPKey = 'w';
	S[0].DKey = 's';
	S[0].color = LBLUE;
	S[0].direction = RIGHT;
	S[0].playerNo = 1;
	for (int j = 0; j < S[0].size; j++)
	{
		S[0].ps[j].ri = 20+20;
		S[0].ps[j].ci = col / 2 - j+40;
	}
	if (size > 1)
	{
		S[1].LKey = 'j';
		S[1].RKey = 'l';
		S[1].UPKey = 'i';
		S[1].DKey = 'k';
		S[1].color = BLUE;
		S[1].direction = DOWN;
		S[1].playerNo = 2;
		for (int j = 0; j < S[1].size; j++)
		{
			S[1].ps[j].ri = 10 - j+20;
			S[1].ps[j].ci = 75+40;
		}
	}
	if (size > 2)
	{
		S[2].LKey = '4';
		S[2].RKey = '6';
		S[2].UPKey = '8';
		S[2].DKey = '5';
		S[2].color = RED;
		S[2].direction = UP;
		S[2].playerNo = 3;
		for (int j = 0; j < S[2].size; j++)
		{
			S[2].ps[j].ri = 30 + j+20;
			S[2].ps[j].ci = 20 + 40;
		}
	}
	if (size > 3)
	{
		S[3].LKey = 75;
		S[3].RKey = 77;
		S[3].UPKey = 72;
		S[3].DKey = 80;
		S[3].color = WHITE;
		S[3].direction = LEFT;
		S[3].playerNo = 4;
		for (int j = 0; j < S[2].size; j++)
		{
			S[3].ps[j].ri = 40 + 20;
			S[3].ps[j].ci = 25 + j + 40;
		}
	}

}
void initRowCol(int& row, int& col)
{
	row = 70;
	col = 120;
}
void displayNonFunctionalBoundries(int row, int col)
{
	for (int i = 19; i <= row + 20; i++)
	{
		for (int j = 39; j <= col + 40; j++)
		{
			if (i == 19 || i == row + 20 || j == 39 || j == col + 40)
			{
				gotoRowCol(i, j);
				SetClr(GREEN, BLACK);
				cout << (char)-37;
				SetClr(WHITE, BLACK);
			}
		}
	}
}
void initPorousWall(brick& wall, int row, int col)
{
	wall.bhsym = -37;
	wall.bvsym = -37;
	wall.bclr = PURPLE;
	wall.size = row * 2 + (col - 2) * 2;
	wall.ps = new pos[wall.size];
	wall.porous = new bool[wall.size]{};
	wall.penetrated = new bool[wall.size]{};
	int z = 0;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			
			if (i == 0 || j == 0 || i == row - 1 || j == col - 1)
			{
				if ((i == 0 && j <= (col) / 3) || (i <= (row) / 3) && j == 0 || (i == row - 1 && j <= (col) / 3) || (i >= (2 * (row) / 3) && j == 0) || (i == 0 && j >= 2 * (col) / 3) || (i <= (row) / 3) && j == col - 1 || (i == row - 1 && j >= 2 * (col) / 3) || (i > (2 * (row) / 3) && j == col - 1))
				{
					wall.porous[z] = true;
				}
				wall.ps[z].ri = i + 20;
				wall.ps[z].ci = j + 40;
				z++;
			}

		}
	}
}
void initwall1(brick& wall, int row, int col)
{
	wall.bhsym = 205;
	wall.bvsym = 186;
	wall.bclr = PURPLE;
	wall.size = row * 2 + (col - 2) * 2;
	wall.ps = new pos[wall.size];
	int z = 0;
	for (int i = 20; i < row + 20; i++)
	{
		for (int j = 40; j < col + 40; j++)
		{
			if (i == 20 || j == 40 || i == row + 20 - 1 || j == col + 40 - 1)
			{
				wall.ps[z].ri = i;
				wall.ps[z].ci = j;
				z++;
			}
		}
	}

}
void initwall2(brick& wall, int row, int col)
{
	wall.bhsym = -37;
	wall.bvsym = -37;
	wall.bclr = PURPLE;
	wall.size = 0;
	wall.ps = new pos[wall.size];
	for (int i = 0; i < row; i++)
	{
		for (int j =0; j < col; j++)
		{
			if ((i == 0 && j <= (col) / 3) || (i <= (row) / 3) && j == 0 || (i == row - 1 && j <= (col) / 3) || (i >= (2 * (row) / 3) && j == 0) || (i == 0 && j >= 2 * (col) / 3) || (i <= (row) / 3) && j == col - 1 || (i == row - 1 && j >= 2 * (col) / 3) || (i > (2 * (row) / 3) && j == col - 1))
			{
				pos* TA = new pos[wall.size + 1];
				for (int z = 0; z < wall.size; z++)
				{
					TA[z].ri = wall.ps[z].ri;
					TA[z].ci = wall.ps[z].ci;
				}
				TA[wall.size].ri = i + 20;
				TA[wall.size].ci = j + 40;
				wall.size++;
				delete[]wall.ps;
				wall.ps = TA;
			}

		}
	}

}
void initwall3(brick& wall, int row, int col)
{
	wall.bhsym = -37;
	wall.bvsym = -37;
	wall.bclr = PURPLE;
	wall.size = 0;
	wall.ps = new pos[wall.size];
	int wallcount = 0;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (j % 20 == 10)
			{
				wallcount++;
				if (wallcount % 2 == 0)
				{
					if (i >= row - (row / 3))
					{
						pos* TA = new pos[wall.size + 1];
						for (int z = 0; z < wall.size; z++)
						{
							TA[z].ri = wall.ps[z].ri;
							TA[z].ci = wall.ps[z].ci;
						}
						TA[wall.size].ri = i + 20;
						TA[wall.size].ci = j + 40;
						wall.size++;
						delete[]wall.ps;
						wall.ps = TA;
					}
				}
				else if (i <= row / 4)
				{
					pos* TA = new pos[wall.size + 1];
					for (int z = 0; z < wall.size; z++)
					{
						TA[z].ri = wall.ps[z].ri;
						TA[z].ci = wall.ps[z].ci;
					}
					TA[wall.size].ri = i + 20;
					TA[wall.size].ci = j + 40;
					wall.size++;
					delete[]wall.ps;
					wall.ps = TA;
				}
			}
		}
	}

}
void initwall4(brick& wall, int row, int col)
{
	wall.bhsym = -37;
	wall.bvsym = -37;
	wall.bclr = PURPLE;
	wall.size = 0;
	wall.ps = new pos[wall.size];
	int wallcount = 0;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (i == 0 || j == 0 || i == row - 1 || j == col - 1 || (i == 20||i==50) && j >= 20 && j <= 100)
			{
				pos* TA = new pos[wall.size + 1];
				for (int z = 0; z < wall.size; z++)
				{
					TA[z].ri = wall.ps[z].ri;
					TA[z].ci = wall.ps[z].ci;
				}
				TA[wall.size].ri = i + 20;
				TA[wall.size].ci = j + 40;
				wall.size++;
				delete[]wall.ps;
				wall.ps = TA;
			}
		}
	}

}
void initwall5(brick& wall, int row, int col)
{
	wall.bhsym = -37;
	wall.bvsym = -37;
	wall.bclr = PURPLE;
	wall.size = 0;
	wall.ps = new pos[wall.size];
	int wallcount = 0;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (j == col/2)
			{
				pos* TA = new pos[wall.size + 1];
				for (int z = 0; z < wall.size; z++)
				{
					TA[z].ri = wall.ps[z].ri;
					TA[z].ci = wall.ps[z].ci;
				}
				TA[wall.size].ri = i + 20;
				TA[wall.size].ci = j + 40;
				wall.size++;
				delete[]wall.ps;
				wall.ps = TA;
			}
			if (i == row / 2)
			{
				pos* TA = new pos[wall.size + 1];
				for (int z = 0; z < wall.size; z++)
				{
					TA[z].ri = wall.ps[z].ri;
					TA[z].ci = wall.ps[z].ci;
				}
				TA[wall.size].ri = i + 20;
				TA[wall.size].ci = j + 40;
				wall.size++;
				delete[]wall.ps;
				wall.ps = TA;
			}
			if ((i == (row) / 3) && j <= (col) / 3 || (i == (row/3)*2 && j <= (col) / 3) || (i >= (2 * (row) / 3) && j == col/3) || (i == row/3 && j >= 2 * (col) / 3) || (i <= (row) / 3) && j == 2*(col/3) || (i == 2*(row/3) && j >= 2 * (col) / 3))
			{
				pos* TA = new pos[wall.size + 1];
				for (int z = 0; z < wall.size; z++)
				{
					TA[z].ri = wall.ps[z].ri;
					TA[z].ci = wall.ps[z].ci;
				}
				TA[wall.size].ri = i + 20;
				TA[wall.size].ci = j + 40;
				wall.size++;
				delete[]wall.ps;
				wall.ps = TA;
			}
		}
	}

}
void init(snake*& S, int& size, fruit& f, int row, int col, int& target, int& sleeptime, int& snakeCount, brick& wall, fruit& bigFood, int& mode, int& submode, int& raceTime, int&inSize, int*&scoreList, fruit&GFood)
{
	bool optionSelected = false;
	int r = 0, c = 0;
	char one_player[] = "1 player";
	char two_player[] = "2 player";
	char three_player[] = "3 player";
	char four_player[] = "4 player";
	char walled[] = "walled..";
	char classic[] = "classic.";
	char campaign[] = "campaign";
	char marathon[] = "marathon";
	char race[] = "race....";
	displayBox(one_player, 10, 20, BLUE, RED);
	displayBox(two_player, 10, 40, BLUE, RED);
	displayBox(three_player, 10, 60, BLUE, RED);
	displayBox(four_player, 10, 80, BLUE, RED);
	while(!optionSelected)
	{
		getRowColbyLeftClick(r, c);
		if (inRange(12, 10, 29, 20, r, c))
		{
			size = 1;
			optionSelected = true;
			sleeptime = OnePlayerSleep;
		}
		else if (inRange(12, 10, 49, 40, r, c))
		{
			size = 2;
			optionSelected = true;
			sleeptime = TwoPlayerSleep;
		}
		else if (inRange(12, 10, 69, 60, r, c))
		{
			size = 3;
			optionSelected = true;
			sleeptime = ThreePlayerSleep;
		}
		else if (inRange(12, 10, 89, 80, r, c))
		{
			size = 4;
			optionSelected = true;
			sleeptime = FourPlayerSleep;
		}
	}
	system("cls");
	if (size > 1)
	{
		optionSelected = false;
		displayBox(walled, 10, 20, BLUE, RED);
		displayBox(classic, 10, 40, BLUE, RED);
		while (!optionSelected)
		{
			getRowColbyLeftClick(r, c);
			if (inRange(12, 10, 29, 20, r, c))
			{
				initPorousWall(wall, row, col);
				optionSelected = true;
			}
			else if (inRange(12, 10, 49, 40, r, c))
			{
				brick tempwall{};
				wall = tempwall;
				optionSelected = true;
				mode = CLASSIC;
			}

		}
		optionSelected = false;
	}
	else
	{
		optionSelected = false;
		displayBox(walled, 10, 20, BLUE, RED);
		displayBox(classic, 10, 40, BLUE, RED);
		displayBox(campaign, 10, 60, BLUE, RED);
		while (!optionSelected)
		{
			getRowColbyLeftClick(r, c);
			if (inRange(12, 10, 29, 20, r, c))
			{
				initPorousWall(wall, row, col);
				optionSelected = true;
			}
			else if (inRange(12, 10, 49, 40, r, c))
			{
				brick tempwall{};
				wall = tempwall;
				optionSelected = true;
				mode = CLASSIC;
			}
			else if (inRange(12, 10, 69, 60, r, c))
			{
				optionSelected = true;
				mode = CAMPAIGN;
			}

		}
		optionSelected = false;
	}
	if (size > 1 && mode != CAMPAIGN)
	{
		system("cls");
		displayBox(marathon, 10, 20, BLUE, RED);
		displayBox(race, 10, 40, BLUE, RED);
		while (!optionSelected)
		{
			getRowColbyLeftClick(r, c);
			if (inRange(12, 10, 29, 20, r, c))
			{
				submode = MARATHON;
				system("cls");
				cout << "enter target\n";
				cin >> target;
				optionSelected = true;
			}
			else if (inRange(12, 10, 49, 40, r, c))
			{
				raceTime = 59;
				submode = RACE;
				optionSelected = true;
			}

		}
	}
	inSize = size;
	scoreList = new int[inSize]{};
	system("cls");
	snakeCount = size;
	f.sym = '*';
	bigFood.sym = 1;
	GFood.sym = 16;
	S = new snake[size];
	initSnake(S, size, row, col);
	fruitRegenerate(f, S, size, row, col, wall, bigFood, GFood);
}
void displayControls(snake* S, int size)
{
	if (size >0)
	{
		cout << "Player1:" << " up='w' down='s' left=='a' right=='d'" << endl;
		SetClr(S[0].color,BLACK);
		gotoRowCol(2, 3);
		cout << S[0].hsym;
		SetClr(BLACK, S[0].color);
		gotoRowCol(2, 2);
		cout << S[0].sym;
		gotoRowCol(2, 1);
		cout << S[0].sym;
		gotoRowCol(2, 0);
		cout << S[0].sym;
		cout << endl;
		SetClr(WHITE, BLACK);
	}
	if (size > 1)
	{
		cout << "Player2:" << " up='i' down='k' left=='h' right=='l'"<<endl;
		SetClr(S[1].color, BLACK);
		gotoRowCol(5, 3);
		cout << S[1].hsym;
		SetClr(BLACK, S[1].color);
		gotoRowCol(5, 2);
		cout << S[1].sym;
		gotoRowCol(5, 1);
		cout << S[1].sym;
		gotoRowCol(5, 0);
		cout << S[1].sym;
		cout << endl;
		SetClr(WHITE, BLACK);
	}
	if (size > 2)
	{
		cout << "Player3:" << " up='8' down='5' left=='4' right=='6'" << endl;
		SetClr(S[2].color, BLACK);
		gotoRowCol(8, 3);
		cout << S[2].hsym;
		SetClr(BLACK, S[2].color);
		gotoRowCol(8, 2);
		cout << S[2].sym;
		gotoRowCol(8, 1);
		cout << S[2].sym;
		gotoRowCol(8, 0);
		cout << S[2].sym;
		cout << endl;
		SetClr(WHITE, BLACK);
	}
	if (size > 3)
	{
		cout << "Player4:" << " up='"<<(char)30<<"' down='"<<(char)31<<"' left=='"<<(char)17<<"' right=='"<<(char)16<<"'" << endl;
		SetClr(S[3].color, BLACK);
		gotoRowCol(11, 3);
		cout << S[3].hsym;
		SetClr(BLACK, S[3].color);
		gotoRowCol(11, 2);
		cout << S[3].sym;
		gotoRowCol(11, 1);
		cout << S[3].sym;
		gotoRowCol(11, 0);
		cout << S[3].sym;
		cout << endl;
		SetClr(WHITE, BLACK);
	}
	_getch();
	system("cls");
}
void displayScore(snake*& S, int size)
{
	for (int i = 0; i < size; i++)
	{
		SetClr(S[i].color, BLACK);
		gotoRowCol(10, 10 + S[i].playerNo * 20);
		cout << "                  ";
		gotoRowCol(10, 10 + S[i].playerNo * 20);
		cout << "player" << S[i].playerNo << " score:" << S[i].score;
	}
}
void displayTarget(int target)
{
	gotoRowCol(46, 0);
	SetClr(AQUA, BLACK);
	cout << "Target:";
	cout << target;
	gotoRowCol(0, 0);
}
void deleteSnakeFromArrsy(snake*& S, int index, int& size)
{
	delete[]S[index].ps;
	for (int i = index; i + 1 < size; i++)
	{
		snake temp=S[i];
		S[i] = S[i + 1];
		S[i + 1] = temp;
	}
	snake* TA = new snake[size - 1];
	for (int i = 0; i < size - 1; i++)
	{
		TA[i] = S[i];
	}
	delete[]S;
	size--;
	S = TA;
}
bool targetachieved(snake* S, int size, int target, int& winnerIndex)
{
	for (int i = 0; i < size; i++)
	{
		if (S[i].score >= target)
		{
			winnerIndex = S[i].playerNo;
			return true;
		}

	}
	return false;
}
void tailHitPenalty(snake* S, snake& s, int size, int index)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (j != i)
			{
				if (S[j].ps[0].ri == S[i].ps[S[i].size - 1].ri && S[j].ps[0].ci == S[i].ps[S[i].size - 1].ci)
				{
					S[j].score--;
				}
			}

		}
	}
}
void sleepAdjust(int size, int& sleeptime)
{
	if (size == 1)
	{
		sleeptime = OnePlayerSleep;
	}
	else if (size == 2)
	{
		sleeptime = TwoPlayerSleep;
	}
	else if (size == 3)
	{
		sleeptime = ThreePlayerSleep;
	}
	else if (size == 4)
	{
		sleeptime = FourPlayerSleep;
	}
}
int findSnake(int snakeNo, snake* S, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (S[i].playerNo == snakeNo)
		{
			return i;
		}
	}
	return -1;
}
bool headHitPenalty(snake* S, snake& s, int&size, int index, int& snakeCount, int& sleeptime)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (j != i)
			{
				if (S[j].ps[0].ri == S[i].ps[0].ri && S[j].ps[0].ci == S[i].ps[0].ci)
				{
					int secondSnake = S[j].playerNo;
					clearSnake(S[i]);
					deleteSnakeFromArrsy(S, i, size);
					snakeCount--;
					j = findSnake(secondSnake, S, size);
					clearSnake(S[j]);
					deleteSnakeFromArrsy(S, j, size);
					snakeCount--;
					sleepAdjust(size, sleeptime);
					return true;
				}
			}

		}
	}
	return false;
}
bool bodyHitPenality(snake*& S, int size, int index, int& snakeCount)
{
	for (int i = 0; i < size; i++)
	{
		for (int x = 1; x < S[i].size; x++)
		{
			if (i != index && S[index].ps[0].ri == S[i].ps[x].ri && S[index].ps[0].ci == S[i].ps[x].ci)
			{
				S[i].score = S[i].score + S[index].score;
				snakeExpand(S[i], S[index].size);
				//S[i].size = S[i].size + S[index].size;
				//clearSnake(S[index]);
				//deleteSnakeFromArrsy(S, index, size);
				//snakeCount--;
				//cout << "body hit";
				return true;
			}
		}
	}
	return false;
}
void displayWall1(const brick& wall, int row, int col)
{
	for (int i = 0; i < wall.size; i++)
	{
		SetClr(wall.bclr, BLACK);
		gotoRowCol(wall.ps[i].ri, wall.ps[i].ci);
		if (i == 0)
		{
			cout << char(201);
		}
		else if (i == col - 1)
		{
			cout << (char)187;
		}
		else if (i == 256)
		{
			cout << (char)200;
		}
		else if (i == wall.size - 1)
		{
			cout << (char)188;
		}
		else if ((i >= 1 && i < col - 1) || (i > 256 && i < wall.size - 1))
		{
			cout << wall.bhsym;
		}
		else
		{
			cout << wall.bvsym;
		}

	}
	SetClr(WHITE, BLACK);
}
void displayWall(const brick& wall, int row, int col)
{
	for (int i = 0; i < wall.size; i++)
	{
		SetClr(wall.bclr, BGC);
		gotoRowCol(wall.ps[i].ri, wall.ps[i].ci);
		cout << wall.bhsym;
	}
}
bool gameEnd(int mode, int submode, snake* players, int size, int target, int& winnerIndex, int raceTime, time_t startTime)
{
	if (submode == MARATHON)
	{
		return targetachieved(players, size, target, winnerIndex);
	}
	else if (submode == RACE)
	{
		time_t ct;
		time(&ct);
		return(ct - startTime == raceTime);
	}
	return false;
}
void displayTime(time_t startTime, int raceTime, bool start)
{
	time_t ct;
	time(&ct);
	gotoRowCol(47, 0);
	SetClr(AQUA, BLACK);
	if (!start)
	{
		cout << "01:00";
	}
	else
	{
		cout << "00:";
		if (raceTime - (ct - startTime) < 10)
		{
			cout << "0" << raceTime - (ct - startTime);
		}
		else
		{
			cout << raceTime - (ct - startTime);
		}
	}
}
void initSnakeForLvl(snake* S, int row, int col)
{
	
	S[0].size = 3;
	for (int j = 0; j < S[0].size; j++)
	{
		S[0].ps[j].ri = row/2 + 10;
		S[0].ps[j].ci = col / 2 - j + 10;
	}
	S[0].direction = RIGHT;
}
void initLvl(snake* players, brick&wall, int& target, int&lvl,int row, int col)
{
	lvl++;
	target += 20;
	initSnakeForLvl(players, row, col);
	if (lvl == LVL1)
	{
		initwall1(wall, row, col);
	}
	else if (lvl == LVL2)
	{
		initwall2(wall, row, col);
	}
	else if (lvl == LVL3)
	{
		initwall3(wall, row, col);
	}
	else if (lvl == LVL4)
	{
		initwall4(wall, row, col);
	}
	else if (lvl == LVL5)
	{
		initwall5(wall, row, col);
	}

}
void camp(snake* players, brick wall, int snakeCount, int sleeptime, int size, int target, int winnerIndex, fruit f, fruit& bigFood, bool snakekill, bool start, int row, int col, char& button, time_t startTime, bool bigFoodSpawned, time_t span, bool end, const fruit& GFood)
{
	int lvl = 0;
	target = 0;
	do
	{
		system("cls");
		displayGround(row, col);
		initLvl(players, wall, target, lvl, row, col);
		displayNonFunctionalBoundries(row, col);
		fruitRegenerate(f, players, size, row, col, wall, bigFood, GFood);
		fruitDisplay(f);
		if (lvl == 1)
		{
			displayWall1(wall, row, col);
		}
		else
		{
			displayWall(wall, row, col);
		}
		do
		{
			if ((_kbhit()))
			{
				button = _getch();
				if (button == -32)
				{
					continue;
				}
				changeDirection(button, players, size);
			}
			for (int i = 0; i < size; i++)
			{
				clearSnake(players[i]);
				MoveSnake(players[i], row, col);
				tailHitPenalty(players, players[i], size, i);
				displayScore(players, size);
				displaySnake(players[i]);
				bigFoodSpawn(bigFood, players, size, row, col, startTime, span, bigFoodSpawned, wall, f, GFood);
				if (fruitCapture(players[i], f, row, col))
				{
					players[i].score++;
					displayScore(players, size);
					snakeExpand(players[i], 1);
					fruitRegenerate(f, players, size, row, col, wall, bigFood, GFood);
					fruitDisplay(f);
				}
				if (bigFoodSpawned && fruitCapture(players[i], bigFood, row, col))
				{
					gotoRowCol(40, 170);
					cout << "          ";
					players[i].score = players[i].score + 10;
					displayScore(players, size);
					snakeExpand(players[i], 1);
					BigFruitClear(bigFood);
					bigFoodSpawned = false;
				}
				if (snakeKillC(players[i], wall) || bodyHitPenality(players, size, i, snakeCount))
				{
					clearSnake(players[i]);
					//deleteSnakeFromArrsy(players, i, size);
					snakeCount--;

					displayWall(wall, row, col);
					if (sleeptime == 25)
					{
						sleeptime *= 2;
					}
					else
					{
						sleeptime *= 5;
					}
					if (snakeCount == 0)
					{
						snakekill = true;
						break;
					}
				}
				Sleep(sleeptime);

			}
			if (!start)
			{
				_getch();
				time(&startTime);
				start = true;
			}
			if (snakekill == true)
			{
				/*clearSnake(players[0]);
				for (int i = 50; i < 53; i++)
				{
					for (int j = 90; j < 120; j++)
					{
						gotoRowCol(i, j);
						cout << (char)-37;
					}
				}*/
				gotoRowCol(51, 95);
				cout << "your score " << players[0].score << "...";
				break;
			}

		} while (!targetachieved(players, size, target, winnerIndex));
		start = false;
		gotoRowCol(0, 0);
		if (snakekill == true)
		{
			break;
			_getch();
		}
	} while (lvl < 5);
	if (!snakekill)
	{
		clearSnake(players[0]);
		gotoRowCol(51, 95);
		cout << "you win!!!";
	}
}
int main()
{
	snake* players{};
	brick wall{};
	int snakeCount = 0;
	int sleeptime = 0;
	int raceTime = 0;
	int size = 0, target = 0, winnerIndex = 0, inSize = 0, * scoreList = nullptr;
	fruit f{};
	fruit bigFood{};
	fruit GFood{};
	bool snakekill = false;
	bool start = false;
	int row = 0, col = 0;
	char button = NULL;
	time_t startTime = 0;
	bool bigFoodSpawned = false;
	bool GFoodSpawned = false;
	time_t Bspan = 0;
	time_t Gspan = 0;
	int mode = 0, submode=0;
	bool end = false;
	int wallDisplayTimes = 0;
	initRowCol(row, col);
	//initwall1(wall, row, col);
	init(players, size, f, row, col, target, sleeptime, snakeCount, wall, bigFood, mode, submode, raceTime, inSize, scoreList, GFood);
	displayControls(players, size);
	if (mode == CAMPAIGN)
	{
		camp(players, wall, snakeCount, sleeptime, size, target, winnerIndex, f, bigFood, snakekill, start, row, col, button, startTime, bigFoodSpawned, Bspan, end, GFood);
	}
	else
	{
		displayNonFunctionalBoundries(row, col);
		displayGround(row, col);
		fruitDisplay(f);
		displayPorousWall(wall, row, col);
		do
		{
			
			if ((_kbhit()))
			{
				button = _getch();
				if (button == -32)
				{
					continue;
				}
				changeDirection(button, players, size);
			}
			for (int i = 0; i < size; i++)
			{
				clearSnake(players[i]);
				MoveSnake(players[i], row, col);
				if (porousWallHit(players[i], wall, row, col))
				{
					players[i].score = players[i].score - 5;
				}
				porousRegenerate(wall);
				displayScore(players, size);
				displaySnake(players[i]);
				if (inSize > 0)
				{
					GFoodSpawn(GFood, players, size, row, col, start, Gspan, GFoodSpawned, wall, f, bigFood);
					if (GFoodSpawned && fruitCapture(players[i], GFood, row, col))
					{
						gotoRowCol(60, 170);
						cout << "          ";
						players[i].score = players[i].score + 5;
						scoreList[players[i].playerNo - 1] = players[i].score;
						displayScore(players, size);
						snakeExpand(players[i], 10);
						GFruitClear(GFood);
						GFoodSpawned = false;
					}
				}
				bigFoodSpawn(bigFood, players, size, row, col, startTime, Bspan, bigFoodSpawned, wall, f, GFood);
				if (fruitCapture(players[i], f, row, col))
				{
					players[i].score++;
					scoreList[players[i].playerNo - 1] = players[i].score;
					displayScore(players, size);
					snakeExpand(players[i], 1);
					fruitRegenerate(f, players, size, row, col, wall, bigFood, GFood);
					fruitDisplay(f);
				}
				if (bigFoodSpawned && fruitCapture(players[i], bigFood, row, col))
				{
					gotoRowCol(40, 170);
					cout << "          ";
					players[i].score = players[i].score + 10;
					scoreList[players[i].playerNo - 1] = players[i].score;
					displayScore(players, size);
					snakeExpand(players[i], 1);
					BigFruitClear(bigFood);
					bigFoodSpawned = false;
				}
				if (snakeKill(players[i], wall) || bodyHitPenality(players, size, i, snakeCount))
				{
					clearSnake(players[i]);
					deleteSnakeFromArrsy(players, i, size);
					snakeCount--;
					displayPorousWall(wall, row, col);
					sleepAdjust(size, sleeptime);
				}
				if (!headHitPenalty(players, players[i], size, i, snakeCount, sleeptime))
				{
					tailHitPenalty(players, players[i], size, i);
				}
				Sleep(sleeptime);

			}
			if (submode == RACE)
			{
				displayTime(startTime, raceTime, start);
			}
			else if (submode == MARATHON)
			{
				displayTarget(target);
			}
			if (!start)
			{
				_getch();
				time(&startTime);
				start = true;
			}
			
			end = gameEnd(mode, submode, players, size, target, winnerIndex, raceTime, startTime);
			if (snakeCount == 0)
			{
				end = true;
			}
			if (end)
			{
				for (int i = 0; i < size; i++)
				{
					clearSnake(players[i]);

				}
				if (submode == MARATHON)
				{
					gotoRowCol(51, 95);
					cout << "player" << winnerIndex << " win";
				}
				else if (submode == RACE)
				{
					int max = scoreList[0];
					int winner = 1;
					for (int i = 0; i < inSize; i++)
					{
						if (scoreList[i] > max)
						{
							max = scoreList[i];
							winner = i+1;
						}

					}
					bool found = false, tie = false;
					for (int i = 0; i < inSize; i++)
					{
						if (scoreList[i] == max)
						{
							if (found)
							{
								tie = true;
							}
							if (!found)
							{
								found = true;
							}
						}

					}
					if (tie)
					{
						gotoRowCol(51, 95);
						cout << "tie..";
					}
					else
					{
						gotoRowCol(51, 95);
						cout << "winner is player "<<winner<<"...";
					}
				}
				_getch();
				break;
			}
			

		} while (!end);
	}
	gotoRowCol(0, 0);
	_getch();
	return 0;
}
int main1()
{
	time_t start = 0;
	time(&start);
	time_t span = 0;
	bool spawned = false;
	int x = 10, y = 20;
	while (true)
	{
		time_t chk;
		time(&chk);
		if (!spawned && (chk - start) != 0 && (chk - start) % 20 == 0)
		{
			gotoRowCol(x, y);
			x += 10;
			y += 20;
			cout << "x";
			span = chk;
			spawned = true;
		}
		if (spawned && (chk - span) == 10)
		{
			spawned = false;
			system("cls");
		}
	}
}
int main2()
{
	brick wall{};
	int row = 70, col = 120;
	displayNonFunctionalBoundries(row, col);
	initPorousWall(wall, row, col);
	displayPorousWall(wall, row, col);
	int x;
	cin >> x;
	return 0;
}