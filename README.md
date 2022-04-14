# xv6plus
xv6 + better memory management

# xv6 book in chinese
http://xv6.dgs.zone/

# TODO:
1. display memory infomation (similar to vmstat) OK

   shows: free memory, processes, page table

2. page table lab ok
  https://pdos.csail.mit.edu/6.828/2021/labs/pgtbl.html

  a) 内核与进程之间添加了一个共享区域，加速一些 syscall

  b) 检测最近访问过的页表 LRU 的时候用的辅助函数

------------------PAGE FAULT------------------------

3. copy on writing
https://pdos.csail.mit.edu/6.828/2021/labs/cow.html

4. lazy page allocation ok
https://pdos.csail.mit.edu/6.828/2020/labs/lazy.html

4.1. zero fill on demand
4.2. demand paging

5. mmap
https://pdos.csail.mit.edu/6.828/2021/labs/mmap.html

-------------------------------------------------------

(optional) 6. swapping (2Q replacement)
https://github.com/anandthegreat/xv7

7. multicore memory allocator 
https://pdos.csail.mit.edu/6.828/2021/labs/lock.html

---------------------MAYBE MORE------------------------
Super Pages

Shared Memory (not very hard)


# 6.s081 in chinese
https://mit-public-courses-cn-translatio.gitbook.io/mit6-s081/

使用git：https://www.runoob.com/git/git-remote-repo.html
