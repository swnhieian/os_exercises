#lec 3 SPOC Discussion

## 第三讲 启动、中断、异常和系统调用-思考题

## 3.1 BIOS
 1. 比较UEFI和BIOS的区别。
 1. 描述PXE的大致启动流程。

## 3.2 系统启动流程
 1. 了解NTLDR的启动流程。
 1. 了解GRUB的启动流程。
 1. 比较NTLDR和GRUB的功能有差异。
 1. 了解u-boot的功能。

## 3.3 中断、异常和系统调用比较
 1. 举例说明Linux中有哪些中断，哪些异常？
 1. Linux的系统调用有哪些？大致的功能分类有哪些？  (w2l1)

>　
　Linux系统调用，包含了大部分常用系统调用和由系统调用派生出的的函数,狭义上的系统调用就有二百多个，不同的内核系统调用数量不同，但数量均十分庞大。
主要包括如下几个大类（并举例）：
> 
+ 进程控制  
fork创建一个新进程  
exit中止进程  
+ 文件读写操作  
open打开文件  
read读文件  
+ 文件系统操作  
chdir改变当前工作目录  
stat取文件状态信息  
+ 系统控制  
reboot重新启动  
time取得系统时间  
+ 内存管理  
sync 将内存缓冲区数据写回硬盘  
+ 网络管理  
getdomainname 取域名  
+ socket控制  
socket 建立socket  
bind 绑定socket到端口  
+ 用户管理  
getuid 获取用户标识号  
+ 进程间通信  
ipc 进程间通信总控制调用  
+ 信号  
kill 向进程或进程组发信号  
+ 消息  
msgsnd 发消息  
msgrcv 取消息  
+ 管道  
pipe 创建管道  
+ 信号量  
semget 获取一组信号量  
+ 共享内存  
shmget 获取共享内存  
>等等  


```
  + 采分点：说明了Linux的大致数量（上百个），说明了Linux系统调用的主要分类（文件操作，进程管理，内存管理等）
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```
 
 1. 以ucore lab8的answer为例，uCore的系统调用有哪些？大致的功能分类有哪些？(w2l1)
 
> 
从lab8_result/kern/sys_call.c中可以看到，ucore的系统调用有二十多个函数，主要包括了文件操作、进程管理、内存管理、系统管理等方面的系统调用，下面参考linux的系统调用，依次说明ucore系统调用的详细功能：
+ 进程管理相关的系统调用有： 
sys_exit，终止进程  
sys_fork，创建进程  
sys_wait，等待子进程终止  
sys_exec，运行子程序  
sys_yield，进程主动让出处理器,并将自己等候调度队列队尾  
sys_kill，向进程或进程组发信号  
sys_getpid，获取进程标识号  
sys_putc，输出一个字符  
sys_pgdir，  
sys_gettime，获取时间  
sys_sleep，睡眠   
+ 文件操作相关的系统调用有：   
sys_open，打开文件  
sys_close，关闭文件  
sys_read，读取文件  
sys_write，写入文件  
sys_seek，移动文件指针  
sys_fstat，读取文件信息  
sys_fsync，把文件在内存中的信息写入磁盘  
sys_getcwd，获取当前的工作目录  
sys_getdirentry，获取目录下的文件  
sys_dup，复制已打开的文件描述字   

 ```
  + 采分点：说明了ucore的大致数量（二十几个），说明了ucore系统调用的主要分类（文件操作，进程管理，内存管理等）
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```
 
## 3.4 linux系统调用分析
 1. 通过分析[lab1_ex0](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex0.md)了解Linux应用的系统调用编写和含义。(w2l1)

