#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cstdio>
#include<map>
#include<algorithm>
#include<assert.h>
using namespace std;
vector<string>keywords;
void readlines(char* filename,vector<vector<pair<bool,string>>>&lines){
    FILE*fp=fopen(filename,"r");
    string s,e;
    s.clear();e.clear();
    lines.push_back(vector<pair<bool,string>>());
    size_t cnt=0;
    int type=1;
    // 1:空串，2:字母/数字/下划线，3:运算符!%^|&*-+=<>./，4:引号内字符串，5:单行注释，6:多行注释，7:宏
    // 负数：存在反斜杠
    // 立即符号：([{?:~,}]);
    while(!feof(fp)){
        char c=fgetc(fp);
        if(c=='\t'||c==' '){
            if(type<0){
                s+='\\';
                s+=c;
                type=-type;
            }
            if(type==4||type==5||type==6||type==7)s+=c;
            else{
                e+=c;
                if(type!=1){
                    lines[cnt].push_back(make_pair(1,s));
                    s.clear();
                    type=1;
                }
            }
        }
        else if(c=='\n'){
            if(type==-6||type==-7){
                s+="\\\n";
                type=-type;
            }
            else if(type<0)type=-type;
            else if(type==6)s+='\n';
            else{
                if(type!=1)lines[cnt].push_back(make_pair(type==2||type==3||type==4,s));
                cnt++;
                lines.push_back(vector<pair<bool,string>>());
                s.clear();e.clear();
                type=1;
            }
        }
        else if(c=='\\'){
            type=-type;
            if(!e.empty()){
                lines[cnt].push_back(make_pair(0,e));
                e.clear();
            }
        }
        else if(c=='('||c=='['||c=='{'||c=='?'||c==':'||c=='~'||c==','||c=='}'||c==']'||c==')'||c==';'){
            if(type<0){
                type=-type;
                if(type==4||type==5||type==6||type==7)s+='\\';
            }
            if(type==2||type==3){
                lines[cnt].push_back(make_pair(1,s));
                s.clear();
                type=1;
            }
            if(type==1){
                if(!e.empty()){
                    lines[cnt].push_back(make_pair(0,e));
                    e.clear();
                }
                lines[cnt].push_back(make_pair(1,string(1,c)));
            }
            else s+=c;
        }
        else if(c=='\''||c=='\"'){
            if(type==-5||type==-6||type==-7){
                type=-type;
                s+='\\';
            }
            if(type==1){
                if(!e.empty()){
                    lines[cnt].push_back(make_pair(0,e));
                    e.clear();
                }
                type=4;
                s+=c;
            }
            else if(type==-4){
                type=4;
                s+='\\';
                s+=c;
            }
            else if(type==4){
                s+=c;
                lines[cnt].push_back(make_pair(1,s));
                s.clear();
                type=1;
            }
            else if(type==5||type==6||type==7)s+=c;
            else{
                lines[cnt].push_back(make_pair(1,s));
                s=string(1,c);
                type=4;
            }
        }
        else if(c=='!'||c=='^'||c=='%'||c=='|'||c=='&'||c=='*'||c=='-'||c=='+'||c=='='||c=='<'||c=='>'||c=='.'||c=='/'){
            if(type<0){
                type=-type;
                s+='\\';
                s+=c;
            }
            else if(type==1){
                if(!e.empty()){
                    lines[cnt].push_back(make_pair(0,e));
                    e.clear();
                }
                type=3;
                s=string(1,c);
            }
            else if(type==3){
                if(s.back()=='/'&&c=='/'){
                    s.pop_back();
                    lines[cnt].push_back(make_pair(1,s));
                    type=5;
                    s="//";
                }
                else if(s.back()=='/'&&c=='*'){
                    s.pop_back();
                    lines[cnt].push_back(make_pair(1,s));
                    type=6;
                    s="/*";
                }
                else s+=c;
            }
            else if(type==6){
                if(s.back()=='*'&&c=='/'){
                    s+=c;
                    lines[cnt].push_back(make_pair(0,s));
                    s.clear();
                    type=1;
                }
                else s+=c;
            }
            else if(type==4||type==5||type==7)s+=c;
            else{
                lines[cnt].push_back(make_pair(1,s));
                type=3;
                s=string(1,c);
            }
        }
        else if(c=='#'){
            if(type<0){
                type=-type;
                s+='\\';
            }
            if(type==4||type==5||type==6||type==7)s+=c;
            else{
                s="#";
                type=7;
                e.clear();
            }
        }
        else if((c<='Z'&&c>='A')||(c<='z'&&c>='a')||(c<='9'&&c>='0')||c=='_'){
            if(type<0){
                type=-type;
                s+='\\';
            }
            if(type==1){
                if(!e.empty()){
                    lines[cnt].push_back(make_pair(0,e));
                    e.clear();
                }
                type=2;
                s+=c;
            }
            else if(type==3){
                lines[cnt].push_back(make_pair(1,s));
                type=2;
                s=string(1,c);
            }
            else s+=c;
        }
        else if(type==4||type==5||type==6||type==7)s+=c;
    }
    if(type!=1){
        lines[cnt].push_back(make_pair(type==2||type==3,s));
    }
}
void work(char* filename){
    vector<vector<pair<bool,string>>>lines;
    readlines(filename,lines);
    map<string,size_t>cnt,rank;
    vector<pair<size_t,string>>rk;
    for(auto&line:lines)for(auto&s:line)if(s.first)cnt[s.second]--;
    for(auto i:cnt)rk.push_back(make_pair(i.second,i.first));
    sort(rk.begin(),rk.end());
    FILE*fp=fopen(filename,"w");
    for(size_t i=0,icnt=0,add=keywords.size();i<rk.size();i++){
        fprintf(fp,"#define ");
        if(icnt==add){
            add*=keywords.size();
            icnt=0;
        }
        size_t r=rank[rk[i].second]=icnt+add;
        while(r>1){
            fprintf(fp,keywords[r%keywords.size()].c_str());
            r/=keywords.size();
        }
        fprintf(fp," %s\n",rk[i].second.c_str());
        icnt++;
    }
    bool e=0;
    for(auto&line:lines){
        for(auto&i:line)
            if(i.first){
                if(!e)fputc(' ',fp);
                size_t r=rank[i.second];
                while(r>1){
                    fprintf(fp,keywords[r%keywords.size()].c_str());
                    r/=keywords.size();
                }
                e=0;
            }
            else{
                fprintf(fp,"%s",i.second.c_str());
                e=1;
            }
        fputc('\n',fp);e=1;
    }
}
void readKeywords(char* filename){
    FILE*fp=fopen(filename,"r");
    string s;s.clear();
    keywords.clear();
    while(!feof(fp)){
        wint_t c=fgetwc(fp);
        if(c==(wint_t)'\n'||c==(wint_t)'\t'||c==(wint_t)' '||c==(wint_t)'\r'){
            if(!s.empty()){
                keywords.push_back(s);
                s.clear();
            }
        }
        else if(c!=0&&c!=(wint_t)EOF)s+=c;
    }
    if(!s.empty())keywords.push_back(s);
}
int main(int argc,char *argv[]){
    keywords=vector<string>{"喵","呜"};
    for(int i=1;i<argc;i++){
        if(string(argv[i])=="-k")
            readKeywords(argv[++i]);
        else if(string(argv[i])=="-f")
            work(argv[++i]);
    }
    return 0;
}
