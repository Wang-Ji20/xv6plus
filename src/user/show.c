#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"
#include "kernel/sysinfo.h"

void
main(void){
    struct sysinfo s;

    printf("\n\n the initial state !! --------------------\n");
    if(sysinfo(&s) < 0) exit(-1);


    char* p = sbrk(20480);
    printf("\n\n sbrk !! --------------------\n");
    if(sysinfo(&s) < 0) exit(-1);


    *(p + 10000) = 1;
    printf("\n\n visit after sbrk !! --------------------\n");
    if(sysinfo(&s) < 0) exit(-1);


    int pid = fork();
    if(pid == 0){
        printf("\n\n fork !! --------------------\n");
        if(sysinfo(&s) < 0) exit(-1);


        *(p + 10000) = 1;
        printf("\n\n visit after fork !! --------------------\n");
        if(sysinfo(&s) < 0) exit(-1);


        printf("\n\n test for stack !! --------------------\n");
        char a[6000];
        printf(a);
        printf(" Succeed !!\n");
    }

    exit(0);
}