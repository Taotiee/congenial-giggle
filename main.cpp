#include <bits/stdc++.h>
using namespace std;

const char keyword[17][10]= {"void","int","float","char","string","begin","end","if",
                             "then","else","while","do","call","read","write","and","or"
                            };
const char split[7]= {'{','}','(',')',';',' ',','};
const char singleop[3]={'+','-','*'};
const char doubleop[3]={'=','>','<'};
int row=1,col=0;//当前处理的行、列
int temp_col=0;
struct token{
    char name[50];
    int row,col,kind;
};
char  getchar(FILE * fp)//从文件中读取一个字符
{
    char ch=fgetc(fp);
    col++;
    if(ch=='\n')
    {
        temp_col=col-1;
        row++;
        col=0;
    }
    return ch;
}
bool isReserve(char temp[50])//是否为保留字
{
    for(int i=0; i<17; i++)
    {
        if(strcmp(temp,keyword[i])==0)
            return true;
    }
    return false;
}
bool isSplit(char ch)
{
    for(int i=0;i<7;i++)
    {
        if(split[i]==ch)
            return true;
    }
    return false;
}
bool isSingleop(char ch)
{
    for(int i=0;i<3;i++)
    {
        if(singleop[i]==ch)
            return true;
    }
    return false;
}
bool isDoubleop(char ch)
{
    for(int i=0;i<3;i++)
    {
        if(doubleop[i]==ch)
            return true;
    }
    return false;
}
void GETSYM(token &tk,FILE *fp)//词法分析，但具体信息不能在此函数中输出
{
    int i=0;
    char ch;
    for(int i=0; i<50; i++)//识别单词清空
        tk.name[i]='\0';
    ch=getchar(fp);
    if(ch==EOF)
    {
        tk.kind=0;
        return ;
    }
    while(ch==' '||ch=='\n')//过滤空白符
    {
        ch=getchar(fp);
    }
    if(isalpha(ch)||ch=='$')//判断是关键字还是标识符
    {
        tk.row=row;
        tk.col=col;//记录该单词的起始位置
        tk.name[i++]=ch;
        ch=getchar(fp);
        while(isalnum(ch)||ch=='$')
        {
            tk.name[i++]=ch;
            ch=getchar(fp);
        }
        tk.name[i]='\0';
        fseek(fp, -1L, SEEK_CUR);
        col--;
        if(ch=='\n')
        {
            row--;
            col=temp_col;
        }
        if(isReserve(tk.name))//判断是否为保留字
            tk.kind=1;
        else//不是保留字则为标识符
            tk.kind=2;
        return ;
    }
    else if(isdigit(ch)||ch=='.')//判断是整数还是小数
    {
        int flag=0;
        char temp=ch;
        tk.row=row;
        tk.col=col;//记录该单词的起始位置
        ch=getchar(fp);
        if(temp=='.'&&!isdigit(ch))
        {
            cout<<"小数格式错误"<<endl;
            tk.kind=404;
            return ;
        }
        if(temp=='.') flag=1;
        tk.name[i++]=temp;
        while(isdigit(ch)||ch=='.')
        {
            if(ch=='.') flag=1;
            tk.name[i++]=ch;
            ch=getchar(fp);
        }
        tk.name[i]='\0';
        if(flag==1) tk.kind=4;
        else tk.kind=3;
        fseek(fp, -1L, SEEK_CUR);
        col--;
        if(ch=='\n')
        {
            row--;
            col=temp_col;
        }
        return ;
    }

    else if(ch=='\"')//判断字符串
    {
        tk.kind=5;
        tk.row=row;
        tk.col=col;
        tk.name[i]='\"';
        i++;
        ch=getchar(fp);
        while(ch!='\"')
        {
            tk.name[i]=ch;
            ch=getchar(fp);
            i++;
        }
        tk.name[i]='\"';i++;
        tk.name[i]='\0';
        return;
    }
    else if(ch=='\'')//判断字符
    {
        tk.kind=6;
        tk.row=row;
        tk.col=col;
        tk.name[i++]='\'';
        int cnt=0;
        char temp;
        ch=getchar(fp);
        while(ch!='\'')
        {
            temp=ch;
            ch=getchar(fp);
            cnt++;
        }
        if(cnt>1)
        {
            cout<<"字符类型错误，单引号内只能有一个字符"<<endl;
            tk.kind=404;
            return ;
        }
        tk.name[i++]=temp;
        tk.name[i++]='\'';
        tk.name[i]='\0';
        return ;
    }
    else if(isSplit(ch))//判断分隔符
    {
        tk.row=row;
        tk.col=col;
        tk.kind=7;
        tk.name[i++]=ch;
        tk.name[i]='\0';
        return ;
    }
    else if(isSingleop(ch))//判断单分界符
    {
        tk.kind=8;
        tk.row=row;
        tk.col=col;
        tk.name[i++]=ch;
        tk.name[i]='\0';
        return ;
    }
    else if(isDoubleop(ch))//判断双分界符
    {
        tk.kind=8;
        tk.row=row;
        tk.col=col;
        tk.name[i++]=ch;
        char temp=ch;
        ch=getchar(fp);
        if(ch=='='||(ch=='>'&&temp=='<'))
        {
            tk.name[i++]=ch;
            tk.name[i]='\0';
        }
        else
        {
            tk.name[i]='\0';
            fseek(fp, -1L, SEEK_CUR);
            col--;
            if(ch=='\n')
        {
            row--;
            col=temp_col;
        }
        }
        return ;
    }
    else if(ch=='/')
    {
        char temp=ch;
        ch=getchar(fp);
        if(ch!='/'&&ch!='*')
        {
            tk.name[i++]=temp;
            tk.name[i]='\0';
            tk.col=col;
            tk.row=row;
            tk.kind=8;
            fseek(fp, -1L, SEEK_CUR);
            col--;
            if(ch=='\n')
        {
            row--;
            col=temp_col;
        }
            return;
        }
        else if(ch=='/')
        {
            tk.kind=404;
            ch=getchar(fp);
            while(ch!='\n')
            {
                ch=getchar(fp);
            }
            return ;
        }
        else if(ch=='*')
        {
            tk.kind=404;
            ch=getchar(fp);
            while(1)
            {
                if(ch=='*')
                {
                    ch=getchar(fp);
                    if(ch=='/')
                        return;
                    else ch=getchar(fp);
                }
                else ch=getchar(fp);
            }
            return;
        }
    }
    else if(ch==EOF)
    {
        tk.kind=0;
        return ;
    }
}
int main()
{
    FILE * fin=fopen("C:\\Users\\产权\\Desktop\\input.TXT","r");
    token tk;//当前读取单词信息
    tk.kind=-1;
    if(fin==NULL)
    {
        cout<<"文件打开出错";
        return -1;
    }

    while(tk.kind!=0)
    {
        GETSYM(tk,fin);
        if(tk.kind==0) break;
        if(tk.kind==404) continue;
        cout<<'('<<tk.kind<<','<<tk.name<<','<<tk.row<<','<<tk.col<<')'<<endl;
    }
    fclose(fin);
    return 0;
}
