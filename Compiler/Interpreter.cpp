#include"stdafx.h"
#include"define.h"
using namespace std;

extern order code[cmax];
extern table tab[tmax];
extern atable atab[amax];
extern btable btab[bmax];
extern stable stab[smax];
extern double rconst[c2max];

order ir;
int pc;
int tt;
int bb;
int h1, h2, h3;
int lncnt, ocnt, blkcnt, chrcnt;
string ps;
int fld[4];
int display1[lmax];

stack s[stacksize];

void inter0()
{
	if (ir.f == 0)
	{
		tt = tt + 1;
		if (tt > stacksize)
			ps = "stkchk";
		else
			s[tt].i = display1[ir.x] + ir.y;
	}
	else if (ir.f == 1)
	{
		tt = tt + 1;
		if (tt > stacksize)
			ps = "stkchk";
		else
			s[tt] = s[display1[ir.x] + ir.y];
	}
	else if (ir.f == 2)
	{
		tt = tt + 1;
		if (tt > stacksize)
			ps = "stkchk";
		else
			s[tt] = s[s[display1[ir.x] + ir.y].i];
	}
	else if (ir.f == 3)
	{
		h1 = ir.y;
		h2 = ir.x;
		h3 = bb;
		do
		{
			display1[h1] = h3;
			h1 = h1 - 1;
			h3 = s[h3 + 2].i;
		} while (h1 != h2);

	}
	else if (ir.f == 8)
	{
		cout << "this is unexcept : 8" << endl;
	}
	else if (ir.f == 9)
	{
		s[tt].i = s[tt].i + ir.y;
	}
}

void inter1()
{
	int  h3, h4;
	if (ir.f == 10)
	{
		pc = ir.y;
	}
	else if (ir.f == 11)
	{
		if (!s[tt].b)
			pc = ir.y;
		tt = tt - 1;
	}
	else if (ir.f == 12)
	{
		h1 = s[tt].i;
		tt = tt - 1;
		h2 = ir.y;
		h3 = 0;
		do
		{
			if (code[h2].f != 13)
			{
				h3 = 1;
				ps = "caschk";
			}
			else
			{
				if (code[h2].y == h1)
				{
					h3 = 1;
					pc = code[h2 + 1].y;
				}
				else
					h2 = h2 + 2;
			}
		} while (h3 == 0);
	}
	else if (ir.f == 14)
	{
		h1 = s[tt - 1].i;
		if (h1 <= s[tt].i)
		{
			s[s[tt - 2].i].i = h1;
		}
		else
		{
			tt = tt - 3;
			pc = ir.y;
		}
	}
	else if (ir.f == 15)
	{
		h2 = s[tt - 2].i;
		h1 = s[h2].i + 1;
		if (h1 <= s[tt].i)
		{
			s[h2].i = h1;
			pc = ir.y;
		}
		else
			tt = tt - 3;
	}
	else if (ir.f == 16)
	{
		h1 = s[tt - 1].i;
		if (h1 >= s[tt].i)
			s[s[tt - 2].i].i = h1;
		else
		{
			pc = ir.y;
			tt = tt - 3;
		}
	}
	else if (ir.f == 17)
	{
		h2 = s[tt - 2].i;
		h1 = s[h2].i - 1;
		if (h1 >= s[tt].i)
		{
			s[h2].i = h1;
			pc = ir.y;
		}
		else
			tt = tt - 3;
	}
	else if (ir.f == 18)
	{
		h1 = btab[tab[ir.y].ref].vsize;
		if ((tt + h1) > stacksize)
			ps = "stkchk";
		else
		{
			tt = tt + 5;
			s[tt - 1].i = h1 - 1;
			s[tt].i = ir.y;
		}
	}
	else if (ir.f == 19)
	{
		h1 = tt - ir.y;
		h2 = s[h1 + 4].i;
		h3 = tab[h2].lev;
		display1[h3 + 1] = h1;
		h4 = s[h1 + 3].i + h1;
		s[h1 + 1].i = pc;
		s[h1 + 2].i = display1[h3];
		s[h1 + 3].i = bb;
		for (h3 = tt + 1; h3 <= h4; h3++)
			s[h3].i = 0;
		bb = h1;
		tt = h4;
		pc = tab[h2].adr;
	}
}

