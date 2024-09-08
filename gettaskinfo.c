#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/sched.h>

#define MAX_ARG_SIZE 30
#define MAX_PROC_NAME_SIZE 25
#define MAX_STATE_SIZE 1 // userspace process always in running state, i.e, 0x0000
#define MAX_START_TIME_SIZE 21
#define MAX_PRIORITY_SIZE 3

int print_to_buffer(char *string, char *fmt, ...){
    int n;
    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    /*
     * Actual string will be stored from string[0] to string[MAX_ARG_SIZE-2]
     * string[MAX_ARG_SIZE-1] will be '\0' if arg size is >= MAX_ARG_SIZE
     * 
     */
    n = vsnprintf(string, MAX_ARG_SIZE, fmt, arg_ptr);
    printk(KERN_ALERT "The string is: %s. n=%d\n", string, n);
    va_end(arg_ptr);
    return n;
}

/*
 * This system call was implemented for kernel 5.10.233.
 *
 * NOTE: A compilation error can be encountered if implemented for a newer kernel.
 * Ex:   task_struct.state is renamed to task_struct.__state as of kernel 5.15.165
 *       and the datatype of task_state.__state is changed to u64 from long.
 * 
 * Please make changes accordingly when compiling for a different kernel.
 * 
 * Use: This system call returns the process name, state, start time (human-readable format)
 *      and normal priority of the the process that invokes this system call.
 *      This information is stored in a buffer passed by the user.
 * 
 * Return value: -ESRCH if PID is invalid
 *               -EFAULT if buffer is invalid
 *               1 if successful
 */

SYSCALL_DEFINE2(gettaskinfo, int, pid, char *, buf){
    struct task_struct *task;
    struct tm result;
    struct timespec64 start_time, boot_time, real_start_time, real_time;
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

    /* get the task_struct of the process */
    task = pid_task(find_vpid(pid), PIDTYPE_PID);

    if(!task){
        printk(KERN_ALERT "Could not get task_struct\n");
        return -ESRCH;
    }

    n = 0;
    len = 0;

    n = print_to_buffer(kern_buff, "%s,", task->comm);
    m = copy_to_user(buffer+len, kern_buff, MAX_PROC_NAME_SIZE+1);
    len += n;
    printk(KERN_ALERT "Proc name: %s\n", kern_buff);

    /* -1 unrunnable, 0 runnable, >0 stopped */
    n = print_to_buffer(kern_buff, "%ld,", task->state);
    m = copy_to_user(buffer+len, kern_buff, MAX_STATE_SIZE+1);
    len += n;
    printk(KERN_ALERT "Proc state: %ld\n", task->state);

    /* Convert task start_time to seconds and nanoseconds */
    start_time.tv_sec  = task->start_time / 1000000000ULL;
    start_time.tv_nsec = task->start_time % 1000000000ULL;

    ktime_get_boottime_ts64(&boot_time);
    ktime_get_real_ts64(&real_time);

    /* This gives the real boot time of the system */
    real_start_time = timespec64_sub(real_time, boot_time);

    /*
     * To that we add the the start time of the process, which is the time
     * elapsed since boot time (in nanoseconds).
     */
    real_start_time = timespec64_add(real_start_time, start_time);

    /* Convert real start time to human-readable format */
    time64_to_tm(real_start_time.tv_sec, 0, &result);

    /* Adjust time to IST */
    result.tm_hour += 5;
    result.tm_min += 30;

    /* Handle minute and hour overflow */
    if (result.tm_min >= 60) {
        result.tm_min -= 60;
        result.tm_hour += 1;
    }
    if (result.tm_hour >= 24) {
        result.tm_hour -= 24;
        result.tm_mday += 1;
    }

    n = print_to_buffer(kern_buff, "%04ld-%02d-%02d %02d:%02d:%02d,",
                        result.tm_year + 1900, result.tm_mon + 1,
                        result.tm_mday, result.tm_hour,
                        result.tm_min, result.tm_sec);

    m = copy_to_user(buffer + len, kern_buff, MAX_START_TIME_SIZE + 1);
    len += n;
    printk(KERN_ALERT "Proc start time in ns: %llu\n", task->start_time);
    printk(KERN_ALERT "Proc start time (human-readable): %s\n", kern_buff);

    n = print_to_buffer(kern_buff, "%d,", task->normal_prio);
    m = copy_to_user(buffer+len, kern_buff, MAX_PRIORITY_SIZE+1);
    len += n;
    printk(KERN_ALERT "Proc normal prio: %d\n", task->normal_prio);

    return 1;
}
