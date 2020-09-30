#include<stdio.h>
void help()
{
    printf("this is YenKoc parser!");
    printf("-h          :头部信息\n");
    printf("-S          :节区表信息\n");
    printf("-s          :符号表信息\n");
    printf("-l          :程序头信息\n");
    printf("-r          :重定位表信息\n");
}
//读取文件头函数
void readfileheader(char* efile,int num)
{
    printf("ELF Header:\r\n");
    //读取前num个字节，魔数打印出来
    for(int i=0;i<num;i++)
    {
        
    }
}
int main()
{

}