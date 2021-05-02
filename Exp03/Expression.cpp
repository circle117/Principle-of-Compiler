#include<iostream>
#include<map>
#include<fstream>
#include<sstream>
#include<vector>
using namespace std;

map<int,string> errors;
int e_num;
vector<string> words;
int lookahead=-1;
string cur;

void load_error()
{
    errors[0] = "语法错误：缺少右括号";
    errors[1] = "语法错误：缺少因子";
    errors[2] = "语法错误：括号异常";
};

// 往下读一个单词类别
void getCur()
{
    if (lookahead==words.size()-1)
        return;
    lookahead += 1;
    cur = words[lookahead];
    cout << cur << endl;
};

void ParseExpression();

// <因子> ::= <标识符>|<无符号整数>| ‘(’<表达式>‘)’
void ParseFactor()
{
    if (cur=="ident"||cur=="number")
        getCur();
    else if (cur=="lparen")
    {
        getCur();
        ParseExpression();
        if (cur=="rparen")
            getCur();
        else
        {
            cout << errors[0] << endl;
            system("pause");
            exit(0);
        }
    }
    else
    {
        cout << cur+errors[1] << endl;
        system("pause");
        exit(0);
    }
};

// <项> ::= <因子>{<乘法运算符> <因子>}
void ParseTerm()
{
    ParseFactor();
    while (cur=="times"||cur=="slash")
    {
        getCur();
        ParseFactor();
    }
};

// <表达式> ::= [+|-]<项>{<加法运算符> <项>}
void ParseExpression()
{
    if (cur=="plus" || cur=="minus")
    {
        getCur();                       // 判断完后，走下一个
        ParseTerm();
    }
    else
    {
        ParseTerm();
    }
    while(cur=="plus"||cur=="minus")
    {
        getCur();
        ParseTerm();
    }
};

int main()
{
    load_error();
    string x;
    cout << "输入测试样例：";
    cin >> x;
    string filename="case"+x+".txt";
    string temp;
    fstream in;
    in.open(filename);
    while(getline(in, temp))
    {
        if (temp[0]=='/')
            continue;
        int i = 0;
        for (i; i<temp.size(); i++)
            if (temp[i]==' ' or temp[i]==',')
                break;
        words.push_back(temp.substr(1, i-1));
    }

    getCur();
    ParseExpression();
    if (cur=="rparen")
    {
        cout << errors[2] << endl;
        system("pause");
        exit(0);
    }
    else if (lookahead!=words.size()-1)
        cout << errors[1] << endl;
    else
        cout << "语法正确。" << endl;
    system("pause");
}