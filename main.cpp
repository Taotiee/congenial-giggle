#include <bits/stdc++.h>
using namespace std;

const char keyword[17][10]= {"void","int","float","char","string","begin","end","if",
                             "then","else","while","do","call","read","write","and","or"
                            };
const char split[7]= {'{','}','(',')',';',' ',','};
const char singleop[3]={'+','-','*'};
const char doubleop[3]={'=','>','<'};
int row=1,col=0;//��ǰ������С���
int temp_col=0;
struct token{
    char name[50];
    int row,col,kind;
};
char  getchar(FILE * fp)//���ļ��ж�ȡһ���ַ�
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
bool isReserve(char temp[50])//�Ƿ�Ϊ������
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
void GETSYM(token &tk,FILE *fp)//�ʷ���������������Ϣ�����ڴ˺��������
{
    int i=0;
    char ch;
    for(int i=0; i<50; i++)//ʶ�𵥴����
        tk.name[i]='\0';
    ch=getchar(fp);
    if(ch==EOF)
    {
        tk.kind=0;
        return ;
    }
    while(ch==' '||ch=='\n')//���˿հ׷�
    {
        ch=getchar(fp);
    }
    if(isalpha(ch)||ch=='$')//�ж��ǹؼ��ֻ��Ǳ�ʶ��
    {
        tk.row=row;
        tk.col=col;//��¼�õ��ʵ���ʼλ��
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
        if(isReserve(tk.name))//�ж��Ƿ�Ϊ������
            tk.kind=1;
        else//���Ǳ�������Ϊ��ʶ��
            tk.kind=2;
        return ;
    }
    else if(isdigit(ch)||ch=='.')//�ж�����������С��
    {
        int flag=0;
        char temp=ch;
        tk.row=row;
        tk.col=col;//��¼�õ��ʵ���ʼλ��
        ch=getchar(fp);
        if(temp=='.'&&!isdigit(ch))
        {
            cout<<"С����ʽ����"<<endl;
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

    else if(ch=='\"')//�ж��ַ���
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
    else if(ch=='\'')//�ж��ַ�
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
            cout<<"�ַ����ʹ��󣬵�������ֻ����һ���ַ�"<<endl;
            tk.kind=404;
            return ;
        }
        tk.name[i++]=temp;
        tk.name[i++]='\'';
        tk.name[i]='\0';
        return ;
    }
    else if(isSplit(ch))//�жϷָ���
    {
        tk.row=row;
        tk.col=col;
        tk.kind=7;
        tk.name[i++]=ch;
        tk.name[i]='\0';
        return ;
    }
    else if(isSingleop(ch))//�жϵ��ֽ��
    {
        tk.kind=8;
        tk.row=row;
        tk.col=col;
        tk.name[i++]=ch;
        tk.name[i]='\0';
        return ;
    }
    else if(isDoubleop(ch))//�ж�˫�ֽ��
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
    FILE * fin=fopen("C:\\Users\\��Ȩ\\Desktop\\input.TXT","r");
    token tk;//��ǰ��ȡ������Ϣ
    tk.kind=-1;
    if(fin==NULL)
    {
        cout<<"�ļ��򿪳���";
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
