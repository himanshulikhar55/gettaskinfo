#include <stdio.h>
#include "lib_gettaskinfo.h"

int main(){
    struct task_info *task = lib_gettaskinfo();
    if(!task){
        printf("lib_gettaskinfo failed\n");
        return 0;
    }
    printf("Syscall was successfull. Some information about this process:\n");
    printf("Name: %s\n", task->name);
    printf("State: %ld\n", task->state);
    printf("Start time: %s\n", task->start_time);
    printf("Normal priority: %d\n", task->normal_prio);
    return 1;
}
