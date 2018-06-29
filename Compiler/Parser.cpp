// Parser.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "define.h"
using namespace std;

extern void error(int n);
extern void fatal(int n);
extern void skip(int n);
extern void testsemicolon();
extern void Interpret();

int t, a, b, c1, c2, lc;
double rconst[c2max];
int display[lmax];
order code[cmax];
table tab[tmax];
atable atab[amax];
btable btab[bmax];

void Block(bool isfun);
void Const_Declaration();
void Constant(conrec &c);
void Var_Declaration();
void Type(string &tp, int &rf, int &x);
void ArrayType(int &aref, int &arse);
void Procedure_Declaration();
void Formal_Parameter_List();
void Statement();
void Assignment(int lv, int ad); 
void Expression(item &x);
void SimpleExpression(item &x);
void Term(item &x);
void Factor(item &x);
void Selector(item &v);
void Call(int i);
string ResultType(string a, string b);
void CompoundStatement();
void IfStatement();
void WhileStatement();
void ForStatement();
void Standproc(int n);


bool stantyps(string sy);
bool statbegsys(string sy);
void enter(string x0, string x1, string x2, int x3);
void enter(string id, string k);

//Block中的全局变量
int level, dx , start = 1;;

//Statement中的全局变量 
int i;

int ord(bool flag)
{
	if (flag)
		return 1;
	else
		return 0;
}

int ord(string s)
{
	if (s == "notyp")
		return 0;
	else if (s == "number")
		return 1;
	else if (s == "real")
		return 2;
	else if (s == "char")
		return 4;
}

bool stantyps(string sy)
{
	if (sy =="notyp"||sy == "number" || sy == "real" || sy == "char")
		return true;
	else
		return false;
}

bool statbegsys(string sy)
{
	if (sy == "beginsym" || sy == "ifsym" || sy == "whilesym" || sy == "forsym")
		return true;
	else
		return false;
}

bool facbegsys(string sy)
{
	if (sy =="number"||sym =="real"||sym =="char"||sy == "ident" || sy == "lparent"||sy =="notyp")
		return true;
	else
		return false;
}

void enter(string x0, string x1, string x2, int x3)
{
	t = t + 1;
	tab[t].name = x0;
	tab[t].link = t - 1;
	tab[t].obj = x1;
	tab[t].typ = x2;
	tab[t].normal = true;
	tab[t].lev = 0;
	tab[t].adr = x3;
}

void enter(string id, string k)
{
	int j, l;
	if (t == tmax)
		fatal(1);
	else
	{
		tab[0].name = id;
		j = btab[display[level]].last;
		l = j;
		while (tab[j].name != id)
			j = tab[j].link;
		if (j != 0)
			error(1);
		else
		{
			t = t + 1;
			tab[t].name = id;
			tab[t].link = l;
			tab[t].obj = k;
			tab[t].typ = "notyp";
			tab[t].ref = 0;
			tab[t].lev = level;
			tab[t].adr = 0;
			tab[t].normal = false;
		}
		btab[display[level]].last = t;
	}
}

void enterarray(int h)
{
	if (h<0 || h>xmax)
		error(27);
	if (a == amax)
		fatal(4);
	else
	{
		a = a + 1;
		atab[a].high = h;
	}
}

void enterblock()
{
	if (b == bmax)
		fatal(2);
	else
	{
		b = b + 1;
		btab[b].last = 0;
		btab[b].lastpar = 0;
	}
}

void enterreal(double x)
{
	if (c2 == c2max - 1)
		fatal(3);
	else
	{
		rconst[c2 + 1] = x;
		c1 = 1;
		while (rconst[c1]!=x)
		{
			c1 += 1;
		}
		if (c1 > c2)
			c2 = c1;
	}
}

void emit(int fct)
{
	if (lc == cmax)
		fatal(6);
	code[lc].f = fct;
	lc = lc + 1;
}

void emit1(int fct,int b)
{
	if (lc == cmax)
		fatal(6);
	code[lc].f = fct;
	code[lc].y = b;
	lc = lc + 1;
}

