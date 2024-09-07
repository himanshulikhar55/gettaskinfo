#include <stdint.h>

#define MAX_BUFF_SIZE 35
#define __NR_gettaskinfo 449

struct task_info{
    char *name;
    long int state;
    char *start_time;
    int normal_prio;
};

struct task_info *lib_gettaskinfo();