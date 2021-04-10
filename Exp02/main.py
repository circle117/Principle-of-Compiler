import re
# 基本字
basic_characters = {}
with open('basic_character.txt', 'r') as f:
    characters = f.readlines()
for character in characters:
    basic_characters[character[:-1]] = ''.join([character[:-1],'sym'])
# print(basic_characters)

# 运算符
opes_dels = {}
with open('operator.txt', 'r') as f:
    characters = f.readlines()
for character in characters:
    character = character[:-1].split(' ')
    opes_dels[character[0]] = character[1]
with open('delimiter.txt', 'r') as f:
    characters = f.readlines()
for character in characters:
    character = character[:-1].split(' ')
    opes_dels[character[0]] = character[1]
# print(opes_dels)

def lexical_analysis(line, res):
    word = re.split(r'[\+\-\*/=(:=)(<=)(>=)#<>\(\),;.(\s*)]', line)
    i = len(word)-1
    while i>-1:
        if word[i]=='':
            word.pop(i)
        i -= 1
    print(word)
    pos_word = 0
    pos_line = 0
    line = line.strip()
    print(line)
    while pos_line<len(line):
        # 基本字
        if pos_word<len(word) and word[pos_word] in basic_characters:
            res.append(''.join(['(', (basic_characters[word[pos_word]]+',').ljust(10),
                        word[pos_word].rjust(8), ')']))
            pos_line += len(word[pos_word])
            pos_word += 1
        # 空格
        elif line[pos_line] == ' ':
            pos_line += 1
        # 二位符号
        elif pos_line+1<len(line) and line[pos_line:pos_line+2] in opes_dels:
            res.append(''.join(['(', (opes_dels[line[pos_line:pos_line+2]]+',').ljust(10),
                        line[pos_line:pos_line+2].rjust(8), ')']))
            pos_line += 2
        # 一位符号
        elif line[pos_line] in opes_dels:
            res.append(''.join(['(', (opes_dels[line[pos_line]]+',').ljust(10),
                        line[pos_line].rjust(8), ')']))
            pos_line += 1
        else:
            cha_type = 'number'
            for elem in word[pos_word]:
                if 'a'<=elem<='z':
                    cha_type = 'ident'
                    break
            res.append(''.join(['(', (cha_type+',').ljust(10),
                        word[pos_word].rjust(8), ')']))
            pos_line += len(word[pos_word])
            pos_word += 1
            

if __name__=="__main__":
    # 读入输入样例
    with open('pro.txt', 'r') as f:
        lines = f.readlines()

    result = []
    for line in lines:
        lexical_analysis(line[:-1].lower(), result)
    for elem in result:
        print(elem)
    