void emit2(int fct, int a, int b)
{
	if (lc == cmax)
		fatal(6);
	code[lc].f = fct;
	code[lc].x = a;
	code[lc].y = b;
	lc = lc + 1;
}

int loc(string id)
{
	int i, j;
	i = level;
	tab[0].name = id;
	do
	{
		j = btab[display[i]].last;
		while (tab[j].name != id)
		{
			j = tab[j].link;
		}
		i = i - 1;
	} while (i >= 0 && j == 0);
	if (j == 0)
		error(0);
	return j;
}

void entervariable()
{
	if (sym == "ident")
	{
		enter(id, "vvariable");
		getsym();
	}
	else
		error(2);
}

void Block(bool isfun)
{
	int tempdx, templev;
	tempdx = dx;
	templev = level;
	int prt, prb, x;
	dx = 5;
	prt = t;
	if (level > lmax)
		fatal(5);
	//test
	enterblock();
	prb = b;
	display[level] = b;
	tab[prt].typ = "notyp";
	tab[prt].ref = prb;
	if (sym == "lparent"&&level > 1)
	{
		Formal_Parameter_List();
	}
	btab[prb].lastpar = t;
	btab[prb].psize = dx;
	if (isfun)
	{
		if (sym == "colon")
		{
			getsym();
			if (sym == "ident")
			{
				x = loc(id);
				getsym();
				if (x != 0)
				{
					if (stantyps(tab[x].typ))
						tab[prt].typ = tab[x].typ;
					else
						error(15);
				}
				else
					skip(2);
			}
		}

		else
			error(5);
	}
	if (sym == "semicolon")
		getsym();
	else if (start == 1)
		start = start - 1;
	else
		error(14);
	do
	{
		if (sym == "constsym")
		{
			Const_Declaration();
		}
		if (sym == "varsym")
		{
			Var_Declaration();
		}
		btab[prb].vsize = dx;
		if (sym == "procssym" || sym == "functsym")
			Procedure_Declaration();
		//test
	} while (!statbegsys(sym));
	tab[prt].adr = lc;
	getsym();
	Statement();
	while (sym == "semicolon" || statbegsys(sym))
	{
		if (sym == "semicolon")
			getsym();
		else
			error(14);
		Statement();
	}
	if (sym == "endsym")
		getsym();
	else
		error(57);

	dx = tempdx;
	level = templev;
//	test
}

void Const_Declaration()
{
	conrec c;
	getsym();
	//test
	while(sym == "ident")
	{
		enter(id, "constant");
		getsym();
		if (sym == "eql")
			getsym();
		else
		{
			error(16);
			if (sym == "becomes")
				getsym();
		}
		Constant(c);
		tab[t].typ = c.tp;
		tab[t].ref = 0;
		if (c.tp == "real")
		{
			enterreal(c.r);
			tab[t].adr = c1;
		}
		else
			tab[t].adr = c.i;
		if (sym == "comma")
			getsym();
		else
		{
			testsemicolon();
			if (sym == "ident")
			{
				error(62);
				skip(62);
			}
			break;
		}
	}
}

void Constant(conrec &c)
{
	int x, sign;
	c.tp = "notyp";
	c.i = 0;
	//test
	if (sym == "charsym")
	{
		c.tp = "char";
		c.i = inum;
		getsym();
	}
	else
	{
		sign = 1;
		if (sym == "plus" || sym == "minus")
		{
			if (sym == "minus")
			{
				sign = -1;
			}
			getsym();
		}
		if (sym == "ident")
		{
			x = loc(id);
			if (x != 0)
			{
				if (tab[x].obj != "constant")
					error(25);
				else
				{
					c.tp = tab[x].typ;
					if (c.tp == "real")
						c.r = sign*rconst[tab[x].adr];
					else
						c.i = sign*tab[x].adr;
				}
			}
			getsym();
		}
		else if (sym == "number")
		{
			c.tp = "number";
			c.i = sign *inum;
			getsym();
		}
		else if (sym == "real")
		{
			c.tp = "real";
			c.r = sign * rnum;
			getsym();
		}
		else
			skip(50);
	}
		//test
}

