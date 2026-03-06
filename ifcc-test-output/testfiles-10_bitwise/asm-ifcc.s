.globl main
 main: 
    push %rbp
    mov %rsp, %rbp
    movl $6, %eax
    sub $4, %rsp
    movl %eax, -4(%rbp)
    movl $5, %eax
    andl -4(%rbp), %eax
    add $4, %rsp
    sub $4, %rsp
    movl %eax, -4(%rbp)
    movl $3, %eax
    xorl -4(%rbp), %eax
    add $4, %rsp
    sub $4, %rsp
    movl %eax, -4(%rbp)
    movl $8, %eax
    orl -4(%rbp), %eax
    add $4, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
