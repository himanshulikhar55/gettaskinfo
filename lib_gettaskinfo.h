#include <stdint.h>

#define MAX_BUFF_SIZE 1024
#define __NR_gettaskinfo 449

struct task_info{
    long int state;
    uint64_t start_time;
    int normal_prio;
};

struct task_info *lib_gettaskinfo();