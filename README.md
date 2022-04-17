# xv6plus
xv6 + better memory management

# xv6 book in chinese
http://xv6.dgs.zone/

# TODO:
1. display memory infomation (similar to vmstat) OK

   shows: free memory, processes, page table

2. page table lab OK
    https://pdos.csail.mit.edu/6.828/2021/labs/pgtbl.html

  a) 内核与进程之间添加了一个共享区域，加速一些 syscall。

  b) 检测最近访问过的页表 LRU 的时候用的辅助函数

------------------PAGE FAULT------------------------

3. copy on writing OK
https://pdos.csail.mit.edu/6.828/2021/labs/cow.html
4. lazy page allocation OK
https://pdos.csail.mit.edu/6.828/2020/labs/lazy.html

我觉得没有任何问题，但是 usertests 有两个点过不了。同一个问题：子进程因为内存分配过大，被杀掉，释放内存数量不够。很奇怪：在父进程等待子进程的 wait 前面加上一个printf，一切就好了。不知道为什么。大部分情况下没有问题。

3. zero fill on demand
4. demand paging
5. mmap
   https://pdos.csail.mit.edu/6.828/2021/labs/mmap.html

-------------------------------------------------------

(optional) 8. swapping (2Q replacement)
https://github.com/anandthegreat/xv7

9. multicore memory allocator 
   https://pdos.csail.mit.edu/6.828/2021/labs/lock.html

---------------------MAYBE MORE------------------------

10. Super Pages

11. Shared Memory (not very hard)
12. buddy allocator 6.828的lab


# 6.s081 in chinese
https://mit-public-courses-cn-translatio.gitbook.io/mit6-s081/

使用git：https://www.runoob.com/git/git-remote-repo.html
