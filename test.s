global _start

_start:
	push ebp
	mov ebp, esp

	xor edx, edx
	mov dh, 0x0a	; \n
	shl edx, 8
	mov dx, 0x7265	; re
	push edx
	push 0x6767696e	; ggin
	push 0x20707520	;  pu 
	push 0x74756853	; tuhS

	xor eax, eax
	mov al, 0x04
	xor ebx, ebx
	inc ebx
	mov ecx, esp
	xor edx, edx
	mov dl, 0xf

	int 80h

	mov esp, ebp
	pop ebp

	ret
