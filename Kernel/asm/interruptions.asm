GLOBAL sti
GLOBAL cli

GLOBAL tickHandler
GLOBAL startSchedule
GLOBAL _yield
GLOBAL _testAndSet
GLOBAL irq0Handler
GLOBAL irq1Handler
GLOBAL setPicMaster
GLOBAL setPicSlave
GLOBAL int80Handler

%include "./asm/macros.m"

EXTERN irqDispatcher
EXTERN tickHandlerC
EXTERN schedule
EXTERN saveKernelStack
EXTERN getKernelStack
EXTERN getCurrentStack
EXTERN getCurrentEntryPoint
EXTERN sysCallHandler
EXTERN sendEOI
EXTERN runModule
EXTERN printProcesses

section .text

tickHandler:
	cli

	push rbp
	mov rbp, rsp

	call tickHandlerC

	mov rsp, rbp
	pop rbp

	pushState

	mov rdi, rsp

	call getKernelStack

	mov rsp, rax

	call schedule

	mov rsp, rax

	popState

	mov al, 20h
	out 20h, al

	sti
	iretq

startSchedule:
	mov rdi, rsp

	push rbp 
	mov rbp, rsp

	call saveKernelStack

	call getCurrentStack

	mov rbx, rax

	call getCurrentEntryPoint

	mov rsp, rbp
	pop rbp

	mov rsp, rbx
	mov [rsp], rax

	sti

	ret

_yield:
	mov rax, rsp

	push QWORD 0
	push QWORD 0
	push rax
	pushfq
	push QWORD 0x008
	push .ret

	pushState

	mov rdi, rsp

	call getKernelStack

	mov rsp, rax

	call schedule

	mov rsp, rax

    popState

    mov al, 20h
    out 20h, al

    iretq

.ret:
	ret

; returns 1 if lock acquired, else 0
_testAndSet:
    push rbp
    mov rbp, rsp

    xor edx, edx
    xor eax, eax
    xor ecx, ecx
    xor ebx, ebx

    inc ebx

    lock CMPXCHG8B [rdi]

    jnz .lockNotAcquired

    mov rax, 1

    mov rsp, rbp
    pop rbp
    ret
  
.lockNotAcquired:
    mov rax, 0

    mov rsp, rbp
    pop rbp
    ret

irq0Handler:
	irqHandler 0

irq1Handler:
	irqHandler 1

int80Handler:
	pushaq

	call sysCallHandler

	popaq

	iretq

sti:
	sti
	ret

cli:
	cli
	ret

setPicMaster:
	setPicMask 0x21

setPicSlave:
	setPicMask 0xA1
