#include "stdafx.h"

#include<string>
#include<iostream>
#include<string>
#include<iomanip>
using namespace std;

/*全局变量*/
extern FILE *fin;

const int norw = 18;		//保留字个数
const int tmax = 1000;		//标识符表容量
const int amax = 300;
const int bmax = 200;
const int smax = 6000;
const int c2max = 200;
const int csmax = 300;
const int xmax = 32767;
const int namx = 14;		//数字允许位数
const int al = 100;			//标识符最长长度
const int lmax = 70;		//最大嵌套层数
const int cmax = 2000;		//可容纳代码行数
const int lineleng = 5000;
const int linelimit = 2000;
const int stacksize = 14500; //运行栈容量
typedef char alfa[al];



extern char ch;				//最后读到的字符
extern string sym;			//读到的类型
extern string id;			//读到的字符串
extern int inum;			//读到的整数
extern double rnum;			//读到的实数
extern int cx;				//行号
extern int sx;				//字符串表下标
extern int sleng;			//字符串长度
extern int kk;				//清空格变量
extern int err;				//错误计数器
extern alfa tempchar;				//零时存放读到的字符


extern string symbol[32];
extern string ssym(char ch);
extern string word[18];
extern string wsym[18];

typedef struct
{
	string typ;
	int ref;
}item;

typedef struct
{
	int f;
	int x;
	int y;
}order;




/*词法分析器的声明*/
extern void clearToken();
extern void getToken();
extern void getch();
extern void getsym();

/*符号表*/
typedef struct
{
	string name;
	int link;
	string obj;
	string typ;
	int ref;
	bool normal;
	int lev;
	int adr;
}table;

typedef struct
{
	string inxtpy;
	string eltyp;
	int elref;
	int high;
	int elsize,size;
}atable;

typedef struct
{
	int last;
	int lastpar;
	int psize;
	int vsize;
}btable;

typedef struct
{
	string stab;
}stable;

typedef struct
{
	string tp;
	int i;
	double r;
}conrec;

typedef struct
{
	int i;
	double r;
	bool b;
	char c;
}stack;
