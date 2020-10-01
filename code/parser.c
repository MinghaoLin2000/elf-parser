#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<malloc.h>
#include<elf.h>
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
void sectionReader(char* efile)
{
    //得到节区的数量
    int nNum=(Elf64_Ehdr *)(efile)->e_shnum;
    //get shstrndex
    Elf64_Ehdr* efilehead=(Elf64_Ehdr*)efile;
    Elf64_Half eshstrndx=efilehead->e_shstrndx;
    //得到偏移地址
    Elf64_Shdr* psecheader=(Elf64_Shdr*)(efile+efilehead->e_shoff);
    Elf64_Shdr* pshstr=(Elf64_Shdr*)(psecheader+eshstrndx);
    //字符串表
    char* pshstrbuff=(char*)(efile+pshstr->sh_offset);
     printf("共有 %d 节区表, 偏移位置开始于 0x%lx:\r\n\r\n",
           nNum, *(Elf64_Off*)(efile+ 40));
    printf("节头:\r\n");  //打印标志位信息
    printf("  [Nr] %-16s  %-16s  %-16s  %-16s\r\n", "Name", "Type", "Address", "Offset");
    printf("       %-16s  %-16s  %-5s  %-5s  %-5s  %-5s\r\n", "Size", "EntSize", "Flags", "Link", "Info", "Align");
    //遍历每一个节表
    for(int i=0;i<nNumSec;i++)
    {
        printf(" [%2d] %-16s ",i,(char*)(psecheader[i].sh_name+pshstrbuff));
        //Type
        switch (psecheader[i].sh_type)
        {
        case SHT_NULL:
                printf("%-16s  ", "NULL");break;
            case SHT_PROGBITS:
                printf("%-16s  ", "PROGBITS");break;
            case SHT_SYMTAB:
                printf("%-16s  ", "SYMTAB");break;
            case SHT_STRTAB:
                printf("%-16s  ", "STRTAB");break;
            case SHT_RELA:
                printf("%-16s  ", "RELA");break;
            case SHT_HASH:
                printf("%-16s  ", "GNU_HASH");break;
            case SHT_DYNAMIC:
                printf("%-16s  ", "DYNAMIC");break;
            case SHT_NOTE:
                printf("%-16s  ", "NOTE");break;
            case SHT_NOBITS:
                printf("%-16s  ", "NOBITS");break;
            case SHT_REL:
                printf("%-16s  ", "REL");break;
            case SHT_SHLIB:
                printf("%-16s  ", "SHLIB");break;
            case SHT_DYNSYM:
                printf("%-16s  ", "DYNSYM");break;
            case SHT_INIT_ARRAY:
                printf("%-16s  ", "INIT_ARRY");break;
            case SHT_FINI_ARRAY:
                printf("%-16s  ", "FINI_ARRY");break;
            case SHT_PREINIT_ARRAY:
                printf("%-16s  ", "PREINIT_ARRAY");break;
            case SHT_GNU_HASH:
                printf("%-16s  ", "GNU_HASH");break;
            case SHT_GNU_ATTRIBUTES:
                printf("%-16s  ", "GNU_ATTRIBUTES");break;
            case SHT_GNU_LIBLIST:
                printf("%-16s  ", "GNU_LIBLIST");break;
            case SHT_GNU_verdef:
                printf("%-16s  ", "GNU_verdef");break;
            case SHT_GNU_verneed:
                printf("%-16s  ", "GNU_verneed");break;
            case SHT_GNU_versym:
                printf("%-16s  ", "GNU_versym");break;
            default:
                printf("%-16s  ", "NONE");break;
        }
        //对应节对应文件开始的偏移以及载入内存中的虚拟地址
        printf("%0161X %081X\r\n",psecheader[i].sh_addr,psecheader[i].sh_offset);
        //对应节所占的文件大小
        printf("      %0161X %0161X  ",psecheader[i].sh_size,psecheader[i].sh_entsize);
         switch (psecheader[i].sh_flags) {
                case 0:
                    printf("%3s    %4u  %4u  %4lu\r\n",
                           "", psecheader[i].sh_link, psecheader[i].sh_info, psecheader[i].sh_addralign);
                    break;
                case 1:
                    printf("%3s    %4u  %4u  %4lu\r\n",
                           "W", psecheader[i].sh_link, psecheader[i].sh_info, psecheader[i].sh_addralign);
                    break;
                case 2:
                    printf("%3s    %4u  %4u  %4lu\r\n",
                           "A", psecheader[i].sh_link, psecheader[i].sh_info, psecheader[i].sh_addralign);
                    break;
                case 4:
                    printf("%3s    %4u  %4u  %4lu\r\n",
                           "X", psecheader[i].sh_link, psecheader[i].sh_info, psecheader[i].sh_addralign);
                    break;
                case 3:
                    printf("%3s    %4u  %4u  %4lu\r\n",
                           "WA", psecheader[i].sh_link, psecheader[i].sh_info, psecheader[i].sh_addralign);
                    break;
                case 5://WX
                    printf("%3s    %4u  %4u  %4lu\r\n",
                           "WX", psecheader[i].sh_link, psecheader[i].sh_info, psecheader[i].sh_addralign);
                    break;
                case 6://AX
                    printf("%3s    %4u  %4u  %4lu\r\n",
                           "AX", psecheader[i].sh_link, psecheader[i].sh_info, psecheader[i].sh_addralign);
                    break;
                case 7://WAX
                    printf("%3s    %4u  %4u  %4lu\r\n",
                           "WAX", psecheader[i].sh_link, psecheader[i].sh_info, psecheader[i].sh_addralign);
                    break;
                case SHF_MASKPROC://MS
                    printf("%3s    %4u  %4u  %4lu\r\n",
                           "MS", psecheader[i].sh_link, psecheader[i].sh_info, psecheader[i].sh_addralign);
                    break;
                default:
                    printf("NONE\r\n");
                    break;
            }

        }
        printf("Key to Flags:\r\n");
    };
}
//打印符号表
void readsymtable(const char* efile)
{
    //从节区定位到偏移
    Elf64_Ehdr* efilehead=(Elf64_Ehdr*)(efile);
    Elf64_Half eshstrndx=efilehead->e_shstrndx;
    Elf64_Shdr* psecheader=(Elf64_Shdr*)(efile+efilehead->e_shoff);
    Elf64_Shdr* pshstr=(Elf64_Shdr*)(psecheader+eshstrndx);
    char* pshstrbuff=(char*)(efile+pshstr->sh_offset);
    for(int i=0;i<efilehead->e_shnum;++i)
    {
        if(!strcmp(psecheader[i].sh_name+pshstrbuff,".dynsym")||!strcmp(psecheader[i]).sh_name+pshstrbuff,".symtab")
        {
            Elf64_Sym* psym=(Elf64_Sym*)(efile+psecheader[i].sh_offset);
            int ncount=psecheader[i].sh_size/psecheader[i].sh_entsize;
            char* pbuffstr=(char*)((psecheader+psecheader[i].sh_link)->sh_offset+efile);
            printf("Symbol table '%s' contains %d entries:\r\n",psecheader[i].sh_name+pshstrbuff,ncount);
            outputsyminfo(psym,pbuffset,ncount);
            continue;
        }
    }
}
//程序头表
void readProgramhead(const char* efile)
{
    //得到程序头表的起始地址
    Elf64_Ehdr* filehead=(Elf64_Ehdr*)efile;
    Elf64_Phdr* pproheader=(Elf64_phdr*)(efile+filehead->e_phoff);
    //通过先获得节头表，然后知道字符串节的位置，以便后续知道字符串
    　Elf64_Shdr* psecheader=(Elf64_Shdr*)(efile+filehead->e_shoff);
    Elf64_Shdr* pshstr=(Elf64_Shdr*)(psecheader+filehead->e_shstrndx);
    char* pstrbuff=(char*)(efile+pshstr->sh_offset);
    printf("Elf 文件类型是");
     switch(pfilehead->e_type)
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
            break;
        default:
            printf(" ERROR\r\n");
            break;
    }
    printf("入口点位置 0X%0lX\r\n", pfilehead->e_entry);
    printf("共有 %d 程序头, 偏移位置 %lu\r\n\r\n", pfilehead->e_phnum, pfilehead->e_phoff);
    printf("Program Headers:\r\n");
    printf("  %-14s  %-16s  %-16s  %-16s\r\n", "Type", "Offset", "VirtAddr", "PhysAddr");
    printf("  %-14s  %-16s  %-16s  %-6s  %-6s\r\n", "", "FileSiz", "MemSiz", "Flags", "Align");
    for(int i=0;i<pfilehead->e_phnum;++i)
    {
        //type
        switch(pproheader[i].p_type)
        {
            case PT_NULL:
                printf("  %-14s  %016lX  %016lX  %016lX\r\n  %-14s  %016lX  %016lX  ", "", pproheader[i].p_offset, pproheader[i].p_vaddr,
                       pproheader[i].p_paddr, "", pproheader[i].p_filesz, pproheader[i].p_memsz);break;
            case PT_LOAD:
                printf("  %-14s  %016lX  %016lX  %016lX\r\n  %-14s  %016lX  %016lX  ", "LOAD", pproheader[i].p_offset, pproheader[i].p_vaddr,
                       pproheader[i].p_paddr, "", pproheader[i].p_filesz, pproheader[i].p_memsz);break;
            case PT_DYNAMIC:
                printf("  %-14s  %016lX  %016lX  %016lX\r\n  %-14s  %016lX  %016lX  ", "DYNAMIC", pproheader[i].p_offset, pproheader[i].p_vaddr,
                       pproheader[i].p_paddr, "", pproheader[i].p_filesz, pproheader[i].p_memsz);break;
            case PT_INTERP:
                printf("  %-14s  %016lX  %016lX  %016lX\r\n  %-14s  %016lX  %016lX  ", "INTERP", pproheader[i].p_offset, pproheader[i].p_vaddr,
                       pproheader[i].p_paddr, "", pproheader[i].p_filesz, pproheader[i].p_memsz);break;
            case PT_NOTE:
                printf("  %-14s  %016lX  %016lX  %016lX\r\n  %-14s  %016lX  %016lX  ", "NOTE", pproheader[i].p_offset, pproheader[i].p_vaddr,
                       pproheader[i].p_paddr, "", pproheader[i].p_filesz, pproheader[i].p_memsz);break;
            case PT_SHLIB:
                printf("  %-14s  %016lX  %016lX  %016lX\r\n  %-14s  %016lX  %016lX  ", "SHLIB", pproheader[i].p_offset, pproheader[i].p_vaddr,
                       pproheader[i].p_paddr, "", pproheader[i].p_filesz, pproheader[i].p_memsz);break;
            case PT_PHDR:
                printf("  %-14s  %016lX  %016lX  %016lX\r\n  %-14s  %016lX  %016lX  ", "PHDR", pproheader[i].p_offset, pproheader[i].p_vaddr,
                       pproheader[i].p_paddr, "", pproheader[i].p_filesz, pproheader[i].p_memsz);break;
            case PT_TLS:
                printf("  %-14s  %016lX  %016lX  %016lX\r\n  %-14s  %016lX  %016lX  ", "TLS", pproheader[i].p_offset, pproheader[i].p_vaddr,
                       pproheader[i].p_paddr, "", pproheader[i].p_filesz, pproheader[i].p_memsz);break;
            case PT_NUM:
                printf("  %-14s  %016lX  %016lX  %016lX\r\n  %-14s  %016lX  %016lX  ", "NUM", pproheader[i].p_offset, pproheader[i].p_vaddr,
                       pproheader[i].p_paddr, "", pproheader[i].p_filesz, pproheader[i].p_memsz);break;
            case PT_GNU_EH_FRAME:
                printf("  %-14s  %016lX  %016lX  %016lX\r\n  %-14s  %016lX  %016lX  ", "GNU_EH_FRAME", pproheader[i].p_offset, pproheader[i].p_vaddr,
                       pproheader[i].p_paddr, "", pproheader[i].p_filesz, pproheader[i].p_memsz);break;
            case PT_GNU_RELRO:
                printf("  %-14s  %016lX  %016lX  %016lX\r\n  %-14s  %016lX  %016lX  ", "GNU_RELRO", pproheader[i].p_offset, pproheader[i].p_vaddr,
                       pproheader[i].p_paddr, "", pproheader[i].p_filesz, pproheader[i].p_memsz);break;
            case PT_GNU_STACK:
                printf("  %-14s  %016lX  %016lX  %016lX\r\n  %-14s  %016lX  %016lX  ", "GNU_STACK", pproheader[i].p_offset, pproheader[i].p_vaddr,
                       pproheader[i].p_paddr, "", pproheader[i].p_filesz, pproheader[i].p_memsz);break;
            default:
                break;
        }
        //
        switch(pproheader[i].p_flags)
        {
            case PF_X:
                printf("%-6s  %-lX\r\n", "  E", pproheader[i].p_align);break;
            case PF_W:
                printf("%-6s  %-lX\r\n", " W ", pproheader[i].p_align);break;
            case PF_R:
                printf("%-6s  %-lX\r\n", "R  ", pproheader[i].p_align);break;
            case PF_X|PF_W:
                printf("%-6s  %-lX\r\n", " WE", pproheader[i].p_align);break;
            case PF_X|PF_R:
                printf("%-6s  %-lX\r\n", "R E", pproheader[i].p_align);break;
            case PF_W|PF_R:
                printf("%-6s  %-lX\r\n", "RW ", pproheader[i].p_align);break;
            case PF_X|PF_R|PF_W:
                printf("%-6s  %-lX\r\n", "RWE", pproheader[i].p_align);break;
            default:
                printf("\r\n");
                break;
        }
        if(PT_INTERP == pproheader[i].p_type)
            printf("      [Requesting program interpreter: %s]\r\n", (char*)(pbuff + pproheader[i].p_offset));
    }
    printf("\r\n Section to Segment mapping:\r\n");
    printf("  段节...\r\n");
    for(int i=0;i<pfilehead->e_phnum;++i)
    {
        printf("   %-7d", i);
        for(int n = 0;n<pfilehead->e_shnum;++n)
        {
            Elf64_Off temp = psecheader[n].sh_addr + psecheader[n].sh_size;
            if((psecheader[n].sh_addr>pproheader[i].p_vaddr && psecheader[n].sh_addr<pproheader[i].p_vaddr + pproheader[i].p_memsz)  ||
                    (temp > pproheader[i].p_vaddr && temp<=pproheader[i].p_vaddr + pproheader[i].p_memsz))
            {
                printf("%s ", (char*)(psecheader[n].sh_name + pstrbuff));
            }
        }
        printf("\r\n");
    }
}
//关于main()函数的参数解释，参考了这篇博客https://www.cnblogs.com/liuzhenbo/p/11044404.html
//main函数的实参是在操作系统中命令行输入的，argc是输入参数的个数（包括运行的文件名），argv这个字符指针数组，每个都代表参数对应的字符串
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
    }else if(!strcmp(argv[1],"-S"))
    {
        sectionReader(efile);
    }else if(!strcmp(argv[1],"-s"))
    {
        readsymtable(efile);
    }
}