#include<sys/syscall.h>
#include<sys/defs.h>
#include<sys/virtualMemory.h>
#include<sys/kprintf.h>
#include<sys/tarfs.h>

#include<sys/task.h>

#define MSR_EFER 0xC0000080
#define MSR_STAR 0xC0000081
#define MSR_LSTAR 0xC0000082
#define MSR_SFMASK 0xC0000084

uint64_t* userRSP = 0;
uint64_t* userRIP = 0;
uint64_t* kernelRSP = 0;
uint64_t userRax = 0;
uint64_t userRbx = 0;
uint64_t userRcx = 0;
uint64_t userRdx = 0;
uint64_t userRsi = 0;
uint64_t userRdi = 0;
uint64_t userRbp = 0;
uint64_t userRflags = 0;

uint64_t readMSR(uint32_t msrAddr)
{
    uint32_t msrLow, msrHigh;
    __asm__ __volatile__ ("rdmsr" :"=a"(msrLow),"=d"(msrHigh):"c"(msrAddr));
    return (uint64_t)msrHigh<<32|(uint64_t)msrLow;
}

void* syscallHandler(uint64_t paramA,uint64_t paramB,uint64_t paramC,uint64_t paramD,uint64_t paramE,uint64_t paramF,uint64_t syscallNum) {
    
    kprintf("\n%d %d %d %d %d %d %d",syscallNum,paramA,paramB,paramC,paramD,paramE,paramF);
//    char* p =  (char*)paramB;
//    kprintf("%s",p);
//
    
    switch(syscallNum)
    {
        case 0: kprintf("Kill me System Call\n"); exit();
        case 1: kprintf("Fork System Call\n"); return (void*)fork();
        case 99: kprintf("Malloc System Call\n"); return (void*)malloc(paramA);
        case 0: //kprintf("Read File System Call\n"); 
                return (void *)readFile(paramA,(char *)paramB,paramC);
        case 4: //kprintf("Write File System Call\n");
                return (void *)writeFile(paramA,(char *)paramB,paramC);  
        case 3: kprintf("Close File System Call\n");return (void*)closeFile(paramA);
        case 2: kprintf("Open File System Call");return (void*)openFile((char *)paramA);
        case 9: kprintf("Open Dir System Call");return (void*)openDirectory((char *)paramA);
        case 10: kprintf("Close Dir System Call");return (void*)closeDir(paramA);
        case 11: kprintf("Read Dir System Call");return (void*)readDir(paramA,(char *)paramB,paramC);
        case 12: kprintf("Get Dents System Call");return (void*)getDirEntries(paramA,(char *)paramB,paramC);  
        case 80: kprintf("Change Directory System Call");return (void*)changeDirectory((char *)paramA);
        case 79: kprintf("Get Current Working Directory System Call");return (void*)getCWD((char *)paramA,paramB);
        case 78: kprintf("Exec System Call\n"); return (void*)exec((char*)paramA,(char*)paramB,(char*)paramC);
        case 88: kprintf("WaitPID System Call\n"); return (void*)waitpid((void*)paramA,(uint64_t*)paramB,(void*)paramC);
            case 420: kprintf("print System Call\n"); return (void*)printMe((void*)paramA,(char*)paramB,(char*)paramC);
    }
    
    return (void*)100;
    
}

void writeMSR(uint64_t value,uint32_t msrAddr)
{
    uint32_t msrLow;
    uint32_t msrHigh;
    msrLow=(uint32_t)value;
    msrHigh=(uint32_t)(value>>32);
     __asm__ __volatile__ ("wrmsr"::"a"(msrLow),"d"(msrHigh),"c"(msrAddr));
}

void _syscallEntry();

void initSyscalls()
{
    kernelRSP = (uint64_t*)kmalloc();
    //kprintf("TSS RSP : %p ", kernelRSP);
    //STEP1: set the system call extension bit (SCE bit) to 1;
    uint64_t sce = readMSR(MSR_EFER);
    sce |= (0x1); //enable syscalls
    writeMSR(sce,MSR_EFER);
    
    writeMSR(1<<9,MSR_SFMASK);
    
    writeMSR((uint64_t)0x8<<32|(uint64_t)0x1B<<48,MSR_STAR);
    
    writeMSR((uint64_t)_syscallEntry,MSR_LSTAR);
    
    
    
}
