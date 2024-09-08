#include <stdint.h>

#define MAX_BUFF_SIZE 35
#define __NR_gettaskinfo 441 /* sys_gettaskinfo number */

struct task_info{
    char *name; /* process name */
    long int state; /* process state */
    char *start_time; /* process start time */
    int normal_prio; /* process normal priority */
};

struct task_info *lib_gettaskinfo();