#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<malloc.h>
 
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

//关于main()函数的参数解释，参考了这篇博客https://www.cnblogs.com/liuzhenbo/p/11044404.html
//main函数的实参是在操作系统中命令行输入的，argc是输入参数的个数（包括文件名），argv这个字符指针数组，每个都代表参数对应的字符串
int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        printf("您输入的指令有误，请重新输入!\r\n");
        help();
        return 0;
    }
    //打开指定文件
    int fd=open(argv[2],O_RDONLY,0);
    //如果打开文件失败，就直接返回，因为后面是需要将文件的内存完整复制到内存的
    if(fd==-1)
    {
        printf("打开文件失败\n");
        return 0;
    }
    //分配文件
    long int end =lseek(fd,0,SEEK_END);
    long int begin=lseek(fd,0,SEEK_SET);
    char* efile=malloc(end);
    if(!efile)
    {
        printf("文件分配内存失败");
        return 0;
    }
    memset(efile,0,end);
    if (-1==read(fd,efile,end))
    {
        perror("文件读取失败");
        return 0;
    }
    if(!strcmp(agrv[1],"-h"))
    {
        readfileheader(efile);
    }
}