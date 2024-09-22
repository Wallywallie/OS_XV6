# problem: Handle faults on the invalid page below the user stack.
测试stacktest时，发现遇到load page fault然后panic: remap的现象，检查之后发现是因为访问*sp导致的：读取一个已经map过的(~PTE_U)页面-load page fault-重新分配内存并mappages-remap
因此需要在遇到load page fault时，需要分辨是lazy allocation导致的，还是访问guard page导致的

# Handle the case in which a process passes a valid address from sbrk() to a system call such as read or write, but the memory for that address has not yet been allocated.
测试sbrkarg时发现，因为sbrk输出的地址还没有分配内存，向这个地址写入的时候会不停地产生store page fault并分配一个page直到耗尽物理内存。
原因是sys_write调用walkaddr，会返回0
