#include<iostream>
#include<map>
#include<fstream>
#include<sstream>
#include<vector>
#include<stack>
using namespace std;


#include<bits/stdc++.h>
using namespace std;

#define N 400

map<string,string> word;
void map_init(){
    word["begin"]="beginsym";
    word["call"]="callsym";
    word["const"]="constsym";
    word["do"]="dosym";
    word["end"]="endsym";
    word["if"]="ifsym";
    word["odd"]="oddsym";
    word["procedure"]="proceduresym";
    word["read"]="readsym";
    word["then"]="thensym";
    word["var"]="varsym";
    word["while"]="whilesym";
    word["write"]="writesym";
    word["+"]="plus";
    word["-"]="minus";
    word["*"]="times";
    word["/"]="slash";
    word["="]="eql";
    word["#"]="neq";
    word["<"]="lss";
    word["<="]="leq";
    word[">"]="gtr";
    word[">="]="geq";
    word[":="]="becomes";
    word["("]="lparen";
    word[")"]="rparen";
    word[","]="comma";
    word[";"]="semicolon";
    word["."]="period";
    word["["]="lbracket";
	word["]"]="rbracket"; 
}

struct node{
    string id;
    string s;
}n[N];

struct middle{
    string place="";
    string code="";
};


map<int,string> errors;             // �����������
vector<string> words;               // �洢�ʷ��������
vector<string> items;               // �洢��Ӧֵ
map<string, middle> A;              // A
int lookahead=-1;                   // ����
string cur;                         // ��ǰ������
int flag=0;
int tk=1;
string op;
string x="";
string y="";
string z="";
stack<string> temp;

void init_tac()
{
    x="";
    y="";
    z="";
    op="";
}

// ���ش���
void load_error()
{
    errors[0] = "�﷨����ȱ��������";
    errors[1] = "�﷨�����쳣������ȱ�����ӻ���Ų�����";
    errors[2] = "�﷨���������쳣";
    errors[3] = "�﷨����ȱ�������";
};

// ���¶�һ���������
void getCur()
{
    lookahead += 1;
    if (lookahead==words.size())          // �����򷵻�
        return;
    cur = words[lookahead];
    temp.push(items[lookahead]);
    // cout << lookahead <<cur<< endl;
};

int lengthoftk()
{
    if (tk/1000!=0)
        return 4;
    else if (tk/100!=0)
        return 3;
    else if (tk/10!=0)
        return 2;
    else if (tk/1!=0)
        return 1;
    return 0;
};

void ParseExpression();

// <����> ::= <��ʶ��>|<�޷�������>| ��(��<���ʽ>��)��
void ParseFactor()
{
    if (cur=="number")
    {
        // A -> number
        x = "T"+to_string(tk);
        y = items[lookahead];
        A[x].code="("+op+", "+y+", "+z+", "+x+")";
        A[x].place=x;
        temp.pop();             // ���ֳ�ջ
        temp.push(x);           // tk��ջ
        tk += 1;
        cout << A[x].code << endl;
        init_tac();
        getCur();
    }
    else if (cur=="ident")
    {
        // A -> ident
        A[items[lookahead]].place=items[lookahead];
        // cout << A[items[lookahead]].code << endl;
        // cout << temp.top() << endl;
        getCur();
    }
    else if (cur=="lparen")
    {
        getCur();
        ParseExpression();
        if (cur=="rparen")
        {
            temp.pop();             // (��ջ
            string cur_item = temp.top();
            temp.pop();             // A1��ջ
            temp.pop();             // (��ջ
            temp.push(cur_item);    // A1��ջ
            // cout << temp.top() << endl;
            getCur();
        }
        else
        {
            cout << errors[0] << endl;
            system("pause");
            exit(0);
        }
    }
    else
    {
        cout << errors[1] << endl;
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
        x = "T"+to_string(tk);
        A[x].place = x;
        string next;
        if (lookahead!=words.size())
        {
            next = temp.top();           // ��һ�����ջ
            temp.pop();
        }
        z = temp.top();                     // �ұ����ջ
        temp.pop();
        op = temp.top();                    // �������ջ
        temp.pop();
        y = temp.top();                     // ������ջ
        temp.pop();
        A[x].code = "("+op+", "+y+", "+z+", "+x+")";
        temp.push(x);
        if (lookahead!=words.size())
            temp.push(next);
        cout << A[x].code << endl;
        init_tac();
        tk += 1;
    }
};

