.globl main
 main: 
    push %rbp
    mov %rsp, %rbp
    movl $10, %eax
    mov %rbp, %rsp
    pop %rbp
    ret
