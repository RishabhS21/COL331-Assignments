// system calls
int write(int, const void*, int);
int close(int);
int open(const char*, int);
int exec(char*);
int get_sched_policy(void);
int set_sched_policy(int);
// ulib.c
void printf(int, const char*, ...);