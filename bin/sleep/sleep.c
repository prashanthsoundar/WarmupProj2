
#include <stdlib.h>


void* syscall(void* syscallNum,void* param1,void* param2,void* param3,void* param4,void* param5,void* param6) {
    void* returnValue;
    __asm__ volatile ("movq %1, %%rax; movq %2, %%rdi; movq %3, %%rsi; movq %4, %%rdx; movq %5, %%r10; movq %6, %%r8;movq %7, %%r9;syscall;movq %%rax, %0;":"=g"(returnValue):"g"(syscallNum),"g"(param1),"g"(param2),"g"(param3),"g"(param4),"g"(param5),"g"(param6):"%rax","%rdi","%rsi","%rdx","%r10","%r8","%r9","memory");//clobber list
    return returnValue;
}

int main(int argc, char *argv[], char *envp[]){
    
    int sleep_seconds=0;
    if(argc==1){
        return 0;
    }
    else if (argc>1){
        sleep_seconds=argv[1];
    }
    syscall((void*)50,(void*)0,(void*)0,(void*)0,(void*)0,(void*)0,(void*)0);
    
    while((uint64_t)syscall((void*)51,(void*)0,(void*)0,(void*)0,(void*)0,(void*)0,(void*)0)<=sleep_seconds);
    
    syscall((void*)52,(void*)0,(void*)0,(void*)0,(void*)0,(void*)0,(void*)0);
    
    return 0;
}
