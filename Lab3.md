# HowToDebug

- in shell run `make qemu-gdb`
- then in another tab run `gdb-multiarch`
- then encounter a problem, says "warning: File "/media/nvidia/Ubuntu4T/CS/MIT_6s081/xv6-labs-2020/.gdbinit" auto-loading has been declined by your `auto-load safe-path' set to "$debugdir:$datadir/auto-load".
- solution: `source .gdbinit`

# GDB

- b funcname/line //设置断点
> b 25 //在25行设置断点
> b main//在main函数第一行设置断点

- layout split  //显示汇编

- next //运行下一行,不进入内部
- step //进入内部

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
堆栈指针
> addi sp, sp, -16 //分配16字节空间
> sd ra, 8(sp)
> sd s0, 0(sp)
> ld ra, 8(sp)
> ld s0, 0(sp)
> addi sp, sp, 16 //释放16字节空间
> ret

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






