#include <stdio.h>
#include "lib_gettaskinfo.h"
// #include <linux/syscalls.h>
#include <stdarg.h>
#include <stdio.h>

int vout(char *string, char *fmt, ...){
    int n;
    va_list arg_ptr;

    va_start(arg_ptr, fmt);
    n = vsnprintf(string, 8, fmt, arg_ptr);
    va_end(arg_ptr);
    return n;
}
int main(){
    struct task_info *task = lib_gettaskinfo();
    printf("Syscall was successfull. Some information about this process:\n");
    printf("Name: %s\n", task->name);
    printf("State: %ld\n", task->state);
    printf("Start time: %s\n", task->start_time);
    printf("Normal priority: %d\n", task->normal_prio);
    return 1;
}