void Var_Declaration()
{
	string tp;
	int t0, t1, rf=0, sz=0;
	getsym();
	while (sym == "ident")
	{
		t0 = t;
		entervariable();
		while (sym == "comma")
		{
			getsym();
			entervariable();
		}
		if (sym == "colon")
			getsym();
		else
			error(5);
		t1 = t;
		Type(tp,rf,sz);
		while (t0 < t1)
		{
			t0 = t0 + 1;
			tab[t0].typ = tp;
			tab[t0].ref = rf;
			tab[t0].lev = level;
			tab[t0].adr = dx;
			tab[t0].normal = true;
			dx = dx + sz;
		}
		if (sym != "semicolon")
		{
			error(63);
			getsym();
		}
		else
		{
			testsemicolon();
		}
	}

}

void Type(string &tp, int &rf, int &sz)
{
	string eltp;
	int elrf, x,t0,t1;
	tp = "notyp";
	rf = 0;
	sz = 0;
	//test
	if (sym == "ident")
	{
		x = loc(id);
		if (x != 0)
		{
			if (tab[x].obj != "typel")
				error(29);
			else
			{
				tp = tab[x].typ;
				rf = tab[x].ref;
				sz = tab[x].adr;
				if (tp == "notyp")
					error(30);
			}
		}
		getsym();
	}
	else if (sym == "arraysym")
	{
		getsym();
		if (sym == "lbrack")
			getsym();
		else
		{
			error(11);
			if (sym == "lparent")
				getsym();
		}
		tp = "array";
		ArrayType(rf, sz);
	}
}

void ArrayType(int &aref, int &arse)
{
	string eltp;
	conrec high;
	int elrf, elsz;
	Constant(high);
	if (high.tp == "real")
	{
		error(27);
		high.tp = "number";
		high.i = 0;
	}
	enterarray(high.i);
	atab[a].inxtpy = "number";
	aref = a;
	if (sym == "rbrack")
		getsym();
	else
		error(12);
	if (sym == "ofsym")
		getsym();
	else
		error(8);
	string tempstr = "notyp";
	Type(eltp, elrf, elsz);
	arse = (atab[aref].high)*elsz;
	atab[aref].size = high.i;
	atab[aref].eltyp = eltp;
	atab[aref].elref = elrf;
	atab[aref].elsize = elsz;
}

void Procedure_Declaration()
{
	int templev;
	bool isfun;
	isfun = (sym == "functsym");
	getsym();
	if (sym != "ident")
	{
		error(2);
		id = "";
	}
	if (isfun)
		enter(id, "function");
	else
		enter(id, "procedure");
	tab[t].normal = true;
	getsym();
	templev = level;
	level += 1;
	Block(isfun);
	level = templev;
	if (sym == "semicolon")
		getsym();
	else
		error(14);
	emit(32 + ord(isfun));
}

void Formal_Parameter_List()
{
	string tp;
	bool valpar;
	int rf, sz, x, t0;
	
	getsym();
	tp = "notyp";
	rf = 0;
	sz = 0;
	//test
	if (sym == "rparent")
		error(64);
	while (sym == "ident" || sym == "varsym")
	{
		if (sym != "varsym")
			valpar = true;
		else
		{
			getsym();
			valpar = false;
		}
		t0 = t;
		entervariable();
		while (sym == "comma")
		{
			getsym();
			entervariable();
		}
		if (sym == "colon")
		{
			getsym();
			if (sym != "ident")
				error(2);
			else
			{
				x = loc(id);
				getsym();
				if (x != 0)
				{
					if (tab[x].obj != "typel")
						error(29);
					else
					{
						tp = tab[x].typ;
						rf = tab[x].ref;
						if (valpar)
							sz = tab[x].adr;
						else
							sz = 1;
					}
				}
			}
			//test
		}
		else
			error(5);
		while (t0 < t)
		{
			t0 = t0 + 1;
			tab[t0].typ = tp;
			tab[t0].ref = rf;
			tab[t0].adr = dx;
			tab[t0].normal = valpar;
			dx = dx + sz;
		}
		if (sym != "rparent")
		{
			if (sym == "semicolon")
				getsym();
			else
			{
				error(14);
				if (sym == "comma")
					getsym();
			}
			//test
		}
	}//{while}
	if (sym == "rparent")
	{
		getsym();
		//test
	}
	else
		error(4);
}

