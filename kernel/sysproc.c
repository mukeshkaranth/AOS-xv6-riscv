#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// print a hello statement that salutes the user upon syscall.
uint64
sys_salutation(void)
{
  int n;
  argint(0, &n);
  salutation(n);
  printf("number of running processes: %d\n", procCount());
  procdump();
  return 0;
}

// Utility function to return number of process, 
// number of system calls made or number of free
// memory pages present in the system.
uint64
sys_sysinfo(void)
{
  int n;
  argint(0, &n);
  switch (n)
  {
    case 0:
      return procCount();
      break;
    case 1:
      return sysCallCounts;
      break;
    case 2:
      return memkFreePages();
      break;
    
    default:
      printf("Error: parameter index out of bounds. Please provide a value from 0 to 2.\n");
      exit(-1);
      break;
  }
  return 0;
}

// Function to return information about the process
// Parent process ID, System calls made by the process
// and size of the process in pages.
uint64
sys_procinfo(void)
{
  uint64 n;
  struct proc * process = myproc();
  argaddr(0, &n);
  int extraPage = process->sz % PGSIZE ? 1 : 0;
  int nPages = process->sz/4096 + extraPage;
  copyout(process->pagetable, n, (char *)&(process->parent->pid), sizeof(int));
  copyout(process->pagetable, n+4, (char *)&(process->sysCallCount), sizeof(int));
  copyout(process->pagetable, n+8, (char *)&(nPages), sizeof(int));
  return 0;
}

uint64
sys_clone(void) {
  uint64 stack;
  argaddr(0, &stack);
  return clone((void *)stack);
}
