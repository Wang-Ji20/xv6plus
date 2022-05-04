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

## Appendix： 中期报告

一、已选题目调研

内存管理是操作系统的核心机制之一。通过一层地址空间的抽象，操作系统赋予每个进程以单独的内存空间的假象，隔离开了每一个进程，使之不会互相干扰。除此以外，还能实现很多有意思的机制。

在xv6中，我们（xv6的设计者是这么称呼的，所以我们也这么称呼）采用术语“虚拟内存”代表每个进程都有自己虚拟的地址空间，不指代交换和替换机制。

参考通行操作系统的机制，我们设计了以下的目标。



二、预期功能目标

已实现：

- 系统调用：vmstat 显示内存状况
- 共享内存：内核与页表共享
- 页错误：懒惰页分配(lazy page allocation)
- 页错误：写时复制 fork (COW fork)

要实现：

- 系统调用：mmap 内存映射
- 页错误：覆盖交换(swapping)机制（另一种说法，虚拟内存）
- 页错误：需求分页(demand paging)机制
- 空闲空间分配：伙伴分配法(buddy allocator)



三、技术框架

覆盖交换:

交换谁？ 2Q Replacement - Linux Kernel的方法，两个队列，交换不常使用的

怎么换？mmap 特殊区域

高低水位 一次换一个cluster

需求分页：

改造 exec 系统调用