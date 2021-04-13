#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

map<string,string> basic_characters;
map<string,string> opes_dels;

void split(string str, vector<string> &res)
{
    istringstream ss(str);
    string s;
    while(ss >> s)
        res.push_back(s);
};

void load_reserved_word()
{
    // basic characters
    ifstream in;
    in.open("basic_character.txt");
    string temp;
    while(getline(in,temp))
        basic_characters[temp] = temp + "sym";
    // for (auto it=basic_characters.rbegin();it!=basic_characters.rend();it++)
    //     cout <<it->first<<", "<<it->second<< endl;
    in.close();

    // operators and delimiters
    in.open("operator.txt");
    while(getline(in,temp))
    {
        vector<string> ope;
        split(temp, ope);
        opes_dels[ope[0]] = ope[1];
    }
    in.close();
    in.open("delimiter.txt");
    while(getline(in,temp))
    {
        vector<string> ope;
        split(temp, ope);
        opes_dels[ope[0]] = ope[1];
    }
    in.close();
    // for (auto it=opes_dels.rbegin();it!=opes_dels.rend();it++)
    //     cout <<it->first<<", "<<it->second<< endl;
};

void output_result(string key, string value)
{
    cout << "(";
    cout.width(10);
    cout << std::left << key+",";
    cout.width(8);
    cout << std::right << value;
    cout.width(0);
    cout << ")" << endl;
};

void find_word(string substring)
{
    if (basic_characters.find(substring)!=basic_characters.end())
    {
        output_result(basic_characters[substring], substring);
    }
    else
    {
        string char_type = "number";
        for(int k=0; k<substring.size(); k++)
            if ('a' <= substring[k] & substring[k]<= 'z')
            {
                char_type = "ident";
                break;
            }
        output_result(char_type, substring);
    }
};

void lexical_analysis(vector<string> &lines, vector<string> $res)
{
    for(int i=0; i<lines.size();i++)
    {
        int begin = 0;
        int cur = 0;
        while (begin<lines[i].size())
        {
            // space
            char a = lines[i][cur];
            string sub(1, a);
            // cout << begin<<" " << cur << " "<< sub << endl;
            if (lines[i][cur]==' ')
            {
                if (cur!=begin)
                {
                    find_word(lines[i].substr(begin, cur-begin));
                    begin = cur;
                }
                cur += 1;
                begin += 1;
            }
            else if (cur>0 && opes_dels.find(lines[i].substr(cur-1,2))!=opes_dels.end())
            {
                // cout << "in" << endl;
                if (cur-begin>1)
                {
                    find_word(lines[i].substr(begin, cur-1-begin));
                    begin = cur-1;
                }
                output_result(opes_dels[lines[i].substr(begin,cur-begin+1)],lines[i].substr(begin, cur-begin+1));
                cur += 1;
                begin = cur;
            }
            else if (opes_dels.find(sub)!=opes_dels.end() && (lines[i][cur+1]=='\0' | opes_dels.find(lines[i].substr(cur,2))==opes_dels.end()))
            {
                if (cur!=begin)
                {
                    find_word(lines[i].substr(begin, cur-begin));
                    begin = cur;
                }
                output_result(opes_dels[sub], sub);
                cur += 1;
                begin = cur;
            }
            else
            {
                cur += 1;
            }

            if (cur==lines[i].size()& begin!=cur)
            {
                find_word(lines[i].substr(begin, cur-begin));
                break;
            }
        }
    }
}

int main()
{
    load_reserved_word();

    string filename="case_6.txt";
    vector<string> lines;
    string temp;
    ifstream in;
    in.open(filename);
    while(getline(in, temp))
    {
        transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        lines.push_back(temp);
    }
    in.close();

    vector<string> res;
    lexical_analysis(lines, res);

    getchar();
}