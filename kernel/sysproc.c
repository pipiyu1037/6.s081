#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

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
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL

#define MAXPG 64

int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 va;
  int pageNum;
  uint64 pBuf;

  pagetable_t topPagetable = myproc()->pagetable;
  uint64 KBuf = 0;

  if(argaddr(0, &va) < 0)
    return -1;

  if(va >= MAXVA)
    panic("pgaccess");

  if(argint(1, &pageNum) < 0)
    return -1;

  if(pageNum > MAXPG)
    panic("pgaccess");

  if(argaddr(2, &pBuf) < 0)
    return -1;
  
  for(int i = 0; i < pageNum; i++){
    pagetable_t pagetable = topPagetable;
    int pteValid = 1;
    for(int level = 2; level > 0; level--){
      pte_t pte = pagetable[PX(level, va)];
      if(pte & PTE_V){
        pagetable = (pagetable_t)PTE2PA(pte);
      }else{
        pteValid = 0;
        break;
      }
    }
    if(!pteValid) break;

    if(pagetable[PX(0, va) + i] & PTE_A){
      printf("%d ", i);
      KBuf |= (1L << i);
    }
    pagetable[PX(0, va) + i] &= ~PTE_A;
  }
  printf("\n");
  vmprint(topPagetable, 0);

  printf("%p\n", KBuf);
  if(copyout(topPagetable, pBuf, (char*)&KBuf, sizeof(KBuf)) < 0)
    return -1;
  return 0;
}
#endif

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
