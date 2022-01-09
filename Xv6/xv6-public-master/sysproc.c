#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "stat.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

void
sys_shutdown(void)
{
  outw(0xB004, 0x0|0x2000);
  outw(0x604, 0x0|0x2000);
}

int
sys_add(void)
{
  //struct mystat *ct;
  int *nums, size;
  argptr (0 , (void*)&nums ,sizeof(*nums));
  argint(1, &size);
  int s = 0;
  // for(int i = 0; i < ct->sz; i++){
  //   s += ct->nums[i];
  // }
  for(int i = 0; i < size; i++){
    s += nums[i];
  }
  return s;
}

int
sys_incr(void)
{
  int num;
  argint(0,&num);
  return num+1;
}

char*
sys_substr(void)
{
  char* str;
  int start_ind, len;
  argptr(0, (void*)&str, sizeof(*str));
  argint(1, &start_ind);
  argint(2, &len);
  char *s = &str[start_ind];
  s[len] = '\0';
  return s;
}

int
sys_substroccur(void)
{
  char* str1, * str2;
  
  argptr(0, (void*)&str1, sizeof(*str1));
  argptr(1, (void*)&str2, sizeof(*str2));
  int N = strlen(str1);
  int M = strlen(str2);
  int count = 0;
  
   for (int i = 0; i <= N - M; i++)
  {
      int j;
      for (j = 0; j < M; j++){
        if (str1[i+j] != str2[j]){
          break;
        }
      }
          
      if (j == M) 
      {
        count++;
      }
  }
  return count;
}

int
sys_nahian(void)
{
  int student_id = 1705091;
  return student_id;
}

int
sys_salsabil(void)
{
  int a = 5, c = 11, student_id;
  argint(0, &student_id);
  int x = student_id;
  int m = student_id;
  
  for(int i = 0; i < 500; i++){
    x = ((a*x) + c) % m;
  }
  return x;
}