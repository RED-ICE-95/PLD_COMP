.globl main
 main: 
    push %rbp
    mov %rsp, %rbp
    sub $4, %rsp
    movl $65, %eax
    sub $4, %rsp
    movl %eax, -8(%rbp)
    movl $5, %eax
    addl -8(%rbp), %eax
    add $4, %rsp
    movl %eax, -4(%rbp)
    movl -4(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
