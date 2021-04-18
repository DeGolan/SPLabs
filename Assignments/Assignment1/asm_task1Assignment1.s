section	.rodata			        ; we define (global) read-only variables in .rodata section
	format_string: db "%d", 10,0	; format string

section .data			        ; we define (global) uninitialized variables in .bss section
	an: dd -1 		        ; enough to store integer in [-2,147,483,648 (-2^31) : 2,147,483,647 (2^31-1)]          		; an is a local variable of size double-word, we use it to count the string characters


section .text                           ; we write code in .text section
        global assFunc                  ; 'global' directive causes the function assFunc(...) to appear in global scope  
        extern printf   
        extern c_checkValidity		


assFunc:                        	; assFunc function definition - functions are defined as labels
        push ebp              		; save Base Pointer (bp) original value
        mov ebp, esp         		; use Base Pointer to access stack contents (assFunc(...) activation frame)
        pushad                   	; push all signficant registers onto stack (backup registers values)
        mov ecx, dword [ebp+8]		; get function argument on stack
        mov [an], ecx
					; now ecx register points to the input string
        push dword [ebp+8]
        call c_checkValidity
        cmp eax, 1
        mov ebx, [an]
        je Even

Odd:    shl ebx, 3
        jmp Rest

Even:   shl ebx, 2

Rest:
        
        push ebx			; call printf with 2 arguments -  
	push format_string	        ; pointer to str and pointer to format string
	call printf

	add esp, 12		        ; clean up stack after call
        popad                    	; restore all previously used registers
        mov esp, ebp			; free function activation frame
        pop ebp				; restore Base Pointer previous value (to returnt to the activation frame of main(...))
        				
        
        
