.globl main
 main: 
    push %rbp
    mov %rsp, %rbp
    movl $24, %eax
    sub $4, %rsp
    movl %eax, -4(%rbp)
    movl $4, %eax
    movl %eax, %ecx
    movl -4(%rbp), %eax
    cdq
    idivl %ecx
    add $4, %rsp
    sub $4, %rsp
    movl %eax, -4(%rbp)
    movl $2, %eax
    movl %eax, %ecx
    movl -4(%rbp), %eax
    cdq
    idivl %ecx
    add $4, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
