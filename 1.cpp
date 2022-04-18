/*
实验一：倒排索引的构建及向量空间模型检索
实验内容
•   对硬盘目录中的10个文本文件（doc01.txt – doc10.txt），在内存中建立倒排索引。
•   构建检索系统，输入两个查询词，输出包含查询词的文件ID。
*/

//参考：https://blog.csdn.net/weixin_45730130/article/details/115967403

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <fstream>
using namespace std;

static vector<string> files = { //文件名称
    "doc1.txt","doc2.txt", "doc3.txt",
    "doc4.txt","doc5.txt", "doc6.txt",
    "doc7.txt","doc8.txt","doc9.txt","doc10.txt"};

struct Doc {
    string docName; //文档名
    int docID{}; //文档ID
};

vector<Doc> Docs; //文档集

void makeDocs (vector<Doc> &docs) { //生成文档集
    Doc *doc = new Doc;
    for (int i = 0; i < files.size(); i++) {
        doc->docName = files[i]; //记录该文件的文件名
        doc->docID = i + 1; //为该文件赋docID
        docs.push_back(*doc); //将该文件记录到文档集docs中
    }
    delete doc;
}

void showDocs () { //显示文档集
    for (int i = 0; i < Docs.size(); i++) {
        cout << "DocName: " << Docs[i].docName << "\tdocID: " << Docs[i].docID << endl;
    }
}

map<string,set<int>> indexList; //倒排索引表

void index (vector<Doc> &docs) { //由文档建立倒排索引表
    for (int i = 0; i < docs.size(); i++) { //依次将每个文件添加到倒排索引表中
        ifstream in(docs[i].docName); //尝试打开文件
        int ch; //ch：扫描到的字母
        string s; //s：获取到的单词
        map<string, set<int>>::iterator it; //it：用于判定词项是否在倒排记录表中
        if (in.is_open()) { //文件成功打开
            while (!in.eof()) { //一次循环获取一个单词
                do { //首先找到第一个字母
                    ch = in.get(); //获取一个字符
                } while (!isalpha(ch) && !in.eof()); //不是字母且未到文件尾则继续循环
                if (in.eof()) { //防止将空格添加到全体倒排索引表中
                    break;
                }
                while (isalpha(ch)) { //将当前单词放入s中
                    ch = tolower(char(ch)); //将字母转换为小写
                    s += ch; //合成单词，将每个字母依次放入s中
                    ch = in.get(); //读取下一个字母
                }
                it = indexList.find(s); //判断s是否已经存在于全体倒排索引表中
                if (it != indexList.end()) { //如果s已经存在于全体倒排索引表中
                    it->second.insert(i + 1); //直接将词项对应的docID加入倒排索引表中
                }
                else { //如果s不在全体倒排索引表中，需要将该词项生成新的倒排索引表并加入到全体倒排索引表中
                    set<int> newSet{i + 1}; //将s词项对应的文档编号放入新的倒排索引表中
                    pair<string, set<int>> newTerm(s, newSet); //生成新词项及其倒排记录表
                    indexList.insert(newTerm); //将新词项及其倒排记录表加入全体倒排记录表
                }
                s.clear(); //清空s,进行下一个单词的操作
            }
        }
        in.close(); //关闭当前文件
    }
    cout << "倒排索引构建成功" << endl;
}

void showIndexList (map<string, set<int>> indexList) { //使用前向迭代器将indexList中的数据输出
    map<string, set<int>> ::iterator it;
    map<string, set<int>> ::iterator itEnd;
    it = indexList.begin();
    itEnd = indexList.end();
    while (it != itEnd) {
        cout << setw(20) << it->first << "\t\t";
        set<int> sec = it->second; //再使用一个前向迭代器将sec中的数据输出
        set<int> ::iterator it2;
        set<int> ::iterator it2End;
        it2 = sec.begin();
        it2End = sec.end();
        while (it2 != it2End) {
            cout << *it2 << " ";
            it2++;
        }
        cout << endl;
        it++;
    }
}

void query (string s) { //查询s是否存在于倒排索引表中
    map<string, set<int>>::iterator it;
    it = indexList.find(s); //判断s是否在indexList中
    if (it != indexList.end()) { //存在
        cout << s << "出现的文档docID:";
        set<int> sec = it->second;
        set<int> ::iterator it2;
        set<int> ::iterator it2End;
        it2 = sec.begin();
        it2End = sec.end();
        while (it2 != it2End) {
            cout << *it2 << " ";
            it2++;
        }
        cout << endl;
    }
    else { //不存在
        cout << s << "没有在文档中出现" << endl;
    }
}

bool flag1[20], flag2[20], f;

void query(string s1, string s2, int size) {
    for (int i = 0; i <= 20; i++) {
        flag1[i] = false;
        flag2[i] = false;
    }
    f = false;
    map<string, set<int>>::iterator it;
    it = indexList.find(s1);
    if (it != indexList.end()) {
        set<int> sec = it->second;
        set<int> ::iterator it2;
        set<int> ::iterator it2End;
        it2 = sec.begin();
        it2End = sec.end();
        while (it2 != it2End) {
            flag1[*it2] = true;
            it2++;
        }
    }
    else {
        cout << s1 << "没有在文档中出现" << endl;
        f = true;
    }
    it = indexList.find(s2);
    if (it != indexList.end()) {
        set<int> sec = it->second;
        set<int> ::iterator it2;
        set<int> ::iterator it2End;
        it2 = sec.begin();
        it2End = sec.end();
        while (it2 != it2End) {
            flag2[*it2] = true;
            it2++;
        }
    }
    else {
        cout << s2 << "没有在文档中出现" << endl;
        f = true;
    }
    if (!f) {
        cout << s1 << "和" << s2 << "共同出现的文档docID:";
        for (int i = 1; i <= size; i++) {
            if(flag1[i] && flag2[i]) {
                cout << i << " ";
            }
        }
        cout << endl;
    }
}

int main() {
    makeDocs(Docs); //生成文档集
    showDocs(); //输出文档集
    index(Docs); //建立倒排索引表
    showIndexList(indexList); //输出倒排索引表
    string s1, s2;
    cout << "请输入第一个查询词：";
    cin >> s1;
    cout << "请输入第二个查询词：";
    cin >> s2;
    int size = Docs.size();
    query(s1, s2, size); //查询这两个词是否在倒排索引表中
    cout << endl;
    return 0;
}