void Statement()
{
	if (sym == "ident" || statbegsys(sym))
	{
		if (sym == "ident")
		{
			i = loc(id);
			getsym();
			if (i != 0)
			{
				if (tab[i].obj == "constant" || tab[i].obj == "typel")
					error(45);
				else if (tab[i].obj == "vvariable")
					Assignment(tab[i].lev, tab[i].adr);
				else if (tab[i].obj == "procedure")
				{
					if (tab[i].lev != 0)
						Call(i);
					else
						Standproc(tab[i].adr);
				}
				else if (tab[i].obj == "function")
				{
					if (tab[i].ref == display[level])
						Assignment(tab[i].lev + 1,0);
					else
						error(45);
				}

			}
		}
		else if (sym == "beginsym")
			CompoundStatement();
		else if (sym == "ifsym")
			IfStatement();
		else if (sym == "whilesym")
			WhileStatement();
		else if (sym == "forsym")
			ForStatement();
	}
	//test
}

void Assignment(int lv, int ad)
{
	item x, y;
	int f;
	x.typ = tab[i].typ;
	x.ref = tab[i].ref;
	if (tab[i].normal)
		f = 0;
	else
		f = 1;
	emit2(f, lv, ad);
	if (sym == "lbrack" || sym == "lparent")
		Selector(x);
	if (sym == "becomes")
		getsym();
	else
	{
		error(51);
		if (sym == "eql")
			getsym();
	}
	Expression(y);
	if (x.typ == y.typ)
	{
		if (stantyps(x.typ))
			emit(38);
		else
		{
			if (x.ref != y.ref)
				error(46);
			else
			{
				if (x.typ == "array")
					emit1(23, atab[x.ref].size);
				else
					emit1(23, btab[x.ref].vsize);
			}
		}
	}
	else if (x.typ == "number"&&y.typ == "char")
	{
		if (stantyps(x.typ))
			emit(38);
	}
	else if (x.typ == "real"&&(y.typ == "number" || y.typ == "char"))
	{
		emit1(26, 0);
		emit(38);
	}
	else if (x.typ != "notyp"&&y.typ != "notyp")
		error(46);
}

void Expression(item &x)
{
	item y;
	string op;
	SimpleExpression(x);
	if (sym == "eql" || sym == "neq" || sym == "lss" || sym == "leq" || sym == "gtr" || sym == "geq")
	{
		op = sym;
		getsym();
		SimpleExpression(y);
		if ((x.typ == "notyp" || x.typ == "number") && (x.typ == y.typ))
		{
			if (op == "eql")
				emit(45);
			else if (op == "neq")
				emit(46);
			else if (op == "lss")
				emit(47);
			else if (op == "leq")
				emit(48);
			else if (op == "gtr")
				emit(49);
			else if (op == "geq")
				emit(50);
		}
		else if ((x.typ == "char") && (x.typ == y.typ))
		{
			if (op == "eql")
				emit(64);
			else if (op == "neq")
				emit(65);
			else if (op == "lss")
				emit(66);
			else if (op == "leq")
				emit(67);
			else if (op == "gtr")
				emit(68);
			else if (op == "geq")
				emit(69);
		}
		else
		{
			if (x.typ == "number" || x.typ == "char")
			{
				x.typ == "real";
				emit1(26, 1);
			}
			else if (y.typ == "number" || y.typ == "char")
			{
				y.typ == "real";
				emit1(26, 0);
			}
			if (x.typ == "real"&&y.typ == "real")
			{
				if (op == "eql")
					emit(39);
				else if (op == "neq")
					emit(40);
				else if (op == "lss")
					emit(41);
				else if (op == "leq")
					emit(42);
				else if (op == "gtr")
					emit(43);
				else if (op == "geq")
					emit(44);
			}
			else
				error(35);/////////////////////////////////////////////////////////////////////////////////////////////////
		}
		x.typ = "bool";
	}
	else if (sym == "charsym")
		error(61);
}

