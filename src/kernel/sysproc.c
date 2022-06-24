#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"

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
  if(n < 0 && (growproc(n) < 0)) return -1;
  if(n > 0) myproc()->sz += n;
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

extern int freeMemory(void);
extern int numproc(void);
extern void vmprint(pagetable_t pgt, int depth);

uint64
sys_sysinfo(void){
  struct sysinfo s;
  s.freemem = freeMemory();
  s.nproc = numproc();

  uint64 pg;
  
  if(argaddr(0, &pg) < 0)
    return -1;

  if(copyout(myproc()->pagetable, pg, (char *) &s, sizeof(s)) < 0)
    return -1;

  vmprint(myproc()->pagetable, 0);
  return 0;

}

extern pte_t * walk(pagetable_t pagetable, uint64 va, int alloc);

uint64
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 start_address = 0;
  int pages_to_check = 0;
  uint64 buffer = 0;
  uint64 bitmask = 0;

  if(argaddr(0, &start_address) < 0 || argint(1, &pages_to_check) < 0 ||
  argaddr(2, &buffer) < 0)
    return -1;
  
  for (int i = 0; i < pages_to_check && i < 64; i++)
  {
    uint64 virtualAddr = start_address + i * PGSIZE;
    pte_t* tpte = walk(myproc()->pagetable, virtualAddr, 0);
    if(tpte == 0 || (((*tpte) & (PTE_V | PTE_U)) == 0))
      return -1;
    if(*tpte & PTE_A){
      *tpte = *tpte & ~PTE_A;
      bitmask |= (1 << i);
    }
  }
  
  if(copyout(myproc()->pagetable, buffer, (char*)&bitmask, sizeof(bitmask)) < 0)
    return -1;

  return 0;
}