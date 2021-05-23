#include<iostream>
#include<map>
#include<fstream>
#include<sstream>
#include<vector>
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


map<int,string> errors;             // 储存错误类型
vector<string> words;               // 存储词法分析结果
int lookahead=-1;                   // 索引
string cur;                         // 当前词类型
int flag=0;

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
    // cout << lookahead <<cur<< endl;
};

void ParseExpression();

// <因子> ::= <标识符>{'['<表达式>']'|'('<表达式>{,<表达式>}')'}|<无符号整数>| ‘(’<表达式>‘)’
void ParseFactor()
{
    if (cur=="ident"){
        getCur();
        if(cur=="lbracket"){//改了这里-20210515WZL
			getCur();
        	ParseExpression();
        	if(cur=="rbracket"){
				getCur();
			}
        	else
        	{
        		cout << errors[0] << endl;
        		flag=1;
        		return;
			}
		}
		else if(cur=="lparen"){
			getCur();
			ParseExpression();
			while(cur=="comma"){
				getCur();
				ParseExpression();
			}
			if(cur=="rparen"){
				getCur();
			}
			else{
				cout << errors[0] << endl;
            	flag=1;
            	return;	
			}
		}
	}
	else if(cur=="number"){
		getCur();
	}
    // 表达式
    else if (cur=="lparen")
    {
        getCur();
        ParseExpression();
        if (cur=="rparen"){
            getCur();
        }
        else
        {
            // 缺少右括号
            cout << errors[0] << endl;
            flag=1;
            return;
        }
    }
    else
    {
        // 不符合因子中的任何情况
        cout << errors[1] << endl;
        flag=2;
        return;
    }
};

// <项> ::= <因子>{<乘法运算符> <因子>}
void ParseTerm()
{
    ParseFactor();
    if(flag) return;
    while (cur=="times"||cur=="slash")
    {
        getCur();
        ParseFactor();
        if(flag) return;
    }
};

// <表达式> ::= [+|-]<项>{<加法运算符> <项>}
void ParseExpression()
{
    if (cur=="plus" || cur=="minus")
    {
        getCur();                       // 判断完后，走下一个
        ParseTerm();
        if(flag) return;
    }
    else
    {
        ParseTerm();
        if(flag) return;
    }
    while(cur=="plus"||cur=="minus")
    {
        getCur();
        ParseTerm();
        if(flag) return;
    }
};

// <条件表达式> ::= <表达式> <关系运算符> <表达式> |ODD<表达式>
void ParseConditionExpression()
{
    if (cur=="oddsym")
    {
        getCur();
        ParseExpression();
        if(flag) {system("pause");
            exit(0);}
    }
    else
    {
        ParseExpression();
        if (flag) {system("pause");
            exit(0);}
        if (cur=="eql"||cur=="neq"||cur=="lss"||
            cur=="leq"||cur=="gtr"||cur=="geq")
        {
            getCur();
        }
        else
        {
            cout << errors[3] << endl;
            system("pause");
            exit(0);
        }
        ParseExpression();
        if(flag) {system("pause");
            exit(0);}
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
        words.push_back(n[i].id);

    load_error();
    // string x;
    // cout << "输入测试样例：" << endl;
    // cin >> x;
    // string filename="case"+x+".txt";
    // string temp;
    // fstream in;
    // in.open(filename);
    // while(getline(in, temp))
    // {
    //     if (temp[0]=='/')
    //         continue;
    //     int i = 0;
    //     for (i; i<temp.size(); i++)
    //         if (temp[i]==' ' or temp[i]==',')
    //             break;
    //     words.push_back(temp.substr(1, i-1));
    // }

    getCur();
    cout<<"表达式判断：";
    ParseExpression();
    if (cur=="rparen" && lookahead!=words.size())
        cout << errors[2] << endl;
    else if (cur!="rparen" && lookahead!=words.size())
        cout << errors[1] << endl;
    else if (flag==0)
        cout << "语法正确，是表达式" << endl;

    lookahead=-1;
    flag=0;
    getCur();
    cout<<"条件表达式判断：";
    ParseConditionExpression();
    if (cur=="rparen" && lookahead!=words.size())
        cout << errors[2] << endl;
    else if (lookahead!=words.size())
        cout << errors[1] << endl;
    else if(flag==0)
        cout << "语法正确，是条件表达式" << endl;
    system("pause");
}