void SimpleExpression(item &x)
{
	item y;
	string op;

	if (sym == "plus" || sym == "minus")
	{
		op = sym;
		getsym();
		Term(x);
		if (x.typ == "array")
			error(33);
		else if (op == "minus")
			emit(36);
	}
	else
		Term(x);
	while (sym == "plus" || sym == "minus")
	{
		op = sym;
		getsym();
		Term(y);
		x.typ = ResultType(x.typ, y.typ);
		if (x.typ == "notyp");
		
		else if (x.typ == "number"|| x.typ == "char")
		{
			if (op == "plus")
				emit(52);
			else
				emit(53);//code30
		}
		else if (x.typ == "real")
		{
			if (op == "plus")
				emit(54);
			else
				emit(55);
		}
		
	}//{while}
}

void Term(item &x)
{
	item y;
	string op;
	Factor(x);
	while (sym == "times" || sym == "ridv")
	{
		op = sym;
		getsym();
		Factor(y);
		if (op == "times")
		{
			x.typ = ResultType(x.typ, y.typ);
			if (x.typ == "notyp");

			else if (x.typ == "number" || x.typ == "char")
				emit(57);
			else if (x.typ == "real")
				emit(60);
		}
		else if (op == "ridv" && (x.typ == "number" || x.typ == "char") && (y.typ == "number" || y.typ == "char"))
		{
			if (op == "ridv")
				emit(58);
			else
			{
				if (x.typ != "notyp"&&y.typ != "notyp")
					error(34);
				x.typ = "notyp";
			}
		}
		else if (op == "ridv")
		{
			if (x.typ == "number" || x.typ == "char")
			{
				emit1(26, 1);
				x.typ = "real";
				
			}
			if (y.typ == "number"||y.typ == "char")
			{
				emit1(26, 0);
				y.typ = "real";
			}
			if (x.typ == "real"&&y.typ == "real")
			{
				emit(61);
			}
			else
			{
				if (x.typ != "notyp"&&y.typ != "notyp")
					error(33);
				x.typ = "notyp";
			}
		}
	}//while
}

void Factor(item &x)
{
	int i, f;
	x.typ = "notyp";
	x.ref = 0;
	//test
	while (facbegsys(sym))
	{
		if (sym == "ident")
		{
			i = loc(id);
			getsym();
			if (tab[i].obj == "constant")
			{
				x.typ = tab[i].typ;
				x.ref = 0;
				if (x.typ == "real")
					emit1(25, tab[i].adr);
				else if (x.typ == "char")
				{
					emit1(70, tab[i].adr);
				}
				else
					emit1(24, tab[i].adr);
			}
			else if (tab[i].obj == "vvariable")
			{
				x.typ = tab[i].typ;
				x.ref = tab[i].ref;
				if (sym == "lbrack" || sym == "lparent" )
				{
					if (tab[i].normal)
						f = 0;
					else
						f = 1;
					emit2(f, tab[i].lev, tab[i].adr);
					Selector(x);
					if (stantyps(x.typ))
						emit(34);
				}
				else
				{
					if (stantyps(x.typ))
					{
						if (tab[i].normal)
							f = 1;
						else
							f = 2;
					}
					else
					{
						if (tab[i].normal)
							f = 0;
						else
							f = 1;
					}
					emit2(f, tab[i].lev, tab[i].adr);
				}
			}
			else if (tab[i].obj == "typel" || tab[i].obj == "procedure")
				error(44);
			else if (tab[i].obj == "function")
			{
				x.typ = tab[i].typ;
				if (tab[i].lev != 0)
					Call(i);
				else
					//Standfct(tab[i].adr,x);
					error(0);
			}
		}
		else if (sym == "char" || sym == "number" || sym == "real")
		{
			if (sym == "real")
			{
				x.typ = "real";
				enterreal(rnum);
				emit1(25, c1);
			}
			else
			{
				if (sym == "char")
				{
					x.typ = "char";
					emit1(70, inum);
				}
				else
				{
					x.typ = "number";
					emit1(24, inum);
				}
			}
			x.ref = 0;
			getsym();
		}
		else
		{

			if (sym == "lparent")
			{
				getsym();
				Expression(x);
				if (sym == "rparent")
					getsym();
				else
					error(4);
			}
			else
				error(9);
		}
		//test
	}//while
}

