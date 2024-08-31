#include <stdio.h>
#include "lib_gettaskinfo.h"
#include <linux/sched.h>
#include <linux/kernel.h>
// #include <linux/syscalls.h>

int main(){
    struct task_info *task = lib_gettaskinfo();
    // struct task_struct *task = pid_task(find_vpid(getpid()), PIDTYPE_PID);
    // printf("%s\n", task->comm);
    return 1;
}