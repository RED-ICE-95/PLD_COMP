.globl main
 main: 
    push %rbp
    mov %rsp, %rbp
    movl $42, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
