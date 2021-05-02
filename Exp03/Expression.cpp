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
    errors[0] = "�﷨����ȱ��������";
    errors[1] = "�﷨����ȱ������";
    errors[2] = "�﷨���������쳣";
};

// ���¶�һ���������
void getCur()
{
    if (lookahead==words.size()-1)
        return;
    lookahead += 1;
    cur = words[lookahead];
    cout << cur << endl;
};

void ParseExpression();

// <����> ::= <��ʶ��>|<�޷�������>| ��(��<���ʽ>��)��
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

// <��> ::= <����>{<�˷������> <����>}
void ParseTerm()
{
    ParseFactor();
    while (cur=="times"||cur=="slash")
    {
        getCur();
        ParseFactor();
    }
};

// <���ʽ> ::= [+|-]<��>{<�ӷ������> <��>}
void ParseExpression()
{
    if (cur=="plus" || cur=="minus")
    {
        getCur();                       // �ж��������һ��
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
    cout << "�������������";
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
        cout << "�﷨��ȷ��" << endl;
    system("pause");
}