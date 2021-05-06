%macro errorPrint 1
    push %1
    push format_string
    call printf
    add esp,8
%endmacro

section	.rodata			; we define (global) read-only variables in .rodata section
	format_string: db "%s",0	; format string
    format_number: db "%d",0	; format number
    calc_str : db "calc:" , 0
    overflow_str: db "Error:Operand Stack overflow", 10,0
    plus_str: db "Error: Insufficient Number of Arguments on Stack", 10,0 
    ;check: db "+", 0
section .bss
    capacity : resb 4 ; operand stack's capacity
    tmp: resb 4
    operands: resb  20 ; operands stack (5 pointers to numbers as default)
    stksize: resb 4
    input: resb 80 ; input buffer (80 max size)
    head_backup: resb 4

section .text
  align 16
  global main
  extern printf
  extern fprintf 
  extern fflush
  extern malloc 
  extern calloc 
  extern free 
  extern gets 
  extern getchar 
  extern fgets 
  extern stdout
  extern stdin
  extern stderr
  global main


main:
    mov dword [stksize], 0
    mov dword [capacity], 5 ; default
    
    cmp dword [esp+4], 1
    je noArgs

    mov eax, [esp+8]
    mov eax, [eax+4] ; FIRST ARGUMENT = stack size in octal
  

  noArgs:  call myCalc
           ret

;Procedures:

convertor: 
    mov ebx, 0; pointer to next char in input
    start:
        mov byte DL , [input+ebx]
        cmp byte DL, 0x00 
        je end_convertor
        cmp byte DL, 10
        je if_NL
        cmp DL, '9' 
        ja end
        cmp DL, '0'
        jb end

        sub DL, '0' ; for real dec value
        mov byte [input+ebx], DL
        if_NL:
            add ebx,1
            jmp start

end_convertor:  
          sub dword ebx, 2  
          mov edx, ebx ;input+edx = the address of the last char in the input, EDX+1 = length of the string
          end:  ret


link_maker:
    mov ecx, 7 ; size of the link
    pushad
    push ecx
    call malloc ; EAX = pointer to the new link
    add esp, 4
    mov [head_backup], eax ; backup for the head pointer
    mov [tmp], eax
    popad
    mov eax, [tmp]
    mov dword edi, 1 ; LINK COUNTER
    mov byte BH, 1 ; DIGIT COUNTER

    maker_loop: 

        inner_while: cmp byte BH, 4;(2^3)
                     ja end_inner_while
                     mov byte CL, BH ; the AND result is in CL
                     AND byte CL, [input+edx] ; BH =CL=  00000010       00000111     
                     cmp byte CL, BH
                     jne nothing
                     add dword [eax], edi ; OR operation 
                     
        
        nothing:     shl BH, 1
                     shl edi, 1

                     cmp dword edi, 0x1000000 ; its 2^24 the last bit in the data link
                     jne inner_while
                     cmp edx, 0 ; last iteration no need for new malloc
                     jne rest
                     cmp byte BH, 8
                     jb  rest
                     jmp end_maker_loop
                     
         rest:       mov dword edi, 1
                     mov esi, eax ; tmp pointer
                     mov ecx, 7
                     
                     pushad
                     push ecx
                     call malloc; eax points to the next link
                     add esp,4
                     mov [tmp], eax ; backup
                     popad
                     mov eax, [tmp] ; restore
                     mov [esi+3], eax; curr->next = newPointer
                     
                     
                     jmp inner_while

        end_inner_while:
                     mov byte BH, 1; reseting the counter
    

    end_maker_loop: 
                    sub edx, 1
                    cmp edx, 0xFFFFFFFF 
                    je end_link_maker
                    jmp maker_loop
end_link_maker: 
                mov dword [eax+3], 0 ; null-pointer
                ret





myCalc: ; main loop 

  calc_print:   push calc_str ;print calc
                push format_string
                call printf
                add esp, 8
    
                push dword [stdin] ;geting input
                push 80 
                push input
                call fgets
                add esp, 12


  

  cmp byte [input], '0'
  jb operation
  cmp byte [input], '9'
  ja operation

  call convertor 
 
  mov ecx, [capacity]
  cmp dword [stksize], ecx
  jb cont
  errorPrint overflow_str
  jmp myCalc

 cont:
  call link_maker
  break:mov eax, [head_backup]
  mov ebx, [stksize]  
  mov [operands+4*ebx], eax
  mov eax, [operands+4*ebx]
  inc ebx
  mov dword [stksize], ebx


 jmp myCalc

end_main: ret ; TODO num of calcs


operation: 
    
    cmp byte [input], 10 ;no input was entered
    je myCalc
    cmp byte [input], 'q' ;termination
    je end_main 
    cmp byte [input], '+'
    je plus_operator
    cmp byte [input], 'p'
    ;je pop_and_print
    cmp byte [input], 'd'
    ;je duplicate
    cmp byte [input], '&'
    ;je bitwise_and
    cmp byte [input], 'n'
    ;je nob

    jmp myCalc
    

plus_operator:
    cmp byte [stksize], 2
    jae no_plus_err
    errorPrint plus_str
no_plus_err: jmp myCalc


pop_and_print:
    cmp byte [stksize], 1
    jae no_pop_err
    jmp myCalc
    no_pop_err:
        mov ebx, [stksize]
        mov edx, [operands+4*ebx-1] ;pointer to the number in the head of the stack
        