> 
+ objdump是Linux下的一个反汇编工具
如运行`objdump -d lab1-ex0.exe`中的部分信息：
```
Disassembly of section .text:
> 
0000000000400400 <_start>:
  400400:	31 ed                	xor    %ebp,%ebp
  400402:	49 89 d1             	mov    %rdx,%r9
  400405:	5e                   	pop    %rsi
  400406:	48 89 e2             	mov    %rsp,%rdx
  400409:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  40040d:	50                   	push   %rax
  40040e:	54                   	push   %rsp
  40040f:	49 c7 c0 60 05 40 00 	mov    $0x400560,%r8
  400416:	48 c7 c1 f0 04 40 00 	mov    $0x4004f0,%rcx
  40041d:	48 c7 c7 45 10 60 00 	mov    $0x601045,%rdi
  400424:	e8 b7 ff ff ff       	callq  4003e0 <__libc_start_main@plt>
  400429:	f4                   	hlt    
  40042a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
```
+ nm可以显示指定文件中符号的信息，可以有库、对象名，符号类型、名称等等信息
如运行`nm lab1-ex0.exe`的部分信息：
```
0000000000000002 a AF_INET
000000000060105c B __bss_start
000000000060105c b completed.6972
0000000000601028 D __data_start
0000000000601028 W data_start
0000000000400430 t deregister_tm_clones
00000000004004a0 t __do_global_dtors_aux
0000000000600e18 t __do_global_dtors_aux_fini_array_entry
0000000000601030 D __dso_handle
0000000000600e28 d _DYNAMIC
000000000060105c D _edata
0000000000601060 B _end
0000000000400564 T _fini
00000000004004c0 t frame_dummy
0000000000600e10 t __frame_dummy_init_array_entry
0000000000400670 r __FRAME_END__
0000000000601000 d _GLOBAL_OFFSET_TABLE_
                 w __gmon_start__
0000000000601038 d hello
```
+ file可以查看某一个文件的类型及其他对应的详细信息
运行`file lab1-ex0.exe`可得如下信息，说明它是一个可执行文件：
```
lab1-ex0.exe: ELF 64-bit LSB  executable, x86-64, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=d1c9294b45dbb8faf13e3db2d50aac427cafaad6, not stripped
```
　　系统调用：实际上运行该程序可以看到程序的功能是输出预先定义好的字符串`"hello world\n"`，源代码中由四个`movl`命令将各个参数移动到eax，ebx，ecx，edx四个寄存器中，然后通过`int $0x80`触发系统调用，系统即可根据上述四个寄存器中的中断号和参数信息完成本次的“输出”系统调用。其他系统调用和本次不同之处在于参数的传递和要求不同。

 ```
  + 采分点：说明了objdump，nm，file的大致用途，说明了系统调用的具体含义
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 
 ```
 
 1. 通过调试[lab1_ex1](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex1.md)了解Linux应用的系统调用执行过程。(w2l1)

