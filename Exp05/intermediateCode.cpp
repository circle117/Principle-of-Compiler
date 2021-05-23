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


map<int,string> errors;             // 储存错误类型
vector<string> words;               // 存储词法分析结果
vector<string> items;               // 存储对应值
map<string, middle> A;              // A
int lookahead=-1;                   // 索引
string cur;                         // 当前词类型
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

// 加载错误
void load_error()
{
    errors[0] = "语法错误：缺少右括号";
    errors[1] = "语法错误：异常结束，缺少因子或符号不存在";
    errors[2] = "语法错误：括号异常";
    errors[3] = "语法错误：缺少运算符";
};

// 往下读一个单词类别
void getCur()
{
    lookahead += 1;
    if (lookahead==words.size())          // 结束则返回
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

// <因子> ::= <标识符>|<无符号整数>| ‘(’<表达式>‘)’
void ParseFactor()
{
    if (cur=="number")
    {
        // A -> number
        x = "T"+to_string(tk);
        y = items[lookahead];
        A[x].code="("+op+", "+y+", "+z+", "+x+")";
        A[x].place=x;
        temp.pop();             // 数字出栈
        temp.push(x);           // tk入栈
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
            temp.pop();             // (出栈
            string cur_item = temp.top();
            temp.pop();             // A1出栈
            temp.pop();             // (出栈
            temp.push(cur_item);    // A1入栈
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

// <项> ::= <因子>{<乘法运算符> <因子>}
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
            next = temp.top();           // 下一个项出栈
            temp.pop();
        }
        z = temp.top();                     // 右边项出栈
        temp.pop();
        op = temp.top();                    // 运算符出栈
        temp.pop();
        y = temp.top();                     // 左边项出栈
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

// <表达式> ::= [+|-]<项>{<加法运算符> <项>}
void ParseExpression()
{
    if (cur=="plus"||cur=="minus")
    {
        getCur();                       // 判断完后，走下一个
        ParseTerm();
        x = "T"+to_string(tk);
        A[x].place = x;
        string next;
        if (lookahead!=words.size())
        {
            next = temp.top();           // 下一个项出栈
            temp.pop();
        }
        z = temp.top();                     // 右边项出栈
        temp.pop();
        op = temp.top();                    // 运算符出栈
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
            next = temp.top();           // 下一个项出栈
            temp.pop();
        }
        z = temp.top();                     // 右边项出栈
        temp.pop();
        op = temp.top();                    // 运算符出栈
        temp.pop();
        y = temp.top();                     // 左边项出栈
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
    map_init();//初始化
    char ch;
    int len=0;
    string word1;//string变量识别单词
    string str;//string变量进行字符识别
    ifstream infile("analysis.txt");
    cout << "输入测试数据：";
    cin >> str;
    // cout << endl;
    // while(buf&&infile.get(ch)) buf.put(ch);//将文件中的字符读出来
    // str= buf.str();//将得到的字符储存到string类型变量中
    int csize=str.length();
    for(int i=0;i<csize;i++){//对整个字符串进行遍历
        while(str[i]==' '||str[i]=='\n'||str[i]==9) i++;//若最开始为空格或换行符，则将指针的位置往后移
        if(i>=csize) break;
        if(str[i]>='A'&&str[i]<='Z') str[i]=str[i]-'A'+'a';
        if(isalpha(str[i])){
            word1=str[i++];
            while(isalpha(str[i])||isdigit(str[i])){
                word1+=str[i++];
            }
            if(word.find(word1)!=word.end()){//判断是不是基本字，若为基本字则进行输出
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
        }else if(str[i]==':'){//对:=进行判断
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
        }else if(str[i]=='<'){//对<,<=分别进行判断
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
        }else if(str[i]=='>'){//对>,>=分别进行判断
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
        }else{//对其他的基本字依次进行判断
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
        cout << "语法正确。" << endl;
    system("pause");
}
