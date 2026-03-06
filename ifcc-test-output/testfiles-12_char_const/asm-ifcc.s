.globl main
 main: 
    push %rbp
    mov %rsp, %rbp
    movl $90, %eax
    sub $4, %rsp
    movl %eax, -4(%rbp)
    movl $65, %eax
    movl -4(%rbp), %ecx
    subl %eax, %ecx
    movl %ecx, %eax
    add $4, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