> 
　　参考http://www.cnblogs.com/ggjucheng/archive/2012/01/08/2316692.html
strace常用来跟踪进程执行时的系统调用和所接收的信号。 在Linux世界，进程不能直接访问硬件设备，当进程需要访问硬件设备(比如读取磁盘文件，接收网络数据等等)时，必须由用户态模式切换至内核态模式，通过系统调用访问硬件设备。strace可以跟踪到一个进程产生的系统调用,包括参数，返回值，执行消耗的时间。
strace -c 统计每一系统调用的执行的时间，次数和出错的次数。
运行strace命令可以得到如下结果：
```
moocos-> strace -c ./lab1-ex1.exe
hello world
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 25.56    0.000023          23         1           write
 22.22    0.000020           3         8           mmap
 17.78    0.000016           4         4           mprotect
 11.11    0.000010          10         1           munmap
 10.00    0.000009           5         2           open
  8.89    0.000008           3         3         3 access
  1.11    0.000001           1         1           read
  1.11    0.000001           1         2           close
  1.11    0.000001           1         1           execve
  1.11    0.000001           1         1           arch_prctl
  0.00    0.000000           0         3           fstat
  0.00    0.000000           0         1           brk
------ ----------- ----------- --------- --------- ----------------
100.00    0.000090                    28         3 total
```
　　这个程序是一个简单的输出helloworld的程序，运行strace命令后首先输出了hello world，接着输出了执行这个程序过程中产生的系统调用。第一列第二列表示各个系统调用占用的时间和百分比，第三四五列对应了调用的次数和产生错误的次数，可以看到write/mmap/mprotect占用的时间最多，这三个对应了写入文件/映射虚拟内存页/设置内存映像保护。
```
[~/Desktop/ucore_lab-master/related_info/lab1]
moocos-> more /proc/interrupts
           CPU0       
  0:         57   IO-APIC-edge      timer
  1:        524   IO-APIC-edge      i8042
  8:          0   IO-APIC-edge      rtc0
  9:          0   IO-APIC-fasteoi   acpi
 12:        625   IO-APIC-edge      i8042
 14:          0   IO-APIC-edge      ata_piix
 15:        462   IO-APIC-edge      ata_piix
 19:       2989   IO-APIC-fasteoi   eth0
 20:       3301   IO-APIC-fasteoi   vboxguest
 21:      12566   IO-APIC-fasteoi   ahci, snd_intel8x0
 22:         29   IO-APIC-fasteoi   ohci_hcd:usb1
NMI:          0   Non-maskable interrupts
LOC:      33971   Local timer interrupts
SPU:          0   Spurious interrupts
PMI:          0   Performance monitoring interrupts
IWI:       7090   IRQ work interrupts
RTR:          0   APIC ICR read retries
RES:          0   Rescheduling interrupts
CAL:          0   Function call interrupts
TLB:          0   TLB shootdowns
TRM:          0   Thermal event interrupts
THR:          0   Threshold APIC interrupts
MCE:          0   Machine check exceptions
MCP:          2   Machine check polls
ERR:          0
MIS:          0
```
　　/proc/interrupts文件中存放了系统中的中断情况，从左到右依次是irq的序号，在各自cpu发生中断的次数，可编程中断控制器，设备名称。例如中断号19的中断口，在cpu0上响应了2989次中断，链接在这个端口的中断链表上的设备结构是IO-APIC-fasteoi，这个中断号是设备名为eth0，即网卡。  
　　系统调用执行过程（和strace似乎没有太多关系）： 参考http://blog.chinaunix.net/uid-9688646-id-3088247.html
系统调用是通过软中断指令 INT 0x80 实现的，而这条INT 0x80指令就被封装在C库的函数中。
　（软中断和我们常说的硬中断不同之处在于，软中断是由指令触发的，而不是由硬件外设引起的。）
　　INT 0x80 这条指令的执行会让系统跳转到一个预设的内核空间地址，它指向系统调用处理程序，即system_call函数。  
　　system_call函数是怎么找到具体的系统调用服务例程的？通过系统调用号查找系统调用表sys_call_table！软中断指令INT 0x80执行时，系统调用号会被放入 eax 寄存器中，system_call函数可以读取eax寄存器获取，然后将其乘以4，生成偏移地址，然后以sys_call_table为基址，基址加上偏移地址，就可以得到具体的系统调用服务例程的地址了！  
　　然后就到了系统调用服务例程了。需要说明的是，系统调用服务例程只会从堆栈里获取参数，所以在system_call执行前，会先将参数存放在寄存器中，system_call执行时会首先将这些寄存器压入堆栈。system_call退出后，用户可以从寄存器中获得（被修改过的）参数。  
　　另外：系统调用通过软中断INT 0x80陷入内核，跳转到系统调用处理程序system_call函数，然后执行相应的服务例程。但是由于是代表用户进程，所以这个执行过程并不属于中断上下文，而是进程上下文。因此，系统调用执行过程中，可以访问用户进程的许多信息，可以被其他进程抢占，可以休眠。  
　　当系统调用完成后，把控制权交回到发起调用的用户进程前，内核会有一次调度。如果发现有优先级更高的进程或当前进程的时间片用完，那么会选择优先级更高的进程或重新选择进程执行。

 ```
  + 采分点：说明了strace的大致用途，说明了系统调用的具体执行过程（包括应用，CPU硬件，操作系统的执行过程）
  - 答案没有涉及上述两个要点；（0分）
  - 答案对上述两个要点中的某一个要点进行了正确阐述（1分）
  - 答案对上述两个要点进行了正确阐述（2分）
  - 答案除了对上述两个要点都进行了正确阐述外，还进行了扩展和更丰富的说明（3分）
 ```
 
## 3.5 ucore系统调用分析
 1. ucore的系统调用中参数传递代码分析。
 1. ucore的系统调用中返回结果的传递代码分析。
 1. 以ucore lab8的answer为例，分析ucore 应用的系统调用编写和含义。
 1. 以ucore lab8的answer为例，尝试修改并运行代码，分析ucore应用的系统调用执行过程。
 
## 3.6 请分析函数调用和系统调用的区别
 1. 请从代码编写和执行过程来说明。
   1. 说明`int`、`iret`、`call`和`ret`的指令准确功能
 
