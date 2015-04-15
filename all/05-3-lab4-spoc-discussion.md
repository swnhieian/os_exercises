# lab4 spoc 思考题

- 有"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的ucore_code和os_exercises的git repo上。

## 个人思考题

### 总体介绍

(1) ucore的线程控制块数据结构是什么？

> proc_struct

### 关键数据结构

(2) 如何知道ucore的两个线程同在一个进程？

> 比较两个proc_struct的parent值和CR3值

(3) context和trapframe分别在什么时候用到？

> context 切换进程
> trapframe fork中copy_thread时用到,kernel_thread创建内核进程

(4) 用户态或内核态下的中断处理有什么区别？在trapframe中有什么体现？

> 用户态需要切换堆栈，内核态不需要
> trapframe 的 cs 寄存器的值

### 执行流程

(5) do_fork中的内核线程执行的第一条指令是什么？它是如何过渡到内核线程对应的函数的？
```
tf.tf_eip = (uint32_t) kernel_thread_entry;
/kern-ucore/arch/i386/init/entry.S
/kern/process/entry.S
```

(6)内核线程的堆栈初始化在哪？
```
tf和context中的esp
```

(7)fork()父子进程的返回值是不同的。这在源代码中的体现中哪？

(8)内核线程initproc的第一次执行流程是什么样的？能跟踪出来吗？

## 小组练习与思考题

(1)(spoc) 理解内核线程的生命周期。

> 需写练习报告和简单编码，完成后放到git server 对应的git repo中

### 掌握知识点
1. 内核线程的启动、运行、就绪、等待、退出
2. 内核线程的管理与简单调度
3. 内核线程的切换过程

### 练习用的[lab4 spoc exercise project source code](https://github.com/chyyuu/ucore_lab/tree/master/related_info/lab4/lab4-spoc-discuss)


请完成如下练习，完成代码填写，并形成spoc练习报告

### 1. 分析并描述创建分配进程的过程

> 注意 state、pid、cr3，context，trapframe的含义
``` 
    //给进程控制块分配一块内存空间
    struct proc_struct *proc = kmalloc(sizeof(struct proc_struct));
    //如果分配成功
    if (proc != NULL) {
        //初始化
        memset(proc, 0, sizeof(struct proc_struct));
        //设置状态为uninitialized
        proc->state = PROC_UNINIT;
        //pid为非法
        proc->pid = -1;
        //cr3暂时无用
        proc->cr3 = boot_cr3;
    }
    return proc;
```

### 练习2：分析并描述新创建的内核线程是如何分配资源的

> 注意 理解对kstack, trapframe, context等的初始化
```
kernel_thread(int (*fn)(void *), void *arg, uint32_t clone_flags) {
    struct trapframe tf;
    memset(&tf, 0, sizeof(struct trapframe));
    //标明该进程为内核态进程
    tf.tf_cs = KERNEL_CS;
    tf.tf_ds = tf.tf_es = tf.tf_ss = KERNEL_DS;
    tf.tf_regs.reg_ebx = (uint32_t)fn;
    tf.tf_regs.reg_edx = (uint32_t)arg;
    //设置eip的值，在do_fork里跳转到kernel_thread_entry
    tf.tf_eip = (uint32_t)kernel_thread_entry;
    return do_fork(clone_flags | CLONE_VM, 0, &tf);
}
do_fork主要内容：
    //给PCB分配空间
    proc = alloc_proc();
    //分配一个pid
    proc->pid = get_pid();
    //为该进程分配内核堆栈空间
    setup_kstack(proc);
    //把context和trapframe复制到PCB中
    copy_thread(proc, stack, tf);
    //加到队列里
    list_add_before(&proc_list, &proc->list_link);
    //使进程就绪
    wakeup_proc(proc);
    //进程计数加1
    nr_process++;
    ret = proc->pid;
    //设置当前进程为新进程的父进程
    proc->parent=current;
```

当前进程中唯一，操作系统的整个生命周期不唯一，在get_pid中会循环使用pid，耗尽会等待

### 练习3：阅读代码，在现有基础上再增加一个内核线程，并通过增加cprintf函数到ucore代码中
能够把进程的生命周期和调度动态执行过程完整地展现出来

> 见 https://github.com/swnhieian/os_exercises/tree/master/all/lab4-spoc-discuss


### 练习4 （非必须，有空就做）：增加可以睡眠的内核线程，睡眠的条件和唤醒的条件可自行设计，并给出测试用例，并在spoc练习报告中给出设计实现说明

### 扩展练习1: 进一步裁剪本练习中的代码，比如去掉页表的管理，只保留段机制，中断，内核线程切换，print功能。看看代码规模会小到什么程度。


