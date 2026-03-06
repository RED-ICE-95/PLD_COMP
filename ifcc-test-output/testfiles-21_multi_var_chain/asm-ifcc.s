.globl main
 main: 
    push %rbp
    mov %rsp, %rbp
    sub $4, %rsp
    sub $4, %rsp
    sub $4, %rsp
    movl $10, %eax
    movl %eax, -12(%rbp)
    movl $3, %eax
    movl %eax, -8(%rbp)
    movl -12(%rbp), %eax
    sub $4, %rsp
    movl %eax, -16(%rbp)
    movl -8(%rbp), %eax
    imull -16(%rbp), %eax
    add $4, %rsp
    sub $4, %rsp
    movl %eax, -16(%rbp)
    movl -12(%rbp), %eax
    movl -16(%rbp), %ecx
    subl %eax, %ecx
    movl %ecx, %eax
    add $4, %rsp
    sub $4, %rsp
    movl %eax, -16(%rbp)
    movl -8(%rbp), %eax
    addl -16(%rbp), %eax
    add $4, %rsp
    movl %eax, -4(%rbp)
    movl -4(%rbp), %eax
    mov %rbp, %rsp
    pop %rbp
    ret
