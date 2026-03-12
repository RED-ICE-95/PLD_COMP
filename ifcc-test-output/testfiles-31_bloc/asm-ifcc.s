.globl main
 main:
    push %rbp
    mov %rsp, %rbp
    sub $4, %rsp
    movl $2, %eax
    movl %eax, -4(%rbp)
    sub $4, %rsp
    movl $2, %eax
    movl %eax, -8(%rbp)
    sub $4, %rsp
    movl $5, %eax
    movl %eax, -12(%rbp)
    sub $4, %rsp
    movl -12(%rbp), %eax
    sub $4, %rsp
    movl %eax, -20(%rbp)
    movl $2, %eax
    addl -20(%rbp), %eax
    add $4, %rsp
    movl %eax, -16(%rbp)
    movl -4(%rbp), %eax
    movl -4(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
