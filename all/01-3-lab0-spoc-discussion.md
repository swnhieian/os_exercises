# lab0 SPOC思考题

## 个人思考题

---

能否读懂ucore中的AT&T格式的X86-32汇编语言？请列出你不理解的汇编语言。
- [x]  

>  http://www.imada.sdu.dk/Courses/DM18/Litteratur/IntelnATT.htm
基本可以读懂常用的汇编命令及语言

虽然学过计算机原理和x86汇编（根据THU-CS的课程设置），但对ucore中涉及的哪些硬件设计或功能细节不够了解？
- [x]  

> 对于如何启动操作系统，用户态、核心态的功能和切换，页替换算法的实现以及一些变量的具体设置细节及实现方法了解都不是很详细。  


哪些困难（请分优先级）会阻碍你自主完成lab实验？
- [x]  

>   
对于原理和实现方法的不了解
使用调试、模拟等工具的不熟练
实验完成的复杂程度
课业繁重时间紧迫

如何把一个在gdb中或执行过程中出现的物理/线性地址与你写的代码源码位置对应起来？
- [x]  

>  使用objdump进行反汇编。 

了解函数调用栈对lab实验有何帮助？
- [x]  

>  
能够更清楚地理解计算机底层对于函数调用的机理，了解计算机中内存的分配，便于更高效地编写操作系统，完成实验。

你希望从lab中学到什么知识？
- [x]  

>  
学到操作系统的基本原理和具体实现方法，掌握高层操作在底层的运行机制，深入了解计算机的运行原理，并和以往学过的其他原理课程知识相验证。

---

## 小组讨论题

---

搭建好实验环境，请描述碰到的困难和解决的过程。
- [x]  

> 在使用虚拟机的时候，遇到了虚拟机无法启动报错的问题，在老师和助教的帮助下，使用了新的虚拟硬盘，重新建立虚拟机后成功启动。

熟悉基本的git命令行操作命令，从github上的[ucore git repo](http://www.github.com/chyyuu/ucore_lab)下载ucore lab实验
- [x] 

> 已经熟悉基本的git命令行命令。

尝试用qemu+gdb（or ECLIPSE-CDT）调试lab1
- [x] 

> 已经完成。

对于如下的代码段，请说明”：“后面的数字是什么含义
```
/* Gate descriptors for interrupts and traps */
struct gatedesc {
    unsigned gd_off_15_0 : 16;        // low 16 bits of offset in segment
    unsigned gd_ss : 16;            // segment selector
    unsigned gd_args : 5;            // # args, 0 for interrupt/trap gates
    unsigned gd_rsv1 : 3;            // reserved(should be zero I guess)
    unsigned gd_type : 4;            // type(STS_{TG,IG32,TG32})
    unsigned gd_s : 1;                // must be 0 (system)
    unsigned gd_dpl : 2;            // descriptor(meaning new) privilege level
    unsigned gd_p : 1;                // Present
    unsigned gd_off_31_16 : 16;        // high bits of offset in segment
};
```

- [x]  

> 
表示定义的变量占其类型（也就是unsigned)中的位数，如16代表占16位，5代表占用5位等。

对于如下的代码段，
```
#define SETGATE(gate, istrap, sel, off, dpl) {            \
    (gate).gd_off_15_0 = (uint32_t)(off) & 0xffff;        \
    (gate).gd_ss = (sel);                                \
    (gate).gd_args = 0;                                    \
    (gate).gd_rsv1 = 0;                                    \
    (gate).gd_type = (istrap) ? STS_TG32 : STS_IG32;    \
    (gate).gd_s = 0;                                    \
    (gate).gd_dpl = (dpl);                                \
    (gate).gd_p = 1;                                    \
    (gate).gd_off_31_16 = (uint32_t)(off) >> 16;        \
}
```

如果在其他代码段中有如下语句，
```
unsigned intr;
intr=8;
SETGATE(intr, 0,1,2,3);
```
请问执行上述指令后， intr的值是多少？

- [x]  

> 65538。[代码](https://github.com/swnhieian/os_exercises/tree/master/all/lab0-spoc-code/value.c "value.c")如lab0-spoc-code/value.c文件.

请分析 [list.h](https://github.com/chyyuu/ucore_lab/blob/master/labcodes/lab2/libs/list.h)内容中大致的含义，并能include这个文件，利用其结构和功能编写一个数据结构链表操作的小C程序
- [x]  

> 
list.h中定义了一个双向链表的通用数据结构，可以进行插入、删除元素，查看链表是否为空等基本操作。<br/>
代码:
[list.h](https://github.com/swnhieian/os_exercises/tree/master/all/lab0-spoc-code/list.h "list.h")  
[test.c](https://github.com/swnhieian/os_exercises/tree/master/all/lab0-spoc-code/test.c "test.c")

---

## 开放思考题

---

是否愿意挑战大实验（大实验内容来源于你的想法或老师列好的题目，需要与老师协商确定，需完成基本lab，但可不参加闭卷考试），如果有，可直接给老师email或课后面谈。
- [x]  

>  

---
