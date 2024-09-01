#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "syscall.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
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
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
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

uint64
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

  backtrace();
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
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

uint64 sys_sigalarm(void) {
  struct proc *p = myproc();
  int interval;
  uint64 func_addr;
  if (argint(0,&interval) < 0) {
    return -1;
  }
  if (argaddr(1,&func_addr) < 0) {
    return -1;
  }

  //only if the peocess has a timer outstanding, call the alarm function

  //p->handler = p->trapframe->epc;


  //store alarm info in proc
  p->alarm_interval = interval;
  p->handler = (void (*)(void)) func_addr;
  return 0;
}

//make sure that control returns to the instruction at which 
//the user program was originally interrupted by the timer interrupt.
uint64 sys_sigreturn(void) {
  struct proc *p = myproc();
  p->ticks = 0;
  *p->trapframe = *p->interrupt_trapframe;
  p->in_alarm = 0;

  return 0;
}
