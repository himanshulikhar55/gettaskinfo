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
        buff[i] = 'a';
    }
    buff[MAX_BUFF_SIZE-1] = '\0';
    pid = getpid();
    printf("The complete string: %s\n", buff);
    rv = syscall(__NR_gettaskinfo, pid, buff);
    printf("rv: %ld\n", rv);
    if(rv == 1){
        printf("System call was successfull :)\nCheck dmesg if you want\n");
    } else {
        fprintf(stderr, "gettaskinfo failed, errno = %d\n", errno);
        printf("Something ducked up :(\n");
    }
    for(int i = 0; i < 30; i++){
        // printf("%c-(%d)", buff[i], i);
        printf("%c", buff[i]);
    }
    printf("\n");
    printf("The complete string: %s\n", buff);

    token = strtok(buff, ",");
    printf("Proc name: %s\n", token);

    token = strtok(NULL, ",");
    printf("Proc state: %s\n", token);

    token = strtok(NULL, ",");
    printf("Proc start time: %s\n", token);

    token = strtok(NULL, ",");
    printf("Proc normal priority: %s\n", token);

    return task;
}
