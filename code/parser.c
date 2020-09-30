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
    printf(" Magic: ");
    //读取前num个字节，魔数打印出来
    for(int i=0;i<num;i++)
    {
        printf("%02X",efile[i]);
        putchar(" ");
    }
    printf("\r\n");
    //文件类型
    printf(" %-33s:","class");
    switch (efile[4])
    {
    case 0:
        printf("Invalid class\r\n");
        /* code */
        break;
    case 1:
        printf(" ELF32\r\n");
        break;
    case 2:
        printf(" ELF64\r\n");
        break;
    default:
        printf(' ERROR\r\n');
        break;
    }
    //数据
    printf(" %-33s","Data");
    switch (efile[5])
    {
    case 0:
        printf(" Invalid data encoding\r\n");
        break;
    case 1:
        printf(" 2's complement,little endian\r\n");
        break;
    case 2:
        printf(" 2's complement, big endian\r\n ");
        break;
    
    default:
        printf(" ERROR\r\n");
        break;
    }
    //版本
    printf(" %-33s: %s\r\n","Version","1(current)");
    //OS/ABI
    printf(" %-33s: %s\r\n","OS/ABI","UNIX - System V");
    //ABI Version
    printf(" %-33s: %s\r\n","ABI Version","0");
    //跳过魔数以及一些其他信息
    efile+=num;
    //Type
    printf(" %-33s; %s\r\n","Type");
    switch (*(u_int16_t *)(efile))
    {
    case 0:
        printf(" No file type\r\n");
        break;
    case 1:
        printf(" Relocatable file\r\n");
        break;
    case 2:
    printf(" Executable file\r\n");
        break;
    case 3:
    printf(" Shared object file\r\n");
    break;
    case 4:
    printf(" Core file\r\n");
    default:
        printf("ERROR\r\n");
        break;
    }
    efile+=sizeof(u_int16_t);
    //Machine
    printf(" %-33s:","Machine");
    switch (*(u_int16_t*)efile)
    {
    case EM_386:
        printf(" Intel 80386\r\n");
        break;
    case EM_ARM:
        printf(" ARM\r\n");
        break;
    case EM_X86_64:
        printf(" AMD X86-64 arrchitecture\r\n");
        break;
    
    default:
        printf("ERROR\r\n");
        break;
    }
    efile+=sizeof(u_int16_t);
    //Version
    printf(" %-33s: %s\r\n","version","0X1");
    efile+=sizeof(u_int32_t);
    //入口点位置
    printf(" %-33s: 0X1x\r\n","Entry point address",*(u_int64_t*)(efile));
    efile+=sizeof(u_int64_t);
    //程序头起始地址
    printf(" %-33s: %1u (bytes into file)\r\n","Start of program headers",*(u_int64_t*)efile);
    efile+=sizeof(u_int64_t);
    //节头表起始地址
        printf(" %-33s: %1u (bytes into file)\r\n","Start of section headers",*(u_int64_t*)efile);
        efile+=sizeof(u_int64_t);
    //Flags
    printf(" %-33s: 0X0\r\n","Flags");
    efile+=sizeof(u_int32_t);
    //elf文件头的大小
    printf(" %-33s: %d (bytes)\r\n","Size of this header",*(u_int16_t*)efile);
    efile+=sizeof(u_int16_t);
    //每个程序头表项的大小
        printf(" %-33s: %d (bytes)\r\n","Size of Program headers",*(u_int16_t*)efile);
    efile+=sizeof(u_int16_t);
    //程序头表的表项目数量
    
        printf(" %-33s: %d (bytes)\r\n","number of the program headers",*(u_int16_t*)efile);
    efile+=sizeof(u_int16_t);
    //每个节头表项的大小
        printf(" %-33s: %d (bytes)\r\n","Size of the section headers",*(u_int16_t*)efile);
    efile+=sizeof(u_int16_t);
    //节头表项数量
        printf(" %-33s: %d (bytes)\r\n","number of section headers",*(u_int16_t*)efile);
    efile+=sizeof(u_int16_t);
    //下标
        printf(" %-33s: %d (bytes)\r\n","section header string table index",*(u_int16_t*)efile);
        


}
int main()
{

}