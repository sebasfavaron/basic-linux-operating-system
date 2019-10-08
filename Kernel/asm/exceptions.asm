GLOBAL exception0Handler
GLOBAL exception4Handler
GLOBAL exception6Handler

%include "./asm/macros.m"

EXTERN exceptionDispatcher
EXTERN printRegisters
EXTERN sysExit

section .text

exception0Handler:
	exceptionHandler 0

exception4Handler:
  exceptionHandler 4

exception6Handler:
  exceptionHandler 6

section .bss

brip resb 8
brsp resb 8
brax resb 8
