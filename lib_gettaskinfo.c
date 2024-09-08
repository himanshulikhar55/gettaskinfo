#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <errno.h>
#include "lib_gettaskinfo.h"
#include <string.h>

struct task_info *lib_gettaskinfo(){
    int pid;
    long rv;
    char *buff, *token;
    struct task_info *task;

    task = NULL;
    buff = (char *)malloc(sizeof(char) * MAX_BUFF_SIZE);
    if(!buff){
        fprintf(stderr, "malloc failed\n");
        return task;
    }
    for(int i = 0; i < MAX_BUFF_SIZE; i++){
        buff[i] = '\0';
    }

    task = (struct task_info *)malloc(sizeof(struct task_info));
    
    pid = getpid();
    rv = syscall(__NR_gettaskinfo, pid, buff);
    
    if(rv == 1){
        printf("System call was successfull :)\nCheck dmesg if you want\n");
    } else {
        fprintf(stderr, "gettaskinfo failed, errno = %d\n", errno);
        printf("Something ducked up :(\n");
    }

    token = strtok(buff, ",");
    task->name = token;

    token = strtok(NULL, ",");
    task->state = strtol(token, NULL, 10);

    token = strtok(NULL, ",");
    task->start_time = token;

    token = strtok(NULL, ",");
    task->normal_prio = strtol(token, NULL, 10);

    return task;
}
