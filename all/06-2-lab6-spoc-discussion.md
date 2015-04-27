# lab5 spoc 思考题

- 有"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的ucore_code和os_exercises的git repo上。


## 个人思考题

### 总体介绍

 - ucore中的调度点在哪里，完成了啥事？
 - 进程控制块中与调度相关的字段有哪些？
 - ucore的就绪队列数据结构在哪定义？在哪进行修改？
 - ucore的等待队列数据结构在哪定义？在哪进行修改？

### 调度算法支撑框架

 - 调度算法支撑框架中的各个函数指针的功能是啥？会被谁在何种情况下调用？
 - 调度函数schedule()的调用函数分析，可以了解进程调度的原因。请分析ucore中所有可能的调度位置，并说明可能的调用原因。
 
### 时间片轮转调度算法

 - 时间片轮转调度算法是如何基于调度算法支撑框架实现的？
 - 时钟中断如何调用RR_proc_tick()的？

### stride调度算法

 - stride调度算法的思路？ 
 - stride算法的特征是什么？
 - stride调度算法是如何避免stride溢出问题的？
 - 无符号数的有符号比较会产生什么效果？
 - 什么是斜堆(skew heap)?

## 小组练习与思考题

### (1)(spoc) 理解调度算法支撑框架的执行过程

即在ucore运行过程中通过`cprintf`函数来完整地展现出来多个进程在调度算法和框架的支撑下，在相关调度点如何动态调度和执行的细节。(约全面细致约好)
> 输出如下： 代码链接：https://github.com/swnhieian/os_exercises/tree/master/all/lab6_result  
```
++ setup timer interrupts
begin schedule()
proc 1 dequeue
pick proc 1 to run
proc 1 is running
proc 2 from waiting to runnable
proc 2 enqueue
proc 1 from state 2 -> sleep
begin schedule()
proc 2 dequeue
pick proc 2 to run
proc 2 is running
kernel_execve: pid = 2, name = "priority".
proc 3 from waiting to runnable
proc 3 enqueue
proc 4 from waiting to runnable
proc 4 enqueue
proc 5 from waiting to runnable
proc 5 enqueue
proc 6 from waiting to runnable
proc 6 enqueue
proc 7 from waiting to runnable
proc 7 enqueue
main: fork ok,now need to wait pids.
proc 2 from state 2 -> sleep
begin schedule()
proc 7 dequeue
pick proc 7 to run
proc 7 is running
child pid 7, acc 672000, time 1003
proc 7 state 2 -> zombie
proc 2 from waiting to runnable
proc 2 enqueue
begin schedule()
proc 6 dequeue
pick proc 6 to run
proc 6 is running
child pid 6, acc 4000, time 1011
proc 6 state 2 -> zombie
begin schedule()
proc 5 dequeue
pick proc 5 to run
proc 5 is running
child pid 5, acc 4000, time 1018
proc 5 state 2 -> zombie
begin schedule()
proc 4 dequeue
pick proc 4 to run
proc 4 is running
child pid 4, acc 4000, time 1025
proc 4 state 2 -> zombie
begin schedule()
proc 3 dequeue
pick proc 3 to run
proc 3 is running
child pid 3, acc 4000, time 1032
proc 3 state 2 -> zombie
begin schedule()
proc 2 dequeue
pick proc 2 to run
proc 2 is running
main: pid 3, acc 4000, time 1033
main: pid 4, acc 4000, time 1033
main: pid 5, acc 4000, time 1033
main: pid 6, acc 4000, time 1033
main: pid 7, acc 672000, time 1033
main: wait pids over
stride sched correct result: 1 1 1 1 168
proc 2 state 2 -> zombie
proc 1 from waiting to runnable
proc 1 enqueue
begin schedule()
proc 1 dequeue
pick proc 1 to run
proc 1 is running
begin schedule()
proc 1 enqueue
proc 1 dequeue
all user-mode processes have quit.
```

请完成如下练习，完成代码填写，并形成spoc练习报告
> 需写练习报告和简单编码，完成后放到git server 对应的git repo中

### 练习用的[lab6 spoc exercise project source code](https://github.com/chyyuu/ucore_lab/tree/master/labcodes_answer/lab6_result)


