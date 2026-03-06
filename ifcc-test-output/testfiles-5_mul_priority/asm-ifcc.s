.globl main
 main: 
    push %rbp
    mov %rsp, %rbp
    movl $2, %eax
    sub $4, %rsp
    movl %eax, -4(%rbp)
    movl $3, %eax
    sub $4, %rsp
    movl %eax, -8(%rbp)
    movl $4, %eax
    imull -8(%rbp), %eax
    add $4, %rsp
    addl -4(%rbp), %eax
    add $4, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
