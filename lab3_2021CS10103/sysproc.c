#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

/* System Call Definitions */
int 
sys_set_sched_policy(void)
{
    // Implement your code here 
    int policy;
    if(argint(0, &policy) < 0){
        return -22;
    }
    if(policy != 0 && policy != 1) return -22;
    myproc()->policy = policy;

    return 0;
}

int 
sys_get_sched_policy(void)
{
    // Implement your code here 
    return myproc()->policy;
}
