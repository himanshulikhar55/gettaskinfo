#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/sched.h>

#define TASK_COMM_LEN 16
#define MAX_ARG_SIZE 25
#define MAX_PROC_NAME_SIZE 25
#define MAX_STATE_SIZE 1 // userspace process always in running state, i.e, 0x0000
#define MAX_START_TIME_SIZE 21
#define MAX_PRIORITY_SIZE 3

int print_to_buffer(char *string, char *fmt, ...){
    int n;
    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    // actual string will be stored from string[0] to string[MAX_ARG_SIZE-2]
    // string[MAX_ARG_SIZE-1] will be '\0' if arg size is >= MAX_ARG_SIZE
    n = vsnprintf(string, MAX_ARG_SIZE, fmt, arg_ptr);
    printk(KERN_ALERT "The string is: %s. n=%d\n", string, n);
    va_end(arg_ptr);
    return n;
}

SYSCALL_DEFINE2(gettaskinfo, int, pid, char *, buffer){
    struct task_struct *task;
    int len, n;
    unsigned long m;
    char kern_buff[30];

    printk(KERN_ALERT "sys_gettaskinfo being called\n");

    if(pid < 0 || pid > PID_MAX_LIMIT){
        printk(KERN_ALERT "PID invalid\n");
        return -ESRCH;
    }
    if(!buffer){
        printk(KERN_ALERT "Buffer Invalid\n");
        return -EFAULT;
    }
    task = pid_task(find_vpid(pid), PIDTYPE_PID);

    if(!task){
        printk(KERN_ALERT "Could not get task_struct\n");
        return -ESRCH;
    }

    n = 0;
    len = 0;
    // TASK_COMM_LEN for task->comm, 1 for comma and 1 for '\0'
    n = print_to_buffer(kern_buff, "%s,", task->comm);
    m = copy_to_user(buffer+len, kern_buff, MAX_PROC_NAME_SIZE+1);
    len += n;
    // if(n){
    //     printk(KERN_ALERT "Could not copy task->comm\n");
    //     return -EFAULT;
    // }
    printk(KERN_ALERT "Proc name: %s\n", kern_buff);

    // maximum value of __state is 0x0008 but since this process will be running, it will be 0x0000
    n = print_to_buffer(kern_buff, "%u,", task->__state);
    m = copy_to_user(buffer+len, kern_buff, MAX_STATE_SIZE+1);
    len += n;
    // if(n){
    //     printk(KERN_ALERT "Could not copy task->__state\n");
    //     return -EFAULT;
    // }
    printk(KERN_ALERT "Proc state: %u\n", task->__state);

    n = print_to_buffer(kern_buff, "%llu,", task->start_time);
    m = copy_to_user(buffer+len, kern_buff, MAX_START_TIME_SIZE+1);
    len += n;
    // if(n){
    //     printk(KERN_ALERT "Could not copy task->start_time\n");
    //     return -EFAULT;
    // }
    printk(KERN_ALERT "Proc time: %llu\n", task->start_time);

    n = print_to_buffer(kern_buff, "%d,", task->normal_prio);
    m = copy_to_user(buffer+len, kern_buff, MAX_PRIORITY_SIZE+1);
    len += n;
    // if(n){
    //     printk(KERN_ALERT "Could not copy task->normal_prio\n");
    //     return -EFAULT;
    // }
    printk(KERN_ALERT "Proc normal prio: %d\n", task->normal_prio);

    return 1;
}
