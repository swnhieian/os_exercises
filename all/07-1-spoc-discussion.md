# 同步互斥(lec 17) spoc 思考题


- 有"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的ucore_code和os_exercises的git repo上。

## 个人思考题

### 背景
 - 请给出程序正确性的定义或解释。
 - 在一个新运行环境中程序行为与原来的预期不一致，是错误吗？
 - 程序并发执行有什么好处和障碍？
 - 什么是原子操作？

### 现实生活中的同步问题

 - 家庭采购中的同步问题与操作系统中进程同步有什么区别？
 - 如何通过枚举和分类方法检查同步算法的正确性？
 - 尝试描述方案四的正确性。
 - 互斥、死锁和饥饿的定义是什么？

### 临界区和禁用硬件中断同步方法

 - 什么是临界区？
 - 临界区的访问规则是什么？
 - 禁用中断是如何实现对临界区的访问控制的？有什么优缺点？

### 基于软件的同步方法

 - 尝试通过枚举和分类方法检查Peterson算法的正确性。
 - 尝试准确描述Eisenberg同步算法，并通过枚举和分类方法检查其正确性。

### 高级抽象的同步方法

 - 如何证明TS指令和交换指令的等价性？
 - 为什么硬件原子操作指令能简化同步算法的实现？
 
## 小组思考题

1. （spoc）阅读[简化x86计算机模拟器的使用说明](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab7/lab7-spoc-exercise.md)，理解基于简化x86计算机的汇编代码。

