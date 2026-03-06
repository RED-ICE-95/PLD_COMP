.globl main
 main: 
    push %rbp
    mov %rsp, %rbp
    movl $3, %eax
    sub $4, %rsp
    movl %eax, -4(%rbp)
    movl $2, %eax
    addl -4(%rbp), %eax
    add $4, %rsp
    sub $4, %rsp
    movl %eax, -4(%rbp)
    movl $2, %eax
    imull -4(%rbp), %eax
    add $4, %rsp
    sub $4, %rsp
    movl %eax, -4(%rbp)
    movl $1, %eax
    movl -4(%rbp), %ecx
    subl %eax, %ecx
    movl %ecx, %eax
    add $4, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
