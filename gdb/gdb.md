[toc]


# 常见指令

进入gdb 
在gcc中编译gdb: gcc -g -o 执行文件名 程序文件
开始调试  gdb 执行文件名


## 基础指令 
>1. b break + line 创建断点 -- 如b10 在10行创建端点
>2. r run 执行到第一个断点的位置，如果没有断点就执行完毕
>3. n next执行下一行语句 但是如果这是一个函数调用则不会进入函数内部而是直接执行完毕
>4. p printf 输出变量名字 如:p argc;p后面可以加一个表达式。例如p ii+100 返回的是就是ii+100的值
>5. c continue 继续运行程序直到下一个断点
>6. set var [name]=value 设置变量的值，例如程序有两个变量:int ii;chr name[21];set var ii=10 把ii的值设置为10;set var_name = "xxx";
>7. q quit 关闭
>8. set args xx xx xx 设置程序参数。如果程序中需要传入参数(agrs,argc),这里就需要输入程序参数才能运行。如果参数中包含特殊字符，就用""包含
>9. s step 执行当前语句如果语句为函数调用，则进入函数执行其中的第一条语句。当然进入的函数必须是由源代码的，如果是第三方库函数，就不会进入
>10. bt 查询函数栈
>11. frame 使用 frame 命令切换到函数栈中不同的帧，显示函数上下文
>12. info 查看某些信息 如info b 查看断点

## gdb正在运行的程序
查询到运行程序的进程号，使用gdb -p  进程号进性调试。此时会阻塞正在执行的程序进程

当关闭gdb后就会自动回复
或者进入gdb之后使用attach + 进程号
查找进程号:
ps -e | grep 'name'
如对run的程序
ps -e | grep run


## gdb调试多进程
例如在C++中使用fork的方式创建两个进程

使用gdb调试的时候默认调试的是父进程(此时子进程不会因为调试而阻塞，子进程会继续执行)

如果想调试子进程:
set follow-fork-mode child设置之后再run就会进入子进程调试模式

set detach-on-fork [on|off]默认是on表示在调试当前进程的时候如果其他进程继续执行，
如果用off表示调试当前进程的时候其他进程就会被gdb挂起

info inferiors 查看当前调试的进程查看当前进程的id，并且切换当前调试的进程:inferior 进程id

## gdb调试多线程
例如在c++中创建两个线程

首先可以使用 ps -al | grep book 查看当前正在运行的轻量级的进程，其中会显示主线程id 和子线程id


在调试的时候，两个线程是相互竞争时间片的会交替执行


调试的过程中:
>1. info threads 显示所有的线程的数量，线程id前面的* 表示当前线程
>2. thread [id] 切换线程，id是info threads创建的，通过thread id切换到这个id
>3. set scheduler-locking on 只运行当前线程
>4. set scheduler-locking off 运行所有线程
>5. 指定某线程执行某gdb命令:thread apply 线程id cmd  如thread apply 2 n 让线程2执行n操作
>6. 全部线程执行某gdb命令:thread apply all cmd



问题:你知道怎么用gdb检查死锁吗

按r之后由于死锁程序会停下，此时执行info threads查看所有线程，没有*的就是被阻塞的线程。之后可以thread apply all bt(查看栈信息)  此时出现lock waits就是被锁阻塞的线程


问题:你知道gdb怎么检查内存泄漏吗
使用info malloc