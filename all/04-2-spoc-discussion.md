#lec9 虚存置换算法spoc练习


## 个人思考题
1. 置换算法的功能？
> 当出现缺页异常，需调入新页面而内存已满时，置换算法选择被置换的物理页面

2. 全局和局部置换算法的不同？ 
> 全局置换页面的选择范围仅限于当前进程占用的物理页面内  
局部置换页面的选择范围是所有可换出的物理页面 

3. 最优算法、先进先出算法和LRU算法的思路？
> 最优：置换在未来最长时间不访问的页面  
先进先出：选择在内存驻留时间最长的页面进行置换  
LRU：选择最长时间没有被引用的页面进行置换  

4. 时钟置换算法的思路？ 
> 仅对页面的访问情况进行大致统计 
在页表项中增加访问位，描述页面在过去一段时间的内访问情况 
各页面组织成环形链表  
指针指向最先调入的页面 
访问页面时，在页表项记录页面访问情况 
缺页时，从指针处开始顺序查找未被访问的页面进行置换 

5. LFU算法的思路？
> 缺页时，置换访问次数最少的页面 

6. 什么是Belady现象？
> 采用FIFO等算法时，可能出现分配的物理页面数增加，缺页次数反而升高的异常现象 

7. 几种局部置换算法的相关性：什么地方是相似的？什么地方是不同的？为什么有这种相似或不同？
> 都是先进先出的思路 

8. 什么是工作集？
> 一个进程当前正在使用的逻辑页面集合，可表示为二元函数W  
 
9. 什么是常驻集？
> 在当前时刻，进程实际驻留在内存当中的页面集合

10. 工作集算法的思路？
> 换出不在工作集中的页面 

11. 缺页率算法的思路？
> 通过调节常驻集大小，使每个进程的缺页率保持在一个合理的范围内 

12. 什么是虚拟内存管理的抖动现象？
> 进程物理页面太少，不能包含工作集 
造成大量缺页，频繁置换 
进程运行速度变慢 

13. 操作系统负载控制的最佳状态是什么状态？
> 通过调节并发进程数（MPL）来进行系统负载控制 

## 小组思考题目

----
(1)（spoc）请证明为何LRU算法不会出现belady现象

####解答

参考http://ranger.uta.edu/~weems/NOTES5314/HW/hw3.3.pdf

假设逻辑页访问顺序为S=a<sub>1</sub>,a<sub>2</sub>,....a<sub>n</sub>。令LRU_<sub>i</sub>(S)表示当物理页面数为i时的缺页次数，我们需要证明的是对于任意的S,i<j，满足LRU<sub>i</sub> >= LRU<sub>i+1</sub>(S) >= LRU<sub>i+2</sub>(S) >= ... >= LRU<sub>j</sub>(S)

定义一个i个节点的链表是i+1个节点的链表的**前缀**，当且仅当两个链表前i个节点完全相同，仅i+1个点的链表可能在尾部多了1个节点。

引理：在访问序列每一步结束的时候，LRU<sub>i</sub>的链表是LRU<sub>i+1</sub>的链表的**前缀**。

归纳证明这个引理：

首先当k = 1时，LRU<sub>1</sub>和LRU<sub>2</sub>均产生1次缺失，由a1产生。归纳假设当k < n时，均有LRU<sub>k</sub>的链表是LRU<sub>k+1</sub>的链表的**前缀**。

考虑当k = n时的情况：

* 一种情况是当访问第n+1页时，a<sub>n+1</sub>已经在LRU<sub>i</sub>的链表中，此时页面在内存中，那么第n+1个访问结束后，都是将这个页面移动到链表的开头，因此引理成立。
* 另一种情况是当访问第n+1页时，a<sub>n+1</sub>不在LRU<sub>i</sub>的链表中。这里再细分两种情况：
  * a<sub>n+1</sub>不在LRU<sub>i+1</sub>的链表中，则发生页缺失，LRU_i和LRU<sub>i+1</sub>都需要把a_n+1移动到链表头
  * a<sub>n+1</sub>在LRU<sub>i+1</sub>的链表中，那么由归纳假设，a<sub>n+1</sub>一定是在链表的最后一页，那么LRU<sub>i+1</sub>把这一页移动到链表头，将原来链表尾的页面删除；LRU<sub>i</sub>同样把a<sub>n+1</sub>移动到链表头，把链表尾的页面删除。那么由归纳假设可知，链表中另外i-1个元素不变，LRU<sub>n</sub>的链表依然是LRU<sub>n+1</sub>的前缀。
  
当引理成立时，若LRU<sub>i+1</sub>产生页缺失，则LRU<sub>i</sub>产生页缺失。那么LRU<sub>i</sub>>=LRU<sub>i+1</sub>。即结论成立。


(2)（spoc）根据你的`学号 mod 4`的结果值，确定选择四种替换算法（0：LRU置换算法，1:改进的clock 页置换算法，2：工作集页置换算法，3：缺页率置换算法）中的一种来设计一个应用程序（可基于python, ruby, C, C++，LISP等）模拟实现，并给出测试。请参考如python代码或独自实现。
 - [页置换算法实现的参考实例](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab3/page-replacement-policy.py)

学号2012011367 mod 4 = 3

缺页率置换算法

输入文件如下：（按视频上的样例）
```
2 2 3 1 2 4 2 4 0 3 
``` 


运行结果如下：
```
Add into set 2.
Access page 2.
Access page 3.
0 4  replaced.
Access page 2.
Add into set 4.
Access page 2.
Access page 4.
1 3  replaced.
Add into set 3.
```

代码实现如下：
```
#include <cstdio>
#include <vector>
using namespace std;
#define SIZE 2
#define TOTAL 5
bool pages[TOTAL];
bool use[TOTAL];
int last = -1;
void process(int order, int pageno) {
    use[pageno] = true;
    if (pages[pageno]) {
        printf("Access page %d.\n", pageno);
        return;
    }
    
    if ((last < 0) || ((order - last)<=SIZE)) {
        pages[pageno] = true;
        printf("Add into set %d.\n", pageno);
        last = order;
        return;
    } 
    for (int i=0; i<TOTAL; i++) {
        if (pages[i] && !use[i]) printf("%d ", i);
        pages[i] = use[i];
        use[i] = false;
    }
    printf(" replaced.\n");
    last = order;
    return;
}
int main() {
    pages[0] = true;
    pages[3] = true;
    pages[4] = true;
    pages[1] = pages[2] = false;
    for (int i=0; i<TOTAL; i++) use[i] = false;
    int pageno, order;
    order = 0;
    while (scanf("%d", &pageno) != EOF) {
        process(++order, pageno);
    }
    return 0;
}
```


 
## 扩展思考题
（1）了解LIRS页置换算法的设计思路，尝试用高级语言实现其基本思路。此算法是江松博士（导师：张晓东博士）设计完成的，非常不错！

参考信息：

 - [LIRS conf paper](http://www.ece.eng.wayne.edu/~sjiang/pubs/papers/jiang02_LIRS.pdf)
 - [LIRS journal paper](http://www.ece.eng.wayne.edu/~sjiang/pubs/papers/jiang05_LIRS.pdf)
 - [LIRS-replacement ppt1](http://dragonstar.ict.ac.cn/course_09/XD_Zhang/(6)-LIRS-replacement.pdf)
 - [LIRS-replacement ppt2](http://www.ece.eng.wayne.edu/~sjiang/Projects/LIRS/sig02.ppt)
