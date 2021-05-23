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


map<int,string> errors;             // �����������
vector<string> words;               // �洢�ʷ��������
int lookahead=-1;                   // ����
string cur;                         // ��ǰ������
int flag=0;

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
    // cout << lookahead <<cur<< endl;
};

void ParseExpression();

// <����> ::= <��ʶ��>{'['<���ʽ>']'|'('<���ʽ>{,<���ʽ>}')'}|<�޷�������>| ��(��<���ʽ>��)��
void ParseFactor()
{
    if (cur=="ident"){
        getCur();
        if(cur=="lbracket"){//��������-20210515WZL
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
    // ���ʽ
    else if (cur=="lparen")
    {
        getCur();
        ParseExpression();
        if (cur=="rparen"){
            getCur();
        }
        else
        {
            // ȱ��������
            cout << errors[0] << endl;
            flag=1;
            return;
        }
    }
    else
    {
        // �����������е��κ����
        cout << errors[1] << endl;
        flag=2;
        return;
    }
};

// <��> ::= <����>{<�˷������> <����>}
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

// <���ʽ> ::= [+|-]<��>{<�ӷ������> <��>}
void ParseExpression()
{
    if (cur=="plus" || cur=="minus")
    {
        getCur();                       // �ж��������һ��
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

// <�������ʽ> ::= <���ʽ> <��ϵ�����> <���ʽ> |ODD<���ʽ>
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
        words.push_back(n[i].id);

    load_error();
    // string x;
    // cout << "�������������" << endl;
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
    cout<<"���ʽ�жϣ�";
    ParseExpression();
    if (cur=="rparen" && lookahead!=words.size())
        cout << errors[2] << endl;
    else if (cur!="rparen" && lookahead!=words.size())
        cout << errors[1] << endl;
    else if (flag==0)
        cout << "�﷨��ȷ���Ǳ��ʽ" << endl;

    lookahead=-1;
    flag=0;
    getCur();
    cout<<"�������ʽ�жϣ�";
    ParseConditionExpression();
    if (cur=="rparen" && lookahead!=words.size())
        cout << errors[2] << endl;
    else if (lookahead!=words.size())
        cout << errors[1] << endl;
    else if(flag==0)
        cout << "�﷨��ȷ�����������ʽ" << endl;
    system("pause");
}
