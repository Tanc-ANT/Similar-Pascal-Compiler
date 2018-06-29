#include "stdafx.h"
#include "define.h"
using namespace std;

char ch;
string sym;
string id;
int inum;
double rnum;
int cx;
int sx;
int sleng;
int kk;
alfa tempchar;
stable stab[smax];

FILE *fin;

extern void error(int n);

string symbol[32] =
{
	"nul", "ident", "number", "plus", "minus", "times", "ridv", "ofsym", "eql", "neq", "less",
	"leq", "gtr", "geq", "lapren", "rparen", "comma", "semicolon", "period", "becomes",
	"beginsym", "endsym", "ifsym", "thensym", "whillesym", "dosym", "callsym", "constsym",
	"varsym", "procsym", "readsym", "writesym"
};

string word[18] =
{
	"",
	"array",
	"begin",
	"const",
	"do",
	"downto",
	"else",
	"end",
	"for",
	"function",
	"if",
	"of",
	"procedure",
	"then",
	"to",
	"var",
	"while",
	" "
};

string wsym[18] =
{
	" ",
	"arraysym",
	"beginsym",
	"constsym",
	"dosym",
	"downtosym",
	"elsesym",
	"endsym",
	"forsym",
	"functsym",
	"ifsym",
	"ofsym",
	"procssym",
	"thensym",
	"tosym",
	"varsym",
	"whilesym",
	" "
};

string ssym(char ch)
{
	if (ch == '+')
		return "plus";
	else if (ch == '-')
		return "minus";
	else if (ch == '*')
		return "times";
	else if (ch == '/')
		return "ridv";
	else if (ch == '(')
		return "lparent";
	else if (ch == ')')
		return "rparent";
	else if (ch == '=')
		return "eql";
	else if (ch == ',')
		return "comma";
	else if (ch == '[')
		return "lbrack";
	else if (ch == ']')
		return "rbrack";
	else if (ch == '.')
		return "period";
	else if (ch == '<')
		return "lss";
	else if (ch == '>')
		return "gtr";
	else if (ch == ';')
		return "semicolon";
	else
		return "error";
}


void clearToken()
{
	int i = 0;
	ch = '\0';
	for (i = 0; i < al; i++)
	{
		tempchar[i] = '\0';
	}
	id = "";
}

void getToken()
{
	cout << left << setw(4) << cx << setw(24) << id << setw(24) << sym << endl;
}


void getch()
{
	ch = fgetc(fin);
	if (ch == EOF&&cx == 0)
	{
		cout << "program inconplete" << endl;
		exit(0);
	}
	else if (ch == EOF&&cx != 0)
	{
		cout << "program complete" << endl;
		exit(0);
	}
}

void getsym()
{
	clearToken();
	int i,j,k;
	getch();
	while (ch == ' ' || ch == '\t' || ch == '\n')
	{
		if (ch == '\n')
			cx += 1;
		getch();
	}
	if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
	{
		int k = 0;
		do{
			if (k < al)
			{
				tempchar[k] = ch;
				k = k + 1;
			}
			getch();
		} while ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')|| (ch>='0'&&ch<='9'));
		fseek(fin, -1, SEEK_CUR);
		if (k >= kk)
			kk = k;
		else
		{
			do{
				tempchar[kk] = ' ';
				kk = kk - 1;
			} while (kk != k);
		}
		id= tempchar;
		i = 1;
		j = norw;
		do{
			k = (i + j) / 2;
			if (id <= word[k])
				j = k - 1;
			if (id >= word[k])
				i = k + 1;
		} while (i <= j);
		if (i - 1 > j)
			sym = wsym[k];
		else
			sym = "ident";
	}
	else if (ch >= 48 && ch <= 57)
	{
		k = 0;
		inum = 0;
		rnum = 0.0;
		sym = "number";
		do{
			id = id + ch;
			k = k + 1;
			getch();
			if (ch == '.')
			{
				sym = "real";
				id = id + ch;
				getch();
			}
		} while (ch >= 48 && ch <= 57);
		fseek(fin, -1, SEEK_CUR);
		char *str = const_cast<char *>(id.c_str());
		int len = strlen(str);
		char *b;
		b = (str + len - 1);
		if (*b == '.')
			error(13);					//判断实数是否合法
		if (sym == "number")
			sscanf_s(str, "%d", &inum);
		else
			sscanf_s(str, "%lf", &rnum);
	}
	else if (ch == ':')
	{
		getch();
		if (ch == '=')
		{
			sym = "becomes";
			id = ":=";
		}
		else
		{
			id = ":";
			sym = "colon";
			fseek(fin, -1, SEEK_CUR);
		}
	}
	else if (ch == '<')
	{
		getch();
		if (ch == '=')
		{
			sym = "leq";
			id = "<=";
		}
		else if (ch == '>')
		{
			sym = "neq";
			id = "<>";
		}
		else
		{
			sym = "lss";
			id = "<";
			fseek(fin, -1, SEEK_CUR);
		}
	}
	else if (ch == '>')
	{
		getch();
		if (ch == '=')
		{
			sym = "geq";
			id = ">=";
		}
		else
		{
			sym = "gtr";
			id = ">";
			fseek(fin, -1, SEEK_CUR);
		}
	}
	else if (ch == '\'')
	{
		sym = "charsym";
		getch();
		id = ch;
		inum = ch;
		getch();
		if (ch != '\'')
		{
			error(59);
			fseek(fin, -1, SEEK_CUR);
			ch = '\'';			//判断字符是否合法
		}
	}
	else if (ch == '"')
	{
		sym = "str";
		do
		{
			getch();
			id = id + ch;
		} while (ch != '"');
		id.erase(id.end() - 1);
		sleng = id.length();
		inum = sx;
		stab[sx].stab = id;
		sx += 1;
	}
	else
	{
		sym = ssym(ch);
		id = ch;
	}
}

