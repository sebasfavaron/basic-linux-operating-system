GLOBAL invalid_opcode
GLOBAL overflow

invalid_opcode:
	push rbp
	mov rbp,rsp

	UD2

	mov rsp,rbp
	pop rbp
	ret

overflow:
	push rbp
	mov rbp,rsp

	int 4

	mov rsp,rbp
	pop rbp
	ret
