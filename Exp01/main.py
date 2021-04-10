import re
pat_new = []
pat_new.append(re.compile('const (.*);'))
pat_new.append(re.compile('var (.*);'))

pat_detect = []
pat_detect.append(re.compile('precedure (.*)'))
pat_detect.append(re.compile(' +(.*):=(.*);'))
pat_detect.append(re.compile('read\((.*)\);'))
pat_detect.append(re.compile('write\((.*)\);'))



def detect(dic, line):
    for pat in pat_new:
        elem = re.findall(pat,line)
        if len(elem) == 0:
            continue
        elems = re.split(r'[,s]', elem[0])
        for e in elems:
            e = re.findall('(.*)=', e)[0] if len(re.findall('(.*)=', e))>0 else e
            dic[e] = 1
    for pat in pat_detect:
        elem = re.findall(pat,line)
        if len(elem)==0:
            continue
        

if __name__=="__main__":
    with open("pro.txt", "r") as f:
        lines = f.readlines()
    
    dic = {}
    for line in lines:
        detect(dic, line[:-1].lower())
    print(dic)