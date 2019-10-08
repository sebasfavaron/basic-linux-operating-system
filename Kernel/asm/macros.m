%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push fs
	push gs
%endmacro

%macro popState 0
	pop gs
	pop fs
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro pushaq 0
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popaq 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
%endmacro

%macro setPicMask 1
	push rbp
	mov rbp, rsp

	mov rax, rdi
	out %1, al

	mov rsp, rbp
	pop rbp
	ret
%endmacro

%macro exceptionHandler 1
	mov [brsp], rsp
	mov [brax], rax
	mov rax, [rsp]
	mov [brip], rax
	mov rax, [brax]

	push rax
	pushaq

	mov rdi, %1
	mov rsi, [brip]
	mov rdx, [brsp]
	call exceptionDispatcher

	mov rbx, 1
	mov rbp, [brsp]
	sub rbp, 8

	.loop:
		cmp rbx, 16
		je .end
		mov rdi, rbx
		mov rsi, [rbp]
		call printRegisters
		sub rbp, 8
		inc rbx
		jmp .loop

	.end:
		call sysExit
%endmacro


%macro irqHandler 1
	pushaq

	mov rdi, %1
	call irqDispatcher

	call sendEOI

	popaq

	iretq
%endmacro
