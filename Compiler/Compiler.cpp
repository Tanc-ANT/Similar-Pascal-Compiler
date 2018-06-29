// Compiler.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"define.h"

extern int lc, t, a, b, c2;
extern int level;
extern void enterids();
extern int display[lmax];
extern btable btab[bmax];
extern void Block(bool isfun);
extern void error(int n);
extern void emit(int fct);
extern void Generat();
extern void Interpret();

int _tmain(int argc, _TCHAR* argv[])
{
	system("chcp 936");
	char file[100];
	cout << "please input file:";
	cin >> file;
	fopen_s(&fin, file, "r+");
	cx = 1;
	lc = 0;
	err = 0;

	t = -1;
	a = 0;
	b = 1;
	sx = 0;
	c2 = 0;
	display[0] = 1;
	level = 1;
	enterids();

	btab[1].last = t;
	btab[1].lastpar = 1;
	btab[1].psize = 0;
	btab[1].vsize = 0;

	getsym();
	Block(false);
	if (sym != "period")
		error(60);
	emit(31);
	Generat();
	cout << "Compilation Complete" << endl;
	if (err == 0)
	{
		Interpret();
		cout << "\n" << "Interpreter Complete" << endl;
	}
	else
		cout << "\n" << "The program has errors" << endl;
	fclose(fin);

	/*
	cout << left << setw(4) << "cx" << setw(24) << "name" << setw(24) << "type" << endl;
	while (true)
	{
	getsym();
	getToken();
	}
	*/
	return 0;
}

