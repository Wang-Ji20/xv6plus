#include "mmap.h"
#include "memlayout.h"
#include "elf.h"
#include "riscv.h"
#include "defs.h"
#include "proc.h"
#define MAP_FAILED 0xffffffffffffffff

void *
mmap(void *addr, uint length, int prot, int flags, struct file* file, uint offset)
{
    // find free space 
    // p->size shall be a good place
    struct proc* p = myproc();
    if(!file->writable && (prot & PROT_WRITE) && (flags & MAP_SHARED))
        return (void *) MAP_FAILED;

    // add this address to VMA
    for(int i = 0; i < 16; i++){
        if(p->vmas[i].used == 1) continue;
        p->vmas[i].used = 1;
        p->vmas[i].address = PGROUNDUP(p->sz);
        p->vmas[i].size = length;
        p->vmas[i].permissions = prot;
        p->vmas[i].the_file_being_mapped = filedup(file);
        p->vmas[i].endaddr = PGROUNDUP(p->sz + length);
        p->vmas[i].flags = flags;
        p->sz += PGROUNDUP(length);
        return (void *)p->vmas[i].address;
    }

    return (void *) MAP_FAILED;
}

int
filewrite_offset(struct file *f, uint64 addr, int n, int offset) {
    int r, ret = 0;
    if(f->writable == 0)
        return -1;
    if(f->type != FD_INODE) {
        panic("filewrite: only FINODE implemented!");
    }

    int max = ((MAXOPBLOCKS-1-1-2) / 2) * BSIZE;
    int i = 0;
    while(i < n) {
        int n1 = n - i;
        if(n1 > max)
            n1 = max;

        begin_op();
        ilock(f->ip);
        if ((r = writei(f->ip, 1, addr + i, offset, n1)) > 0)
            offset += r;
        iunlock(f->ip);
        end_op();

        if(r != n1) {
            break;
        }
        i += r;
    }
    ret = (i == n ? n : -1);
    return ret;
}

int 
munmap(void *addr, uint length){
    uint64 addri = (uint64) addr;
    struct proc* p = myproc();
    for (int i = 0; i < 16; i++)
    {
        struct VMA* v = &(p->vmas[i]);
        if(v->used == 1 && addri < v->endaddr && addri >= v->address){
            int toclose = 0;
            int offset = v->offset;
            addri = PGROUNDDOWN(addri);
            length = PGROUNDUP(length);

            if (addri == v->address)
            {
                if (length == v->size)
                {
                    v->size = 0;
                    toclose = 1;
                }
                else{
                    v->address += length;
                    v->size -= length;
                    v->offset += length;
                }
            }
            else
            {
                v->size -= length;
            }
            if (v->flags & MAP_SHARED)
            {
                filewrite_offset(v->the_file_being_mapped, (uint64)addr, length, offset);
            }
            if(walkaddr(p->pagetable, addri) != 0)
                uvmunmap(p->pagetable, addri, length/PGSIZE, 1);
            if(toclose)
                fileclose(v->the_file_being_mapped);
        }
    }
    

    return 0;
}

int lazy_mapping(struct proc* p, uint64 addr){
    for (int i = 0; i < 16; i++)
    {
        struct VMA* v = &(p->vmas[i]);
        if (v->used == 1 && addr >= v->address && addr < v->endaddr)
        {
            uint64 start = PGROUNDDOWN(addr);
            uint64 offset = start - v->address + v->offset;

            char* mem = kalloc();
            if(!mem) return -1;
            memset(mem, 0, PGSIZE);
            
            if(mappages(p->pagetable, start, PGSIZE, 
            (uint64)mem, (v->permissions << 1)|PTE_U) != 0){
                kfree(mem);
                return -1;
            }

            ilock(v->the_file_being_mapped->ip);
            readi(v->the_file_being_mapped->ip, 1, start, offset, PGSIZE);
            iunlock(v->the_file_being_mapped->ip);
            return 0;
        }
        
    }
    //not found
    return 5;
}