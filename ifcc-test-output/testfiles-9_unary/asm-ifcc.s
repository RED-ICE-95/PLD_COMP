.globl main
 main: 
    push %rbp
    mov %rsp, %rbp
    movl $0, %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    sub $4, %rsp
    movl %eax, -4(%rbp)
    movl $1, %eax
    cmpl $0, %eax
    sete %al
    movzbl %al, %eax
    addl -4(%rbp), %eax
    add $4, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
