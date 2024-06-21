# stack frame
return addr -> (-8)fp
previous stack frame pointer -> (-16)fp

# trap

Every tick, the hardware clock forces an interrupt, which is handled in usertrap() in kernel/trap.c.