void Selector(item &v)
{
	item x;
	int a, j;
	if (sym != "lbrack")
		error(11);
	getsym();
	Expression(x);
	if (v.typ != "array")
		error(28);
	else
	{
		a = v.ref;
		if (atab[a].inxtpy != x.typ)
			error(26);
		else
		{
			if (atab[a].elsize == 1)
				emit1(20, a);
			else
				emit1(21, a);
			v.typ = atab[a].eltyp;
			v.ref = atab[a].elref;
		}

	}
	if (sym == "rbrack")
		getsym();
	else
	{
		error(12);
		if (sym == "rparent")
			getsym();
	}
	//test
}

void Call(int i)
{
	item x;
	int lastp, cp, k;

	emit1(18, i);
	lastp = btab[tab[i].ref].lastpar;
	cp = i;
	if (sym == "lparent")
	{
		do
		{
			getsym();
			if (cp >= lastp)
				error(39);
			else
			{
				cp = cp + 1;
				if (tab[cp].normal)
				{
					Expression(x);
					if (x.typ == tab[cp].typ)
					{
						if (x.ref != tab[cp].ref)
							error(36);
						else
						{
							if (x.typ == "array")
								emit1(22, btab[x.ref].vsize);
						}
					}
					else
					{
						if ((x.typ == "number" || x.typ == "char")&&tab[cp].typ == "real")
						{
							emit1(26, 0);

						}
						else if (x.typ != "notyp")
							error(36);
					}
				}
				else
				{
					if (sym != "ident")
						error(2);
					else
					{
						k = loc(id);
						getsym();
						if (k != 0)
						{
							if (tab[k].obj != "vvariable")
								error(37);
							x.typ = tab[k].typ;
							x.ref = tab[k].ref;
							if (tab[k].normal)
								emit2(0, tab[k].lev, tab[k].adr);//code45
							else
								emit2(1, tab[k].lev, tab[k].adr);
							if (sym == "lbrack" || sym == "lparent")
							{
								Selector(x);
							}
							if (x.typ != tab[cp].typ || x.ref != tab[cp].ref)
								error(36);
						}
					}
				}
			}
			//test
		} while (sym == "comma");
		if (sym == "rparent")
			getsym();
		else
			error(4);
	}
	if (cp < lastp)
		error(39);
	emit1(19, btab[tab[i].ref].psize - 1);
	if (tab[i].lev < level)
		emit2(3, tab[i].lev, level);
}

string ResultType(string a, string b)
{
	if (a == "array" || b == "array")
	{
		error(33);
		return "notyp";
	}
//	else if (a == "char" || b == "char")
//		return "char";
	else
	{
		if (a == "notyp" || b == "notyp")
			return "notyp";
		else
		{
			if (a == "number")
			{
				if (b == "number"||b == "char")
					return "number";
				else
				{
					emit1(26, 1);
					return "real";
				}
			}
			else if (a == "char")
			{
				if (b == "number" || b == "char")
					return "char";
				else
				{
					emit1(26, 1);
					return "real";
				}
			}
			else
			{
				if (b == "number"|| b == "char")
					emit1(26, 0);
				return "real";
			}
		}
	}
}

void CompoundStatement()
{
	getsym();
	Statement();
	while (statbegsys(sym)||sym =="semicolon")
	{
		if (sym == "semicolon")
			getsym();
		else
			error(14);
		Statement();
	}
	if (sym == "endsym")
		getsym();
	else
		error(57);
}

void IfStatement()
{
	item x;
	int lc1, lc2;

	getsym();
	Expression(x);
	if (x.typ != "notyp"&&x.typ !="bool")
		error(17);
	lc1 = lc;
	emit(11);
	if (sym == "thensym")
		getsym();
	else
	{
		error(52);
		if (sym == "dosym")
			getsym();
	}
	Statement();
	if (sym == "elsesym")
	{
		getsym();
		lc2 = lc;
		emit(10);
		code[lc1].y = lc;
		Statement();
		code[lc2].y = lc;
	}
	else
		code[lc1].y = lc;
}

