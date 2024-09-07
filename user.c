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
    printf("%lu\n", sizeof(unsigned int));
    struct task_info *task = lib_gettaskinfo();
    // struct task_struct *task = pid_task(find_vpid(getpid()), PIDTYPE_PID);
    // printf("%s\n", task->comm);
    // char string[100];
    // int n = vout(string, "%d,", 200);
    // printf("The string is:  %s. n=%d\n", string, n);
    // n += vout(string+n, "%d,", 400);
    // printf("The string is:  %s. total chars printed=%d\n", string, n);
    // for(int i=0;i<n;i++)
    //     printf("%c-()", string[i]);
    // printf("\n");
    return 1;
}
