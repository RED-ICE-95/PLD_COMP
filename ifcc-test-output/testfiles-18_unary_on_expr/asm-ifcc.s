.globl main
 main: 
    push %rbp
    mov %rsp, %rbp
    movl $2, %eax
    sub $4, %rsp
    movl %eax, -4(%rbp)
    movl $3, %eax
    addl -4(%rbp), %eax
    add $4, %rsp
    negl %eax
    sub $4, %rsp
    movl %eax, -4(%rbp)
    movl $20, %eax
    addl -4(%rbp), %eax
    add $4, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