void inter2()
{
	if (ir.f == 20)
	{
		h1 = ir.y;
		h2 = 0;
		h3 = s[tt].i;
		if (h3 < h2)
			ps = "inxchk";
		else
		{
			if (h3>atab[h1].high)
				ps = "inxchk";
			else
			{
				tt = tt - 1;
				s[tt].i = s[tt].i + (h3 - h2);
			}
		}
	}
	else if (ir.f == 21)
	{
		h1 = ir.y;
		h2 = 0;
		h3 = s[tt].i;
		if (h3 < h2)
			ps = "inxchk";
		else
		{
			if (h3>atab[h1].high)
				ps = "inxchk";
			else
			{
				tt = tt - 1;
				s[tt].i = s[tt].i + (h3 - h2)*atab[h1].elsize;
			}
		}
	}
	else if (ir.f == 22)
	{
		h1 = s[tt].i;
		tt = tt - 1;
		h2 = ir.y + tt;
		if (h2 > stacksize)
			ps = "stkchk";
		else
		{
			while (tt < h2)
			{
				tt = tt + 1;
				s[tt] = s[h1];
				h1 = h1 + 1;
			}
		}
	}
	else if (ir.f == 23)
	{
		h1 = s[tt - 1].i;
		h2 = s[tt].i;
		h3 = h1 + ir.y;
		while (h1 < h3)
		{
			s[h1] = s[h2];
			h1 = h1 + 1;
			h2 = h2 + 1;
		}
		tt = tt - 2;
	}
	else if (ir.f == 24)
	{
		tt = tt + 1;
		if (tt > stacksize)
			ps = "stkchk";
		else
			s[tt].i = ir.y;
	}
	else if (ir.f == 25)
	{
		tt = tt + 1;
		if (tt > stacksize)
			ps = "stkchk";
		else
			s[tt].r = rconst[ir.y];
	}
	else if(ir.f == 26)
	{
		h1 = tt - ir.y;
		s[h1].r = s[h1].i;
	}
	else if (ir.f == 27)
	{
		if (ir.y == 1)
			cin >> s[s[tt].i].i;
		else if (ir.y == 2)
			cin >> s[s[tt].i].r;
		else if (ir.y == 4)
		{
			char ch;
			cin >> ch;
			s[s[tt].i].c = ch;
			s[s[tt].i].i = ch;
		}
		tt = tt - 1;
	}
	else if (ir.f == 28)
	{
		h2 = ir.y;
		chrcnt = chrcnt + stab[h2].stab.length();
		if (chrcnt > lineleng)
			ps = "lngchk";
		cout << stab[h2].stab;
		h2 = stab[h2].stab.length();

	}
	else if (ir.f == 29)
	{
		chrcnt = chrcnt + fld[ir.y];
		if (chrcnt > lineleng)
			ps = "lngchk";
		else
		{
			if (ir.y == 1)
				cout << s[tt].i;
			else if (ir.y == 2)
				cout << s[tt].r;
			else if (ir.y == 3)
			{
				if (s[tt].b)
					cout << "true";
				else
					cout << "false";
			}
			else if (ir.y == 4)
			{
				cout << (char)s[tt].i;
			}	
		}
		tt = tt - 1;
	}
}

void inter3()
{
	if (ir.f == 30)
	{
		chrcnt = chrcnt + s[tt].i;
		if (chrcnt > lineleng)
			ps = "lngchk";
		else
		{
			if (ir.y == 1)
				cout << s[tt - 1].i;
			else if (ir.y == 2)
				cout << s[tt - 1].r;
			else if (ir.y ==3)
				cout << "this is unexcept : 30" << endl;
		}
		tt = tt - 2;
	}
	else if (ir.f == 31)
	{
		ps = "fin";
	}
	else if (ir.f == 32)
	{
		tt = bb - 1;
		pc = s[bb + 1].i;
		bb = s[bb + 3].i;
	}
	else if (ir.f == 33)
	{
		tt = bb;
		pc = s[bb + 1].i;
		bb = s[bb + 3].i;
	}
	else if (ir.f == 34)
	{
		s[tt] = s[s[tt].i];
	}
	else if (ir.f == 35)
	{
		s[tt].b = !s[tt].b;
	}
	else if (ir.f == 36)
	{
		s[tt].i = -s[tt].i;
	}
	else if (ir.f == 37)
	{
		chrcnt = chrcnt + s[tt - 1].i;
		if (chrcnt > lineleng)
			ps = "lngchk";
		else
			cout << s[tt - 2].r;
		tt = tt - 3;
	}
	else if (ir.f == 38)
	{
		s[s[tt - 1].i] = s[tt];
		tt = tt - 2;
	}
	else if (ir.f == 39)
	{
		tt = tt - 1;
		s[tt].b = (s[tt].r == s[tt + 1].r);
	}
}

void inter4()
{
	if (ir.f == 40)
	{
		tt = tt - 1;
		s[tt].b = (s[tt].r != s[tt + 1].r);
	}
	else if (ir.f == 41)
	{
		tt = tt - 1;
		s[tt].b = (s[tt].r < s[tt + 1].r);
	}
	else if (ir.f == 42)
	{
		tt = tt - 1;
		s[tt].b = (s[tt].r<=s[tt + 1].r);
	}
	else if (ir.f == 43)
	{
		tt = tt - 1;
		s[tt].b = (s[tt].r>s[tt + 1].r);
	}
	else if (ir.f == 44)
	{
		tt = tt - 1;
		s[tt].b = (s[tt].r>=s[tt + 1].r);
	}
	else if (ir.f == 45)
	{
		tt = tt - 1;
		s[tt].b = (s[tt].i==s[tt + 1].i);
	}
	else if (ir.f == 46)
	{
		tt = tt - 1;
		s[tt].b = (s[tt].i != s[tt + 1].i);
	}
	else if (ir.f == 47)
	{
		tt = tt - 1;
		s[tt].b = (s[tt].i < s[tt + 1].i);
	}
	else if (ir.f == 48)
	{
		tt = tt - 1;
		s[tt].b = (s[tt].i <= s[tt + 1].i);
	}
	else if (ir.f == 49)
	{
		tt = tt - 1;
		s[tt].b = s[tt].i > s[tt + 1].i;
	}
}

