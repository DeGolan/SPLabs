%macro errorPrint 1
    push %1
    push format_string
    call printf
    add esp,8
%endmacro

%macro printNumber 1
    push %1
    push format_string
    call printf
    add esp,8
%endmacro

%macro inputCopy 2
    mov esi, 0; position
 %%start_copy:
    
    cmp byte [%1+esi], 0x00 ;null-terminator
    je %%input_copy_end
    mov byte DL, [%1+esi]
    mov byte [%2+esi], DL
    inc esi
    jmp %%start_copy
 %%input_copy_end:
    mov byte [%2+esi], 0

%endmacro

section	.rodata			; we define (global) read-only variables in .rodata section
	format_string: db "%s",0	; format string
    format_number: db "%d",0	; format number
    calc_str : db "calc:" , 0
    overflow_str: db "Error:Operand Stack overflow", 10,0
    plus_str: db "Error: Insufficient Number of Arguments on Stack", 10,0 
section .bss
    capacity : resb 4 ; operand stack's capacity
    tmp: resb 4
    operands: resb  20 ; operands stack (5 pointers to numbers as default)
    stksize: resb 4
    input: resb 80 ; input buffer (80 max size)
    backup_input: resb 80
    head_backup: resb 4
    ltmp: resb 4
    ptmp: resb 4

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
free_list:
    cmp dword [eax+3], 0
    je done_free
    mov ecx, eax ; tmp
    mov eax, [eax+3]
    push ecx
    mov [tmp], eax
    call free
    add esp,4
    mov eax,[tmp]
    jmp free_list
    done_free:
        push eax
        call free
   check3:     add esp, 4
    ret 
plus: ;edx is the larger, eax smaller
    mov bx, [edx]
    adc bx, [eax]
    mov [edx], bx
    mov cl, [edx+2]
    adc cl, [eax+2]
    mov [edx+2], cl
    cmp dword [eax+3], 0
    je done_with_smaller
    mov eax, [eax+3]
    jmp plus

    done_with_smaller:
        mov edx, [edx+3]
       loop_larger:
        mov bx, [edx]
        adc bx,0
        mov [edx],bx
        mov cl, [edx+2]
        adc cl,0
        mov [edx+2], cl
        cmp dword [edx+3], 0 
        je done_with_larger
        mov edx, [edx+3]
        jmp loop_larger

    done_with_larger:
        ret ;edx is the result

new_length: 
    mov ebx,[ltmp] ;current length
    main_length_loop:
        cmp ebx, 1
        je do_nothing
        cmp byte [eax], '0'
        jne do_nothing
        dec ebx
        inc eax
        jmp main_length_loop

    do_nothing:
        mov edx, [head_backup]
        mov [edx+7], ebx ;new length
        ret


unconvert:
    mov ebx , [edx+7] ;length
    mov [ltmp], ebx
    mov ecx, edx ;list pointer
    mov [head_backup], ecx
    add ebx, 2
    pushad
    push ebx
    push dword 1
    call calloc
    add esp,8
    mov [tmp], eax
    popad
    mov eax, [tmp]
    sub ebx, 3
    add eax,ebx
    mov byte [eax+1], 10
    mov byte [eax+2], 0
    add ebx,1
    mov edx, ebx; length
    mov edi, 1 ; link counter
    mov BL, 1; digit counter           edx=length, ecx = pointer to link, edi = link counter , eax= pointer to buffer, BL = digit counter

    main_unconvert_loop:
        cmp edx, 0
        je end_main_unconvert_loop

        unconvert_inner_loop:
            cmp BL, 4
            ja end_unconvert_inner_loop
            mov esi, edi
            AND esi, [ecx] 
            cmp esi, edi
            jne no_or
            add byte [eax], BL

        no_or:
            shl edi,1
            shl BL, 1

            cmp edi, 0x1000000
            jne unconvert_inner_loop
            mov ecx, [ecx+3]
            mov edi, 1
            jmp unconvert_inner_loop
        
        end_unconvert_inner_loop:
            mov BL, 1
            add byte [eax], '0'
            sub eax, 1
            sub edx, 1
            jmp main_unconvert_loop
    end_main_unconvert_loop:
        add eax, 1
        mov [ptmp], eax
        call new_length
        ret
        
    
bitwise: ; edx is pushed back to the stack as result, and eax is freed
    mov [tmp], edx ; head's backup
    mov [head_backup], eax ; the longer one
    bitwise_main_loop:
        mov bx, [edx]
        AND bx, [eax]
        mov [edx], bx
        add edx, 2
        add eax, 2
        mov bl, [edx]
        AND bl, [eax]
        mov [edx], bl
        cmp dword [edx+1], 0
        je bitwise_main_loop_end
        mov edx, [edx+1]
        mov eax, [eax+1]
        jmp bitwise_main_loop
    bitwise_main_loop_end:
        mov eax,[head_backup]
        ;FREE(eax)
        ret


print_string:
        call unconvert
        pushad
        printNumber eax
        popad
        pushad
        mov eax,[tmp]
        push eax
        call free
        add esp, 4
        popad

        ret        

 

list_copy:
    pushad
    mov ecx, 11
    push ecx
    push dword 1
    call calloc
    add esp,8
    mov [head_backup], eax
    popad
    mov eax, [head_backup] ;new head
    mov ebx, [edx+7] ; number's length
    mov [eax+7], ebx
    list_copy_loop:
        cmp dword [edx+3], 0
        je tail
        mov ebx, [edx]
        mov [eax], ebx ;data and garbage
        pushad
        mov ecx,7
        push ecx
        push dword 1
        call calloc
        add esp, 4
        mov [tmp], eax; new link
        popad
        mov ebx, [tmp]
        mov [eax+3], ebx
        mov eax, ebx
        mov edx, [edx+3]
        jmp list_copy_loop

    tail:
        mov dword ebx, [edx]
        mov [eax], ebx
        mov dword [eax+3],0
        ret


