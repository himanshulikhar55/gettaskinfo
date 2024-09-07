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

    // Convert task start_time to seconds and nanoseconds
    start_time.tv_sec = task->start_time / 1000000000ULL;  // Convert nanoseconds to seconds
    start_time.tv_nsec = task->start_time % 1000000000ULL; // Remaining nanoseconds

    // Subtract start_time (from boot) from real_time to get actual start time
    ktime_get_boottime_ts64(&boot_time);
    ktime_get_real_ts64(&real_time);

    real_start_time = timespec64_sub(real_time, boot_time);
    real_start_time = timespec64_add(real_start_time, start_time);

    // Debugging: Print the real start time
    printk(KERN_ALERT "Real start time (seconds since epoch): %lld\n", real_start_time.tv_sec);

    // Convert real start time to human-readable format
    time64_to_tm(real_start_time.tv_sec, 0, &result);
    // Adjust time to IST (UTC + 5:30)
    result.tm_hour += 5;
    result.tm_min += 30;

    // Handle minute and hour overflow
    if (result.tm_min >= 60) {
        result.tm_min -= 60;
        result.tm_hour += 1;
    }
    if (result.tm_hour >= 24) {
        result.tm_hour -= 24;
        result.tm_mday += 1;
        // You may need to handle the end of month/year here if required.
    }
    // Format the time as a string
    n = print_to_buffer(kern_buff, "%04ld-%02d-%02d %02d:%02d:%02d,",
                        result.tm_year + 1900, result.tm_mon + 1,
                        result.tm_mday, result.tm_hour,
                        result.tm_min, result.tm_sec);

    m = copy_to_user(buffer + len, kern_buff, MAX_START_TIME_SIZE + 1);
    len += n;

    printk(KERN_ALERT "Proc start time (human-readable): %s\n", kern_buff);

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