2. （spoc)了解race condition. 进入[race-condition代码目录](https://github.com/chyyuu/ucore_lab/tree/master/related_info/lab7/race-condition)。

 - 执行 `./x86.py -p loop.s -t 1 -i 100 -R dx`， 请问`dx`的值是什么？
 
 > dx未赋值，通过-c可以看到dx初始为0。运行结果如下：
 
	    dx          Thread 0         
	    0   
	   -1   1000 sub  $1,%dx
	   -1   1001 test $0,%dx
	   -1   1002 jgte .top
	   -1   1003 halt
 
 - 执行 `./x86.py -p loop.s -t 2 -i 100 -a dx=3,dx=3 -R dx` ， 请问`dx`的值是什么？

> 有两个线程，中断频率较高，因此线程0运行完后运行线程1，dx初始为3，各执行4次循环，运行结果符合预期，如下：

	   dx          Thread 0                Thread 1         
	    3   
	    2   1000 sub  $1,%dx
	    2   1001 test $0,%dx
	    2   1002 jgte .top
	    1   1000 sub  $1,%dx
	    1   1001 test $0,%dx
	    1   1002 jgte .top
	    0   1000 sub  $1,%dx
	    0   1001 test $0,%dx
	    0   1002 jgte .top
	   -1   1000 sub  $1,%dx
	   -1   1001 test $0,%dx
	   -1   1002 jgte .top
	   -1   1003 halt
	    3   ----- Halt;Switch -----  ----- Halt;Switch -----  
	    2                            1000 sub  $1,%dx
	    2                            1001 test $0,%dx
	    2                            1002 jgte .top
	    1                            1000 sub  $1,%dx
	    1                            1001 test $0,%dx
	    1                            1002 jgte .top
	    0                            1000 sub  $1,%dx
	    0                            1001 test $0,%dx
	    0                            1002 jgte .top
	   -1                            1000 sub  $1,%dx
	   -1                            1001 test $0,%dx
	   -1                            1002 jgte .top
	   -1                            1003 halt


 - 执行 `./x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx`， 请问`dx`的值是什么？
 
 > 和之前指令不同在于中断频率较高，所以两个线程交替执行，但是因为线程切换时上下文保存了寄存器，所以即使频繁切换，运行结果依然正确：
 
	    dx          Thread 0                Thread 1         
	    3   
	    2   1000 sub  $1,%dx
	    3   ------ Interrupt ------  ------ Interrupt ------  
	    2                            1000 sub  $1,%dx
	    2   ------ Interrupt ------  ------ Interrupt ------  
	    2   1001 test $0,%dx
	    2   1002 jgte .top
	    1   1000 sub  $1,%dx
	    2   ------ Interrupt ------  ------ Interrupt ------  
	    2                            1001 test $0,%dx
	    2                            1002 jgte .top
	    1   ------ Interrupt ------  ------ Interrupt ------  
	    1   1001 test $0,%dx
	    1   1002 jgte .top
	    2   ------ Interrupt ------  ------ Interrupt ------  
	    1                            1000 sub  $1,%dx
	    1   ------ Interrupt ------  ------ Interrupt ------  
	    0   1000 sub  $1,%dx
	    0   1001 test $0,%dx
	    1   ------ Interrupt ------  ------ Interrupt ------  
	    1                            1001 test $0,%dx
	    1                            1002 jgte .top
	    0   ------ Interrupt ------  ------ Interrupt ------  
	    0   1002 jgte .top
	   -1   1000 sub  $1,%dx
	   -1   1001 test $0,%dx
	    1   ------ Interrupt ------  ------ Interrupt ------  
	    0                            1000 sub  $1,%dx
	    0                            1001 test $0,%dx
	    0                            1002 jgte .top
	   -1   ------ Interrupt ------  ------ Interrupt ------  
	   -1   1002 jgte .top
	   -1   1003 halt
	    0   ----- Halt;Switch -----  ----- Halt;Switch -----  
	    0   ------ Interrupt ------  ------ Interrupt ------  
	   -1                            1000 sub  $1,%dx
	   -1                            1001 test $0,%dx
	   -1                            1002 jgte .top
	   -1   ------ Interrupt ------  ------ Interrupt ------  
	   -1                            1003 halt

 
 
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 1 -M 2000`, 请问变量x的值是什么？
 
> 一个线程，循环1次，读取2000地址处值，将其加1，存回地址2000，最终2000地址处值为1，即变量x是1，符合预期。
 

		 2000          Thread 0         
		    0   
		    0   1000 mov 2000, %ax
		    0   1001 add $1, %ax
		    1   1002 mov %ax, 2000
		    1   1003 sub  $1, %bx
		    1   1004 test $0, %bx
		    1   1005 jgt .top
		    1   1006 halt

 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -a bx=3 -M 2000`, 请问变量x的值是什么？为何每个线程要循环3次？
 
> 变量x值被两个线程的循环分别加了3次，最终结果是6。两个线程循环3次是因为用变量bx来执行循环，两个线程的bx初值都是3，线程切换不会影响各个线程bx的值，所以每个线程循环了3次。
 
		  2000          Thread 0                Thread 1         
		    0   
		    0   1000 mov 2000, %ax
		    0   1001 add $1, %ax
		    1   1002 mov %ax, 2000
		    1   1003 sub  $1, %bx
		    1   1004 test $0, %bx
		    1   1005 jgt .top
		    1   1000 mov 2000, %ax
		    1   1001 add $1, %ax
		    2   1002 mov %ax, 2000
		    2   1003 sub  $1, %bx
		    2   1004 test $0, %bx
		    2   1005 jgt .top
		    2   1000 mov 2000, %ax
		    2   1001 add $1, %ax
		    3   1002 mov %ax, 2000
		    3   1003 sub  $1, %bx
		    3   1004 test $0, %bx
		    3   1005 jgt .top
		    3   1006 halt
		    3   ----- Halt;Switch -----  ----- Halt;Switch -----  
		    3                            1000 mov 2000, %ax
		    3                            1001 add $1, %ax
		    4                            1002 mov %ax, 2000
		    4                            1003 sub  $1, %bx
		    4                            1004 test $0, %bx
		    4                            1005 jgt .top
		    4                            1000 mov 2000, %ax
		    4                            1001 add $1, %ax
		    5                            1002 mov %ax, 2000
		    5                            1003 sub  $1, %bx
		    5                            1004 test $0, %bx
		    5                            1005 jgt .top
		    5                            1000 mov 2000, %ax
		    5                            1001 add $1, %ax
		    6                            1002 mov %ax, 2000
		    6                            1003 sub  $1, %bx
		    6                            1004 test $0, %bx
		    6                            1005 jgt .top
		    6                            1006 halt
 
 
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 0`， 请问变量x的值是什么？
 
> 变量x的值最终为1，循环变量初始为0，所以两个线程各执行循环1次，但是因为中断频率较高，Thread1在Thread0线程写回增加后的x值之前就读取了x的值（即0），导致最终x的结果是1而非理想的2。
 
		  2000          Thread 0                Thread 1         
		    0   
		    0   1000 mov 2000, %ax
		    0   1001 add $1, %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0                            1000 mov 2000, %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    1   1002 mov %ax, 2000
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1                            1001 add $1, %ax
		    1                            1002 mov %ax, 2000
		    1                            1003 sub  $1, %bx
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1   1003 sub  $1, %bx
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1                            1004 test $0, %bx
		    1                            1005 jgt .top
		    1                            1006 halt
		    1   ----- Halt;Switch -----  ----- Halt;Switch -----  
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1   1004 test $0, %bx
		    1   1005 jgt .top
		    1   1006 halt
 
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 1`， 请问变量x的值是什么？
 
 > 与之前不同在于随机种子变了，即中断出现的地方变了，这一次并没有出现之前的情况，Thread1在Thread0完成对x写回后才读取x，并没有出错。
 
		  2000          Thread 0                Thread 1         
		    0   
		    0   1000 mov 2000, %ax
		    0   1001 add $1, %ax
		    1   1002 mov %ax, 2000
		    1   1003 sub  $1, %bx
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1                            1000 mov 2000, %ax
		    1                            1001 add $1, %ax
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1   1004 test $0, %bx
		    1   1005 jgt .top
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    2                            1002 mov %ax, 2000
		    2                            1003 sub  $1, %bx
		    2                            1004 test $0, %bx
		    2                            1005 jgt .top
		    2   ------ Interrupt ------  ------ Interrupt ------  
		    2   1006 halt
		    2   ----- Halt;Switch -----  ----- Halt;Switch -----  
		    2   ------ Interrupt ------  ------ Interrupt ------  
		    2                            1006 halt

 
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 2`， 请问变量x的值是什么？ 
 
 > 变量x的值是1，情况与前前一条指令相同。
 
		  2000          Thread 0                Thread 1         
		    0   
		    0   1000 mov 2000, %ax
		    0   1001 add $1, %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0                            1000 mov 2000, %ax
		    0                            1001 add $1, %ax
		    1                            1002 mov %ax, 2000
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1   1002 mov %ax, 2000
		    1   1003 sub  $1, %bx
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1                            1003 sub  $1, %bx
		    1                            1004 test $0, %bx
		    1                            1005 jgt .top
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1   1004 test $0, %bx
		    1   1005 jgt .top
		    1   1006 halt
		    1   ----- Halt;Switch -----  ----- Halt;Switch -----  
		    1                            1006 halt

 
 - 变量x的内存地址为2000, `./x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1`， 请问变量x的值是什么？ 
 
> 中断频率更高，x的值依然是1，Thread1读取到的值是Thread0还未完成增加操作的值，即为0。
 
		  2000          Thread 0                Thread 1         
		    0   
		    0   1000 mov 2000, %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0                            1000 mov 2000, %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0   1001 add $1, %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0                            1001 add $1, %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    1   1002 mov %ax, 2000
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1                            1002 mov %ax, 2000
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1   1003 sub  $1, %bx
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1                            1003 sub  $1, %bx
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1   1004 test $0, %bx
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1                            1004 test $0, %bx
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1   1005 jgt .top
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1                            1005 jgt .top
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1   1006 halt
		    1   ----- Halt;Switch -----  ----- Halt;Switch -----  
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1                            1006 halt


3. （spoc） 了解software-based lock, hardware-based lock, [software-hardware-lock代码目录](https://github.com/chyyuu/ucore_lab/tree/master/related_info/lab7/software-hardware-locks)

  - 理解flag.s,peterson.s,test-and-set.s,ticket.s,test-and-test-and-set.s 请通过x86.py分析这些代码是否实现了锁机制？请给出你的实验过程和结论说明。能否设计新的硬件原子操作指令Compare-And-Swap,Fetch-And-Add？
  
#####flag.s
> 未实现，测试命令：`./x86.py -p flag.s  -t 2 -M count -i 1 -c`，运行结果如下，若实现锁机制，count应该为2.


		count          Thread 0                Thread 1         
		    0   
		    0   1000 mov  flag, %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0                            1000 mov  flag, %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0   1001 test $0, %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0                            1001 test $0, %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0   1002 jne  .acquire
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0                            1002 jne  .acquire
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0   1003 mov  $1, flag
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0                            1003 mov  $1, flag
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0   1004 mov  count, %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0                            1004 mov  count, %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0   1005 add  $1, %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0                            1005 add  $1, %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    1   1006 mov  %ax, count
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1                            1006 mov  %ax, count
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1   1007 mov  $0, flag
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1                            1007 mov  $0, flag
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1   1008 sub  $1, %bx
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1                            1008 sub  $1, %bx
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1   1009 test $0, %bx
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1                            1009 test $0, %bx
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1   1010 jgt .top
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1                            1010 jgt .top
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1   1011 halt
		    1   ----- Halt;Switch -----  ----- Halt;Switch -----  
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1                            1011 halt

#####peterson.s
> 未实现，测试命令同上，结果如下：

		count          Thread 0                Thread 1         
		    0   
		    0   1000 lea flag, %fx
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0                            1000 lea flag, %fx
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0   1001 mov %bx, %cx
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0                            1001 mov %bx, %cx
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0   1002 neg %cx
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0                            1002 neg %cx
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0   1003 add $1, %cx
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0                            1003 add $1, %cx
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0   1004 mov $1, 0(%fx,%bx,4)
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0                            1004 mov $1, 0(%fx,%bx,4)
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0   1005 mov %cx, turn
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0                            1005 mov %cx, turn
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0   1006 mov 0(%fx,%cx,4), %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0                            1006 mov 0(%fx,%cx,4), %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0   1007 test $1, %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0                            1007 test $1, %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0   1008 jne .fini
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0                            1008 jne .fini
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0   1012 mov count, %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0                            1012 mov count, %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0   1013 add $1, %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    0                            1013 add $1, %ax
		    0   ------ Interrupt ------  ------ Interrupt ------  
		    1   1014 mov %ax, count
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1                            1014 mov %ax, count
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1   1015 mov $0, 0(%fx,%bx,4)
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1                            1015 mov $0, 0(%fx,%bx,4)
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1   1016 mov %cx, turn
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1                            1016 mov %cx, turn
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1   1017 halt
		    1   ----- Halt;Switch -----  ----- Halt;Switch -----  
		    1   ------ Interrupt ------  ------ Interrupt ------  
		    1                            1017 halt
		
#####test-and-set.s

> 实现了锁机制，因为xchg指令将加锁操作做成了原子操作，用高频率的中断可以检验结果均正确。

#####ticket.s

> 实现了锁机制，通过fetchadd将获取允许进入临界区的线程做成了原子操作，通过之前的测试命令，用高频率的中断可以检验结果均正确。

#####test-and-test-and-set.s

> 实现了锁机制，因为xchg指令将加锁操作做成了原子操作，通过之前的测试命令，用高频率的中断可以检验结果均正确。  
  
  
```
Compare-And-Swap

int CompareAndSwap(int *ptr, int expected, int new) {
  int actual = *ptr;
  if (actual == expected)
    *ptr = new;
  return actual;
}
```

```
Fetch-And-Add

int FetchAndAdd(int *ptr) {
  int old = *ptr;
  *ptr = old + 1;
  return old;
}
```
