#include "stdafx.h"

#include<string>
#include<iostream>
#include<string>
#include<iomanip>
using namespace std;

/*ȫ�ֱ���*/
extern FILE *fin;

const int norw = 18;		//�����ָ���
const int tmax = 1000;		//��ʶ��������
const int amax = 300;
const int bmax = 200;
const int smax = 6000;
const int c2max = 200;
const int csmax = 300;
const int xmax = 32767;
const int namx = 14;		//��������λ��
const int al = 100;			//��ʶ�������
const int lmax = 70;		//���Ƕ�ײ���
const int cmax = 2000;		//�����ɴ�������
const int lineleng = 5000;
const int linelimit = 2000;
const int stacksize = 14500; //����ջ����
typedef char alfa[al];



extern char ch;				//���������ַ�
extern string sym;			//����������
extern string id;			//�������ַ���
extern int inum;			//����������
extern double rnum;			//������ʵ��
extern int cx;				//�к�
extern int sx;				//�ַ������±�
extern int sleng;			//�ַ�������
extern int kk;				//��ո����
extern int err;				//���������
extern alfa tempchar;				//��ʱ��Ŷ������ַ�


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




/*�ʷ�������������*/
extern void clearToken();
extern void getToken();
extern void getch();
extern void getsym();

/*���ű�*/
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