convertor: 
    mov ebx, 0; pointer to next char in input
    start:
        mov byte DL , [input+ebx]
        cmp byte DL, 0x00 
        je end_convertor
        cmp byte DL, 10
        je if_NL
        cmp DL, '7' 
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
    mov ecx, 11 ; size of the link 3bytes - data , 4bytes - pointer , 4byte-number length
    pushad
    push ecx
    push dword 1
    call calloc ; EAX = pointer to the new link
    add esp, 8
    mov [head_backup], eax ; backup for the head pointer
    mov [tmp], eax
    popad
    mov eax, [tmp]
    mov dword [eax+7], edx ;length of the input,no more than 80digits
    add dword [eax+7], 1

    mov dword edi, 1 ; LINK COUNTER
    mov byte BH, 1 ; DIGIT COUNTER

    maker_loop: 

        inner_while: cmp byte BH, 4;(2^3)
                     ja end_inner_while
                     mov byte CL, BH ; the AND result is in CL
                     AND byte CL, [input+edx]    
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
                     push dword 1
                     call calloc; eax points to the next link
                     add esp,8
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

  calc_print:   
  push calc_str ;print calc
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
  cmp byte [input], '7'
  ja operation

  call convertor 
 
  mov ecx, [capacity]
  cmp dword [stksize], ecx
  jb cont
  errorPrint overflow_str
  jmp myCalc

 cont:
  call link_maker
  mov eax, [head_backup]
  mov ebx, [stksize]  
  mov ecx, operands
  mov [ecx+4*ebx], eax
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
    je pop_and_print
    cmp byte [input], 'd'
    je duplicate
    cmp byte [input], '&'
    je bitwise_and
    cmp byte [input], 'n'
    ;je nob

    jmp myCalc
    

  bitwise_and:
    cmp byte [stksize], 2
    jae no_and_err
    errorPrint plus_str
    jmp myCalc
    no_and_err: 
        mov ebx, [stksize]
        mov ecx, operands
        mov edx, [ecx+4*ebx-4] ;edx is first num
        mov eax, [ecx+4*ebx-8] ;eax is second num
        mov dword [ecx+4*ebx-4], 0
        mov dword [ecx+4*ebx-8], 0
        sub ebx,2
        mov [stksize],ebx
        mov ebx, [edx+7] ;first num length
        cmp ebx, [eax+7] ;comparing the lengths
        jle first
        mov ecx, edx ;swapping
        mov edx, eax
        mov eax, ecx
        first:
            pushad
            call bitwise
            popad
        mov edx, [tmp] ; result
        call free_list ; free eax
        call unconvert
        mov ebx, [stksize]  
        mov ecx, operands
        mov [ecx+4*ebx], edx
        inc ebx
        mov [stksize], ebx
        pushad
        push dword [ptmp]
        call free ;free the buffer for calc the new length
        add esp, 4
        popad

       jmp myCalc

    
  plus_operator:
    cmp byte [stksize], 2
    jae no_plus_err
    errorPrint plus_str
    jmp myCalc
    no_plus_err: 
        mov ebx, [stksize]
        mov ecx, operands
        mov edx, [ecx+4*ebx-4] ;edx is first num
        mov eax, [ecx+4*ebx-8] ;eax is second num
        mov dword [ecx+4*ebx-4], 0
        mov dword [ecx+4*ebx-8], 0
        sub ebx,2
        mov [stksize],ebx
        mov ebx, [edx+7] ;first num length
        cmp ebx, [eax+7] ;comparing the lengths
        jge first_plus
        mov ecx, edx ;swapping
        mov edx, eax
        mov eax, ecx
        first_plus:
            inc dword [edx+7] ;inc the length
            ;add one link to edx
            mov [head_backup], edx ; backing the head
            finding_tail:
                cmp dword [edx+3], 0
                je found_tail
                mov edx, [edx+3] ; next link
                jmp finding_tail
            found_tail: ; calloc time
                push dword 7
                push dword 1
                mov [tmp], edx
                mov [ptmp], eax
                call calloc
                add esp, 8
                mov edx, [tmp]
                mov [edx+3], eax
                mov eax, [ptmp]
                mov edx, [head_backup]

            pushad
            call plus
            popad
            pushad
                  call free_list
        popad
        call unconvert
        mov ebx, [stksize]  
        mov ecx, operands
        mov [ecx+4*ebx], edx
        inc ebx
        mov [stksize], ebx
        pushad
        push dword [ptmp]
        call free ;free the buffer for calc the new length
        add esp, 4
        popad

       jmp myCalc



    duplicate:
    mov ecx, [capacity]
    cmp dword [stksize], ecx
    jb no_dup_err
        errorPrint overflow_str        
    jmp myCalc

    no_dup_err:
        mov ebx, [stksize]
        mov ecx, operands
        mov edx, [ecx+4*ebx-4] ;edx is pointer which needed to be copied
        pushad
        call list_copy
        popad
        mov eax, [head_backup]
        mov [ecx+4*ebx], eax
        inc ebx
        mov [stksize], ebx
        jmp myCalc
          

    pop_and_print:
    cmp dword [stksize], 1
        jae no_pop_err
        jmp myCalc
        no_pop_err:
            mov ebx, [stksize]
            mov ecx, operands
            mov edx, [ecx+4*ebx-4] ;pointer to the number in the head of the stack
            pushad
            call print_string
         check4:   popad
            mov eax, edx
            pushad
            call free_list
            popad
            mov dword [ecx+4*ebx-4],0
            dec dword [stksize]
            jmp myCalc