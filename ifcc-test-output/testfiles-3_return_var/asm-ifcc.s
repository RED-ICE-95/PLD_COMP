.globl main
 main: 
    push %rbp
    mov %rsp, %rbp
    sub $4, %rsp
    movl $8, %eax
    movl %eax, -4(%rbp)
    movl -4(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