void inter5()
{
	if (ir.f == 50)
	{
		tt = tt - 1;
		s[tt].b = s[tt].i >= s[tt + 1].i;
	}
	else if (ir.f == 51)
	{
		tt = tt - 1;
		s[tt].b = s[tt].b || s[tt + 1].b;
	}
	else if (ir.f == 52)
	{
		tt = tt - 1;
		s[tt].i = s[tt].i + s[tt + 1].i;
	}
	else if (ir.f == 53)
	{
		tt = tt - 1;
		s[tt].i = s[tt].i - s[tt + 1].i;
	}
	else if (ir.f == 54)
	{
		tt = tt - 1;
		s[tt].r = s[tt].r + s[tt + 1].r;
	}
	else if (ir.f == 55)
	{
		tt = tt - 1;
		s[tt].r = s[tt].r - s[tt + 1].r;
	}
	else if (ir.f == 56)
	{
		tt = tt - 1;
		s[tt].b = s[tt].b && s[tt + 1].b;
	}
	else if (ir.f == 57)
	{
		tt = tt - 1;
		s[tt].i = s[tt].i * s[tt + 1].i;
	}
	else if (ir.f == 58)
	{
		tt = tt - 1;
		if (s[tt + 1].i == 0)
			ps = "divchk";
		else
			s[tt].i = s[tt].i / s[tt + 1].i;
	}
	else if (ir.f == 59)
	{
		tt = tt - 1;
		if (s[tt + 1].r == 0)
			ps = "divchk";
		else
			s[tt].i = s[tt].i-(s[tt].i / s[tt + 1].i)*s[tt].i;
	}
}

void inter6()
{
	if (ir.f == 60)
	{
		tt = tt - 1;
		s[tt].r = s[tt].r*s[tt+1].r;
	}
	else if (ir.f == 61)
	{
		tt = tt - 1;
		s[tt].r = s[tt].r/s[tt+1].r;
	}
	else if (ir.f == 62)
	{
		cout << "this is unexcept : 62" << endl;
	}
	else if (ir.f == 63)
	{
		cout << endl;
		lncnt = lncnt + 1;
		chrcnt = 0;
		if (lncnt > linelimit)
			ps = "linchk";
	}
	else if (ir.f == 64)
	{
		tt = tt - 1;
		s[tt].b = (s[tt].c == s[tt + 1].c);
	}
	if (ir.f == 65)
	{
		tt = tt - 1;
		s[tt].b = (s[tt].c != s[tt + 1].c);
	}
	else if (ir.f == 66)
	{
		tt = tt - 1;
		s[tt].b = (s[tt].c < s[tt + 1].c);
	}
	else if (ir.f == 67)
	{
		tt = tt - 1;
		s[tt].b = (s[tt].c <= s[tt + 1].c);
	}
	else if (ir.f == 68)
	{
		tt = tt - 1;
		s[tt].b = (s[tt].c>s[tt + 1].c);
	}
	else if (ir.f == 69)
	{
		tt = tt - 1;
		s[tt].b = (s[tt].c >= s[tt + 1].c);
	}
}

void inter7()
{
	if (ir.f == 70)
	{
		tt = tt + 1;
		if (tt > stacksize)
			ps = "stkchk";
		else
		{
			s[tt].i = ir.y;
			s[tt].c = (char)ir.y;
		}
	}
}

void Interpret()
{
	s[1].i = 0;
	s[2].i = 0;
	s[3].i = -1;
	s[4].i = btab[1].last;
	display1[0] = 0;
	display1[1] = 0;
	tt = btab[2].vsize - 1;
	bb = 0;
	pc = tab[s[4].i].adr;
	lncnt = 0;
	ocnt = 0;
	chrcnt = 0;
	ps = "run";
	fld[1] = 10;
	fld[2] = 22;
	fld[3] = 10;
	fld[4] = 1;
	do
	{
		ir = code[pc];
		pc = pc + 1;
		ocnt = ocnt + 1;
		switch (ir.f / 10)
		{
		case 0:inter0();
		case 1:inter1();
		case 2:inter2();
		case 3:inter3();
		case 4:inter4();
		case 5:inter5();
		case 6:inter6();
		case 7:inter7();
		}
	} while (ps == "run");
	if (ps != "fin")
	{
		cout << "halt at " << pc << " because of " << ps;
		getchar();
	}
}