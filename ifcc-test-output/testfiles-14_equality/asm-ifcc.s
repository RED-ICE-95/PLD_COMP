.globl main
 main: 
    push %rbp
    mov %rsp, %rbp
    movl $1, %eax
    sub $4, %rsp
    movl %eax, -4(%rbp)
    movl $2, %eax
    addl -4(%rbp), %eax
    add $4, %rsp
    sub $4, %rsp
    movl %eax, -4(%rbp)
    movl $3, %eax
    movl -4(%rbp), %ecx
    cmpl %eax, %ecx
    sete %al
    movzbl %al, %eax
    add $4, %rsp
    sub $4, %rsp
    movl %eax, -4(%rbp)
    movl $2, %eax
    sub $4, %rsp
    movl %eax, -8(%rbp)
    movl $3, %eax
    imull -8(%rbp), %eax
    add $4, %rsp
    sub $4, %rsp
    movl %eax, -8(%rbp)
    movl $7, %eax
    movl -8(%rbp), %ecx
    cmpl %eax, %ecx
    setne %al
    movzbl %al, %eax
    add $4, %rsp
    addl -4(%rbp), %eax
    add $4, %rsp
    mov %rbp, %rsp
    pop %rbp
    ret
