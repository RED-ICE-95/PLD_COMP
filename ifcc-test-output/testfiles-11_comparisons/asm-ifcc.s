.globl main
 main: 
    push %rbp
    mov %rsp, %rbp
    movl $3, %eax
    sub $4, %rsp
    movl %eax, -4(%rbp)
    movl $5, %eax
    movl -4(%rbp), %ecx
    cmpl %eax, %ecx
    setl %al
    movzbl %al, %eax
    add $4, %rsp
    sub $4, %rsp
    movl %eax, -4(%rbp)
    movl $5, %eax
    sub $4, %rsp
    movl %eax, -8(%rbp)
    movl $10, %eax
    movl -8(%rbp), %ecx
    cmpl %eax, %ecx
    setg %al
    movzbl %al, %eax
    add $4, %rsp
    addl -4(%rbp), %eax
    add $4, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