void WhileStatement()
{
	item x;
	int lc1, lc2;

	getsym();
	lc1 = lc;
	Expression(x);
	if (x.typ != "notyp"&&x.typ!="bool")
		error(17);
	lc2 = lc;
	emit(11);
	if (sym == "dosym")
		getsym();
	else
		error(54);
	Statement();
	emit1(10, lc1);
	code[lc2].y = lc;
}

void ForStatement()
{
	string cvt;
	item x;
	int i, f, lc1, lc2;

	getsym();
	if (sym == "ident")
	{

		i = loc(id);
		getsym();
		if (i == 0)
			cvt == "number";
		else
		{
			if (tab[i].obj == "vvariable")
			{
				cvt = tab[i].typ;
				if (!tab[i].normal)
					error(37);
				else
					emit2(0, tab[i].lev, tab[i].adr);
				if (cvt != "notyp" && cvt != "number" && cvt != "char")
					error(18);
			}
			else
			{
				error(37);
				cvt = "number";
			}
		}
	}
	else
		skip(2);
	if (sym == "becomes")
	{
		getsym();
		Expression(x);
		if (x.typ != cvt)
			error(19);
	}
	else
		skip(51);
	f = 14;
	if (sym == "tosym" || sym == "downtosym")
	{
		if (sym == "downtosym")
			f = 16;
		getsym();
		Expression(x);
		if (x.typ != cvt)
			error(19);
	}
	else
		skip(55);
	lc1 = lc;
	emit(f);
	if (sym == "dosym")
		getsym();
	else
		error(54);
	lc2 = lc;
	Statement();
	emit1(f + 1, lc2);
	code[lc1].y = lc;
}

void Standproc(int n)
{
	int i, f;
	item  x, y;

	if (n == 1)
	{
		if (sym == "lparent")
		{
			do
			{
				getsym();
				if (sym != "ident")
					error(2);
				else
				{
					i = loc(id);
					getsym();
					if (i != 0)
					{
						if (tab[i].obj != "vvariable")
							error(37);
						else
						{
							x.typ = tab[i].typ;
							x.ref = tab[i].ref;
							if (tab[i].normal)
								f = 0;
							else
								f = 1;
							emit2(f, tab[i].lev, tab[i].adr);
							if (sym == "lbrack" || sym == "lparent")
								Selector(x);
							if (x.typ == "number" || x.typ == "real" || x.typ == "char" || x.typ == "notyp")
								emit1(27, ord(x.typ));
							else
								error(41);
						}
					}
				}
				//test
			} while (sym == "comma");
			if (sym == "rparent")
				getsym();
			else
				error(4);
		}
	}
	else if (n == 3)
	{
		if (sym == "lparent")
		{
			getsym();
			if (sym == "str")
			{
				emit1(28, inum);
				getsym();
				if (sym == "comma")
				{
					getsym();
					Expression(x);
					if (!stantyps(x.typ))
						error(41);
					emit1(29, ord(x.typ));
				}
			}
			else
			{
				Expression(x);
				if (!stantyps(x.typ))
				{
					error(41);
				}
				emit1(29,ord(x.typ));
			}
		}
		if (sym == "rparent")
			getsym();
		else
			error(4);
	}
}

void enterids()
{
	enter("", "vvariable", "notyp", 0);
	enter("real", "typel", "real", 1);
	enter("char", "typel", "char", 1);
	enter("integer", "typel", "number", 1);
	enter("read", "procedure", "notyp", 1);
	enter("write", "procedure", "notyp", 3);
	enter("", "procedure", "notyp", 0);
}

void Generat()
{
	int temp = 0;
	cout <<"lc\t"<< "f\t" << "x\t" << "y\t" << endl;
	do
	{
		cout <<temp<<"\t"<< code[temp].f << "\t" << code[temp].x << "\t"<<code[temp].y << endl;
		temp += 1;
	} while (temp != lc);
}

