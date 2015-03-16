# lec5 SPOC思考题


NOTICE
- 有"w3l1"标记的题是助教要提交到学堂在线上的。
- 有"w3l1"和"spoc"标记的题是要求拿清华学分的同学要在实体课上完成，并按时提交到学生对应的git repo上。
- 有"hard"标记的题有一定难度，鼓励实现。
- 有"easy"标记的题很容易实现，鼓励实现。
- 有"midd"标记的题是一般水平，鼓励实现。


## 个人思考题
---

请简要分析最优匹配，最差匹配，最先匹配，buddy systemm分配算法的优势和劣势，并尝试提出一种更有效的连续内存分配算法 (w3l1)
```
  + 采分点：说明四种算法的优点和缺点
  - 答案没有涉及如下3点；（0分）
  - 正确描述了二种分配算法的优势和劣势（1分）
  - 正确描述了四种分配算法的优势和劣势（2分）
  - 除上述两点外，进一步描述了一种更有效的分配算法（3分）
 ```
- [x]  

>  
+ 最优匹配  
　　优点：大部分分配的尺寸较小时效果好（可以避免大的空闲分区被拆分，可以减小外部碎片的大小，相对简单）  
　　缺点：会产生外部碎片，释放分区较慢，容易产生很多无用的小碎片  
+ 最差匹配  
　　优点：中等大小的分配较多时，效果最好，可避免出现太多的小碎片  
　　缺点：会产生外部碎片，释放分区较慢，容易破坏大的空闲分区，使得后续难以分配大的分区　　
+ 最先匹配  
　　优点：实现简单，在高地址区域有大块空间分区可供利用  
　　缺点：会产生外部碎片，在分配大块时速度较慢　　
+ buddy system  
　　优点：减小了外部碎片的大小，释放分区较快、较简单  
　　缺点：存在内部碎片，在分区大小略大于２的幂时浪费较大  
另一种可行的分配方法：  
　　循环最先匹配方法：和最先匹配方法类似，只不过每次不是从头开始查找，而是从上次找到的空闲分区开始查找，这种方法可以使得空闲中的内存分布更加均匀，实现起来也简单，但将会缺乏大的空闲分区。  


## 小组思考题

请参考ucore lab2代码，采用`struct pmm_manager` 根据你的`学号 mod 4`的结果值，选择四种（0:最优匹配，1:最差匹配，2:最先匹配，3:buddy systemm）分配算法中的一种或多种，在应用程序层面(可以 用python,ruby,C++，C，LISP等高语言)来实现，给出你的设思路，并给出测试用例。 (spoc)

>  
这里采用buddy system的分配算法，用C++语言实现。  
参考资料见[伙伴分配器的一个极简实现](http://coolshell.cn/articles/10427.html#more-10427)  
代码见[03-1-spoc-code/buddy.cpp](https://github.com/swnhieian/os_exercises/tree/master/all/03-1-spoc-code/buddy.cpp "buddy.cpp")  
以下为测试用例及结果（以16个单位长的内存为例，运行编译出的程序得到的结果）
```
Initialize success!
>> input 'm'+size for malloc; 'f'+addr for free; 'q' for quit
>>m5
Allocate address:0
==========Memory Detail==========
  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
  *  *  *  *  *  *  *  *  _  _  _  _  _  _  _  _
>> input 'm'+size for malloc; 'f'+addr for free; 'q' for quit
>>m3
Allocate address:8
==========Memory Detail==========
  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
  *  *  *  *  *  *  *  *  *  *  *  *  _  _  _  _
>> input 'm'+size for malloc; 'f'+addr for free; 'q' for quit
>>m1
Allocate address:12
==========Memory Detail==========
  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
  *  *  *  *  *  *  *  *  *  *  *  *  *  _  _  _
>> input 'm'+size for malloc; 'f'+addr for free; 'q' for quit
>>m2
Allocate address:14
==========Memory Detail==========
  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
  *  *  *  *  *  *  *  *  *  *  *  *  *  _  *  *
>> input 'm'+size for malloc; 'f'+addr for free; 'q' for quit
>>f1
Free success!
==========Memory Detail==========
  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
  _  _  _  _  _  _  _  _  *  *  *  *  *  _  *  *
>> input 'm'+size for malloc; 'f'+addr for free; 'q' for quit
>>f12
Free success!
==========Memory Detail==========
  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
  _  _  _  _  _  _  _  _  *  *  *  *  _  _  *  *
>> input 'm'+size for malloc; 'f'+addr for free; 'q' for quit
>>f14
Free success!
==========Memory Detail==========
  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
  _  _  _  _  _  _  _  _  *  *  *  *  _  _  _  _
>> input 'm'+size for malloc; 'f'+addr for free; 'q' for quit
>>f9
Free success!
==========Memory Detail==========
  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _
>> input 'm'+size for malloc; 'f'+addr for free; 'q' for quit
>>f2
Free failed! Invalied address!
==========Memory Detail==========
  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _
>> input 'm'+size for malloc; 'f'+addr for free; 'q' for quit
>>q
```

--- 

## 扩展思考题

阅读[slab分配算法](http://en.wikipedia.org/wiki/Slab_allocation)，尝试在应用程序中实现slab分配算法，给出设计方案和测试用例。

## “连续内存分配”与视频相关的课堂练习

### 5.1 计算机体系结构和内存层次
MMU的工作机理？

- [x]  

>  http://en.wikipedia.org/wiki/Memory_management_unit

L1和L2高速缓存有什么区别？

- [x]  

>  http://superuser.com/questions/196143/where-exactly-l1-l2-and-l3-caches-located-in-computer
>  Where exactly L1, L2 and L3 Caches located in computer?

>  http://en.wikipedia.org/wiki/CPU_cache
>  CPU cache

### 5.2 地址空间和地址生成
编译、链接和加载的过程了解？

- [x]  

>  

动态链接如何使用？

- [x]  

>  


### 5.3 连续内存分配
什么是内碎片、外碎片？

- [x]  

>  

为什么最先匹配会越用越慢？

- [x]  

>  

为什么最差匹配会的外碎片少？

- [x]  

>  

在几种算法中分区释放后的合并处理如何做？

- [x]  

>  

### 5.4 碎片整理
一个处于等待状态的进程被对换到外存（对换等待状态）后，等待事件出现了。操作系统需要如何响应？

- [x]  

>  

### 5.5 伙伴系统
伙伴系统的空闲块如何组织？

- [x]  

>  

伙伴系统的内存分配流程？

- [x]  

>  

伙伴系统的内存回收流程？

- [x]  

>  

struct list_entry是如何把数据元素组织成链表的？

- [x]  

>  



