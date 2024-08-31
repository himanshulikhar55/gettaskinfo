#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/sched.h>

SYSCALL_DEFINE2(gettaskinfo, int, pid, char *, buffer){
    struct task_struct *task;

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

    printk(KERN_ALERT "Proc name: %s\n", task->comm);
    printk(KERN_ALERT "Proc state: %u\n", task->__state);
    printk(KERN_ALERT "Proc name: %llu\n", ((ktime_get_ns())-(task->start_time))/1000000000);
    printk(KERN_ALERT "Proc name: %d\n", task->normal_prio);

    return 1;
}