// <���ʽ> ::= [+|-]<��>{<�ӷ������> <��>}
void ParseExpression()
{
    if (cur=="plus"||cur=="minus")
    {
        getCur();                       // �ж��������һ��
        ParseTerm();
        x = "T"+to_string(tk);
        A[x].place = x;
        string next;
        if (lookahead!=words.size())
        {
            next = temp.top();           // ��һ�����ջ
            temp.pop();
        }
        z = temp.top();                     // �ұ����ջ
        temp.pop();
        op = temp.top();                    // �������ջ
        temp.pop();
        A[x].code = "("+op+", "+y+", "+z+", "+x+")";
        temp.push(x);
        if (lookahead!=words.size())
            temp.push(next);
        cout << A[x].code << endl;
        init_tac();
        tk += 1;
    }
    else
    {
        ParseTerm();
    }
    while(cur=="plus"||cur=="minus")
    {
        getCur();
        ParseTerm();
        x = "T"+to_string(tk);
        A[x].place = x;
        string next;
        if (lookahead!=words.size())
        {
            next = temp.top();           // ��һ�����ջ
            temp.pop();
        }
        z = temp.top();                     // �ұ����ջ
        temp.pop();
        op = temp.top();                    // �������ջ
        temp.pop();
        y = temp.top();                     // ������ջ
        temp.pop();
        A[x].code = "("+op+", "+y+", "+z+", "+x+")";
        cout << A[x].code << endl;
        temp.push(x);
        if (lookahead!=words.size())
            temp.push(next);
        init_tac();
        tk += 1;
    }
};

int main()
{
    map_init();//��ʼ��
    char ch;
    int len=0;
    string word1;//string����ʶ�𵥴�
    string str;//string���������ַ�ʶ��
    ifstream infile("analysis.txt");
    cout << "����������ݣ�";
    cin >> str;
    // cout << endl;
    // while(buf&&infile.get(ch)) buf.put(ch);//���ļ��е��ַ�������
    // str= buf.str();//���õ����ַ����浽string���ͱ�����
    int csize=str.length();
    for(int i=0;i<csize;i++){//�������ַ������б���
        while(str[i]==' '||str[i]=='\n'||str[i]==9) i++;//���ʼΪ�ո���з�����ָ���λ��������
        if(i>=csize) break;
        if(str[i]>='A'&&str[i]<='Z') str[i]=str[i]-'A'+'a';
        if(isalpha(str[i])){
            word1=str[i++];
            while(isalpha(str[i])||isdigit(str[i])){
                word1+=str[i++];
            }
            if(word.find(word1)!=word.end()){//�ж��ǲ��ǻ����֣���Ϊ��������������
                // cout<<"("<<word[word1]<<","<<word1<<")"<<endl;
                n[len].id=word[word1];
            }
            else{
                // cout<<"(ident"<<","<<word1<<")"<<endl;
                n[len].id="ident";
            }
            n[len++].s=word1;
            i--;
        }else if(isdigit(str[i])){
            word1=str[i++];
            while(isdigit(str[i])){
                word1+=str[i++];
            }
            // cout<<"(number"<<","<<word1<<")"<<endl;
            n[len].id="number";
            n[len++].s=word1;
            i--;
        }else if(str[i]==':'){//��:=�����ж�
            word1=str[i++];
            if(str[i]=='='){
                word1+=str[i];
                cout<<"("<<word[word1]<<","<<word1<<")"<<endl;
                n[len].id=word[word1];
                n[len++].s=word1;
            }
            else{
                cout<<"(error,"<<word1<<")"<<endl;
                n[len].id="error";
                n[len++].s=word1;
                i--;
            }
        }else if(str[i]=='<'){//��<,<=�ֱ�����ж�
            word1=str[i++];
            if(str[i]=='='){
                word1+=str[i];
                // cout<<"("<<word[word1]<<","<<word1<<")"<<endl;
            }else if(str[i]!=' '||!isdigit(str[i])||!isalpha(str[i])){
                // cout<<"("<<word[word1]<<","<<word1<<")"<<endl;
                i--;
            }
            n[len].id=word[word1];
            n[len++].s=word1;
        }else if(str[i]=='>'){//��>,>=�ֱ�����ж�
            word1=str[i++];
            if(str[i]=='='){
                word1+=str[i];
                // cout<<"("<<word[word1]<<","<<word1<<")"<<endl;
            }else if(str[i]!=' '||!isdigit(str[i])||!isalpha(str[i])){
                // cout<<"("<<word[word1]<<","<<word1<<")"<<endl;
                i--;
            }
            n[len].id=word[word1];
            n[len++].s=word1;
        }else{//�������Ļ��������ν����ж�
            word1=str[i];
            if(word.find(word1)!=word.end()){
                // cout<<"("<<word[word1]<<","<<word1<<")"<<endl;
                n[len].id=word[word1];
                n[len++].s=word1;
            }
            else{
                // cout<<"(error,"<<word1<<")"<<endl;
                n[len].id="error";
                n[len++].s=word1;
            }
        }
    }

    for(int i=0;i<len;i++)
    {
        items.push_back(n[i].s);
        words.push_back(n[i].id);
    }

    load_error();

    getCur();
    ParseExpression();
    if (cur=="rparen" && lookahead!=words.size())
        cout << errors[2] << endl;
    else if (cur!="rparen" && lookahead!=words.size())
        cout << errors[1] << endl;
    else if (flag==0)
        cout << "�﷨��ȷ��" << endl;
    system("pause");
}
