.globl main
 main: 
    push %rbp
    mov %rsp, %rbp
    movl $17, %eax
    sub $4, %rsp
    movl %eax, -4(%rbp)
    movl $5, %eax
    movl %eax, %ecx
    movl -4(%rbp), %eax
    cdq
    idivl %ecx
    add $4, %rsp
    sub $4, %rsp
    movl %eax, -4(%rbp)
    movl $17, %eax
    sub $4, %rsp
    movl %eax, -8(%rbp)
    movl $5, %eax
    movl %eax, %ecx
    movl -8(%rbp), %eax
    cdq
    idivl %ecx
    movl %edx, %eax
    add $4, %rsp
    addl -4(%rbp), %eax
    add $4, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
