  .text
  .globl main
main:
  push R4
  mov R1, R4
  sub #4, R1
BB0:
  mov #42, -8(R4)
  mov -8(R4), R15
  mov R15, -4(R4)
  mov -4(R4), R15
  mov R4, R1
  pop R4
  ret
