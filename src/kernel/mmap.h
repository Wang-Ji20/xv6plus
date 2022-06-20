#pragma once
#include "param.h"
#include "types.h"
#include "fs.h"
#include "fcntl.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "file.h"

void *mmap(void *addr, uint length, int prot, int flags, 
            struct file* file, uint offset);

int munmap(void *addr, uint length);