# HowToDebug

- in shell run `make qemu-gdb`
- then in another tab run `gdb-multiarch`
- then encounter a problem, says "warning: File "/media/nvidia/Ubuntu4T/CS/MIT_6s081/xv6-labs-2020/.gdbinit" auto-loading has been declined by your `auto-load safe-path' set to "$debugdir:$datadir/auto-load".
- solution: `source .gdbinit`

# GDB

- b funcname/line //设置断点
- b 25 //在25行设置断点
- b main//在main函数第一行设置断点

- i frame //查看栈帧信息
- bt //backtrace,查看所有栈帧信息
- frame 序号 //查看具体函数的栈帧


- tui enable //调出显示窗口
- tui apropos //查看更多tui相关命令
- layout split  //显示汇编和源码
- layout source //显示源码
- layout asm //显示汇编
- layout reg //显示寄存器

- next //运行下一行,不进入内部
- step //进入内部

- p *argv@argc //p for print,打印argc个参数
- watch 变量 //变量变化时，会得到通知
- x/d 地址 //x for examine, d for decimal, x for hexadecimal,指用十进制显示所在地址的内存内容

# 汇编

## auipc
`auipc rd imm`
add upper immediate to pc
-- rd 目标寄存器
-- imm 20位的立即数
执行时，将imm左移12位，与当前pc的值相加，存入rd中
常用来生成位置无关代码PIC,如结合jalr使用，构建基于当前地址的的跳转或调用

## jalr
`jalr rd, rs1, imm`:jump and link register
将计算出的目标地址写入PC，并将跳转的返回地址写入指定的寄存器
`target addr = (rs1 + imm) & ~1`,其中`& ~1`确保目标地址是偶数地址

## ld
`ld rd, offset(rs1)`:加载64位数据（双字）到寄存器中

## addi
`addi rd, rs1, imm`

## sd
`sd rs2, offset(rs1)`:将寄存器rs2中的64位数据储存到rs1和offset计算得到的地址中

## sp
栈指针
> addi sp, sp, -16 //分配16字节空间，开辟函数的栈帧
> sd ra, 8(sp)     //把函数的返回地址，保存到ra上
> sd s0, 0(sp)
> ld ra, 8(sp)     //将返回地址，重新保存到ra上
> ld s0, 0(sp)
> addi sp, sp, 16  //释放16字节空间，删除栈帧
> ret              //返回

# 疑问
1. main在一开始会init多少个proc,即,procinit和allocproc分别什么时候调用，调用多少次？
procinit -> 在进入main时调用，但还没有进入scheduler之前
allocproc -> userinit(main中调用一次，生成user第一个proc)
          -> fork



# xv6架构
kernel pagetable -> maps kernel stack to kernel pagetable
xv6在初始化的时候会创建64个进程，每一个进程都有一个kernel stack
kernel stack的作用是：进程进行上下文切换的时候保存寄存器状态
每一个进程结束的时候 kernel stack会被回收
user kernel pagetable只记录自己的kernel stack在哪
system kernel pagetable记录所有的进程的kernel stack
copyin函数输入进程的pagetable将地址写入kernel pagetable中供kernel使用，其中的dst通常是usrgv
疑问：为什么原本的copyin要把pa写到dst去??是因为原本的kernel pagetable全部都是identity mapping吗？

kernel stack 映射在 虚拟地址的高位，而进程的虚拟地址都在低位，所以把进程的pagetable数据复制到kernel pagetable时，两者不会冲突，
这样kenel在执行命令时，可以直接在kernel pagetable中查找pa

# tmux
ctrl-b c 新窗口
ctrl-b p 切换前一个窗口
ctrl-b n 切换下一个窗口
ctrl-b % 垂直拆分窗口
ctrl-b " 水平拆分窗口
ctrl-b o 在不同窗口间切换


# calling convention
## caller saved
寄存器的值在函数调用时不会被保存,其值可以被函数覆写

## callee saved
寄存器的值在函数调用时需要被保存

# frame stack
 frame stack 是由 function call 产生的
 fp -> frame pointer, 指向一个栈帧的最上面
 sp -> stack pointer, 指向一个栈帧的最下面
一个栈帧中最上面的是return address，
其次是指向前一个栈帧的fp
其次是saved register
其次是local variables
...

# 遇到的问题
map pages时忘记加PTE_V引发缺页故障