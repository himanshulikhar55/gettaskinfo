#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include "lib_gettaskinfo.h"

struct task_info *lib_gettaskinfo(){
    int pid;
    long rv;
    char *buff;
    struct task_info *task;

    task = NULL;
    buff = (char *)malloc(sizeof(char) * MAX_BUFF_SIZE);
    pid = getpid();

    rv = syscall(__NR_gettaskinfo);
    printf("rv: %ld\n", rv);
    if(rv == 1){
        printf("System call was successfull :)\nCheck dmesg\n");
    } else {
        printf("Something ducked up :(\n");
    }

    return task;
}