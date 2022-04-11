#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/sysinfo.h"

void
main(void){
    struct sysinfo s;
    if(sysinfo(&s) < 0) exit(-1);

    printf("free memory: %d(bytes)\nprocs number: %d\n",s.freemem,s.nproc);

    exit(0);
}