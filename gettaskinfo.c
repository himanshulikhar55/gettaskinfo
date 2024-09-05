#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/sched.h>

#define TASK_COMM_LEN 16

void print_to_buffer(char *string, char *fmt, ...)
{
   va_list arg_ptr;

   va_start(arg_ptr, fmt);
   vsnprintf(string, 8, fmt, arg_ptr);
   va_end(arg_ptr);
}

SYSCALL_DEFINE2(gettaskinfo, int, pid, char *, buffer){
    struct task_struct *task;
    int len;
    unsigned long n;
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
//    n = copy_to_user(buffer, task->comm, TASK_COMM_LEN+2);
    if(n){
        printk(KERN_ALERT "Could not copy task->comm\n");
        return -EFAULT;
    }
    printk(KERN_ALERT "Proc name: %s\n", task->comm);

    // maximum value of __state is 0x0008 but since this process will be running, it will be 0x0000
    print_to_buffer(kern_buff, "%d,", task->__state);
    n = copy_to_user(buffer+len, kern_buff, 5);
    // if(n){
    //     printk(KERN_ALERT "Could not copy task->__state\n");
    //     return -EFAULT;
    // }
    len += 5;
    printk(KERN_ALERT "Proc state: %u\n", task->__state);

    print_to_buffer(kern_buff, "%llu,", task->start_time);
    n = copy_to_user(buffer+len, kern_buff, 21);
    len += 25;
    // if(n){
    //     printk(KERN_ALERT "Could not copy task->start_time\n");
    //     return -EFAULT;
    // }
    printk(KERN_ALERT "Proc time: %llu\n", task->start_time);

    print_to_buffer(kern_buff, "%d,", task->normal_prio);
    n = copy_to_user(buffer+len, kern_buff, 4);
    // if(n){
    //     printk(KERN_ALERT "Could not copy task->normal_prio\n");
    //     return -EFAULT;
    // }
    printk(KERN_ALERT "Proc normal prio: %d\n", task->normal_prio);

    return 1;
}