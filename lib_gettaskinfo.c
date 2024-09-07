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
    // for(int i = 0; i < 30; i++){
    //     printf("%c-(%d)", buff[i], i);
    //     printf("%c", buff[i]);
    // }
    // printf("\n");
    // printf("The complete string: %s\n", buff);

    token = strtok(buff, ",");
    // printf("Proc name: %s\n", token);
    task->name = token;

    token = strtok(NULL, ",");
    // printf("Proc state: %s\n", token);
    task->state = strtol(token, NULL, 10);

    token = strtok(NULL, ",");
    // printf("Proc start time: %s\n", token);
    task->start_time = token;

    token = strtok(NULL, ",");
    // printf("Proc normal priority: %s\n", token);
    task->normal_prio = strtol(token, NULL, 10);

    return task;
}
