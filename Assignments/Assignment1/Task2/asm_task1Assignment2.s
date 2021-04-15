section	.rodata			; we define (global) read-only variables in .rodata section
	format_string: db "%s",10, 0	; format string

section .bss			; we define (global) uninitialized variables in .bss section
	an: resb 100		; enough to store integer in [-2,147,483,648 (-2^31) : 2,147,483,647 (2^31-1)]
	

section .text
	global convertor
	extern printf

convertor:
	push ebp
	mov ebp, esp	
	pushad			
	mov ecx, dword [ebp+8]	; get function argument (pointer to string)
	;add ecx,1
	;mov eax,[ecx]
	;mov [an],al

	;your code comes here...
	
	mov ebx,0 ;main counter j
	mov edx,0 ;bufLoc

mainLoop:
	mov eax,[ecx]
	cmp al,0	;buf[j] != 0
	je rest
	cmp al,10	;buf[j] != 0
	je rest

	mov esi,edx		;int i=bufLoc
	add edx,4 
zeroLoop:
	cmp esi,edx		;i<bufLoc +4
	jge digit
	mov byte [an+esi],'0'
	inc esi 	;i++
	jmp zeroLoop

digit:
	cmp al,'9'
	jg letter
	sub al, '0'
	jmp cont

letter:
	sub al, 55
cont:
	mov esi,edx
	
convert:
	cmp al,0
	jbe endOfLoop
	mov edi,0
	shr al,1
	adc edi,0
	sub dl,1
	cmp edi,0
	je even
	mov byte[an+edx],'1'
	jmp cont1
even:
	mov byte[an+edx],'0'
cont1:
	
	
	jmp convert

endOfLoop:
	mov edx,esi
	inc ecx 
	inc ebx ;j++
	jmp mainLoop

rest:
	mov byte [an+4*ebx],0
	push an			; call printf with 2 arguments -  
	push format_string	; pointer to str and pointer to format string
	call printf
	add esp, 8		; clean up stack after call

	popad			
	mov esp, ebp	
	pop ebp
	ret