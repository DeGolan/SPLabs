%macro printString 1
    pushad
    push %1
    push format_result
    call printf
    add esp,8
    popad
%endmacro
%macro printNumber 1
    pushad
    push %1
    push format_number
    call printf
    add esp,8
    popad
%endmacro
%macro printChar 1
    pushad
    push %1
    push format_char
    call printf
    add esp,8
    popad
%endmacro
%macro getString 1
        pushad
        mov eax,80
        push eax
        call malloc
        add esp,4
        mov [%1],eax
        push dword [stdin] 
        push 80 
        push eax
        call fgets; get first number
        add esp, 12
        popad
%endmacro
%macro getOctalValue 3
    mov BL,byte[%1]
    cmp byte BL,'0'
    jl %2
    cmp byte BL,'7'
    jg %2
    sub byte BL,'0'
    mov [%3],BL 
%endmacro
%macro getOctalValue 2
    mov BL,byte[%1]
    cmp byte BL,'0'
    jl %2
    cmp byte BL,'7'
    jg %2
%endmacro

section	.rodata			; we define (global) read-only variables in .rodata section
	format_result: db "%s",0	; format string
    format_calc: db "%s",0	; format string
    format_number: db "%d",10,0	; format number
    format_char: db "%c",10,0	; format number
    calc: db "calc: " , 0
    bad_args: db "not enough args" , 0


section .bss

    input1: resb 4 ;pointer to entered number from user
    input2: resb 4 ;pointer to entered number from user
    operator: resb 4 ;pointer to entered operator from user
    size1: resb 4 ;digits number the first number
    size2: resb 4 ;digits number the second number
    max_size:resb 4;will be the size of the result string
    result_ptr:resb 4;will hold the pointer of the result
    result_size:resb 4;will hold the result str size
    temp1:resb 4
    temp2:resb 4
    digit1: resb 4;will hold the current digit of the first number
    digit2: resb 4;will hold the current digit of the second number
    resulDigit:resb 4;will hold the current digit of the result
    carry: resb 4;will hold the current carry
    capacity: resb 4; will hold max capacity of stack
    leading_zeroes: resb 4;counter of leading zeroes
    new_result_ptr: resb 4;will hold the shorther str result
    new_ptr_size:resb 4;will hold the new pointer size
    stack_size:resb 4;will hold the current number of items in the stack
    current:resb 4;will hold a pointer to the next operand
    stack:resb 4;will hold a pointer to the stack
    top:resb 4;will hold a pointer to the top of the stack



   
section .text
    align 16
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
    init:   
        mov dword[capacity],5
        mov dword[stack_size],0
        mov dword[top],-4

        mov ebp, esp
        mov eax,[ebp+4]
        cmp eax,2
        je getCapacity
        printString bad_args
        jmp initStack

    getCapacity:
        mov ecx,[ebp+8]
        mov ecx, [ecx+4]

        getOctalValue ecx,initStack,temp1

        getOctalValue ecx+1,singleDigitCapacity,temp2

        mov CL,byte[temp1]
        shl CL,3
        add [capacity],CL

        mov CL,byte[temp2]
        add [capacity],CL
        sub dword[capacity],5

        jmp initStack

    singleDigitCapacity:
        mov CL,byte[temp1]
        add [capacity],CL
        sub dword[capacity],5

    initStack:

        mov eax,[capacity]
        shl eax,2 ;4 bytes per pointer CHECKED-V
        pushad
        push eax
        call malloc
        add esp,4
        mov [stack],eax
        popad

        mov dword[top],-1
      
        ;push num1
        call printCalc
        call incTop
        getString current
        mov eax,[stack]
        mov ebx,[top]
        mov ecx,[current]
        mov dword[eax+ebx],ecx
        
        ;  ;debug
        ; mov ecx,[eax+ebx]
        ; printString ecx
        ; ;

        ;push num1
        call printCalc
        call incTop
        getString current
        mov eax,[stack]
        mov ebx,[top]
        mov ecx,[current]
        mov dword[eax+ebx],ecx
        
        ;  ;debug
        ; mov ecx,[eax+ebx]
        ; printString ecx
        
        ;operator
        call printCalc
        getString current
        mov eax,[current]
        mov dword[operator],eax
        ;  ;debug
        ; mov ecx,[operator]
        ; printString ecx
        ; ;

        ;pop num2
        mov eax,[stack]
        mov ebx,[top]
        mov ecx,[eax+ebx]
        mov [input2],ecx
        ; ;debug
        ; mov edx,[input2]
        ; printString edx
        ; ;
        call decTop
        ;pop num2
        mov eax,[stack]
        mov ebx,[top]
        mov ecx,[eax+ebx]
        mov [input1],ecx
        ; ;debug
        ; mov edx,[input1]
        ; printString edx
        ; ;
        call decTop

        ; inc dword[top]
        ; mov edx,[top]
        ; call printCalc
        ; getString current
        ; mov eax,[stack]
        ; mov dword[eax+edx*4],current
        ; mov ebx,[eax+edx*4]
        ; printString ebx
       
        

; getUserInput:
;         call printCalc
;         getString current
;         getOctalValue current,opCheck
        
;         add dword[top],4
;         mov eax,[top]
;         ;printNumber eax
;         ;printString current

;         mov ecx,[stack]
;         mov dword[ecx+eax],current
;         ;mov ebx,[ecx,eax]
;         ;printString ebx
;         jmp getUserInput

;     opCheck:
;         ;mov ecx,dword[top]
;         ;printNumber ecx
;         ;mov edx,current
;         mov dword[operator],current
;         ;mov edx, [operator]
;         ;printString edx
;         ;cmp byte [operator],'+'
;         ;je sizeCheck1
;         ;cmp byte [operator],'&'
;         ;je sizeCheck1
;         mov eax,[top]
;         mov ecx,[stack]
;         mov ebx,[ecx+eax]
;         mov [input1],ebx
;         printString ebx

;         sub eax,1
;         mov ebx,[stack+eax]
;         mov [input2],ebx
;         ;printNumber eax


;         sub eax,1
;         mov [top],eax
        
        

;     ;print capacity
;         ;mov ebx,[capacity]
;         ;push ebx
;         ;push format_number
;         ;call printf
;         ;add esp,8 


;         ;cmp eax, 0

;     ;getFirstNum:   
;     ;    call printCalc
;     ;    getString input1
; ;
;     ;getSecondNum: 
;     ;    call printCalc
;     ;    getString input2
; ;
;     ;getOperator:
;     ;    call printCalc
;     ;    getString operator
;     ;    ;^^^^^ WILL CHANGE ^^^^^
;     ;    




        mov eax,0
        mov ebx,[input1]
    sizeCheck1:;check the digid number of number1 
        cmp byte [ebx+eax],0
        je cont1
        inc eax
        jmp sizeCheck1

    cont1:
        mov [size1],eax
        mov eax,0

        ;check the digid number of number2
    sizeCheck2:
        mov ebx,[input2]
        cmp byte [ebx+eax],0
        je cont2
        inc eax
        jmp sizeCheck2

    cont2:
        mov [size2],eax 
        mov eax,[size1]
        mov ebx,[size2]
        cmp eax,ebx ;put in max_size the longer number digit size
        ja sizeOneIsBigger
        mov [max_size],ebx
        jmp cont3

    sizeOneIsBigger:
        mov [max_size],eax
        mov [result_size],eax

    cont3:
        sub dword [size1],2
        sub dword [size2],2
        sub dword [max_size],1

        mov eax,[max_size]
        add eax,2
        push eax
        call malloc;make a new result str in max_size size
        add esp,4
        mov [result_ptr],eax

        mov eax,[max_size]
        add eax,1
        mov ebx,[result_ptr]
        mov byte[ebx+eax],0;put Null at the end of the string
        mov dword[carry],0;reset carry
        mov eax,[max_size]
     
     operation:
         cmp dword[max_size],-1;while(maxSize>=0)
         je loopEnd1    
        ;digit1 case
        cmp dword[size1],0
        jge digit1ToOct
        mov dword[digit1],0
        jmp digitTwo
    
    digit1ToOct:     
        mov eax,[size1]
        mov ecx,[input1]
        mov BL,[ecx+eax]
        sub byte BL,'0'
        mov [digit1],BL       
        ;digit2 case
    
     digitTwo:    
        cmp dword[size2],0
        jge digit2ToOct 
        mov dword[digit2],0
        jmp addOrAnd
    
    digit2ToOct:
        mov eax,[size2]
        mov ecx,[input2]
        mov BL,[ecx+eax]
        sub byte BL,'0'
        mov [digit2],BL
    
    addOrAnd:
        mov  eax,[operator]
        cmp byte [eax],'+'
        je addOP
        cmp byte [eax],'&'
        je andOP
        ;resultDigit=digit1+digit2+carry;  
    addOP:
        
        mov eax, [digit1]
        mov ebx,[digit2]
        add eax,ebx
        mov ebx,[carry]
        add eax,ebx 
        mov [resulDigit],eax
        jmp carryHandle
    
     andOP:
        ;resultDigit=digit1&digit2;
        mov eax,[digit1]
        mov ebx,[digit2]
        AND eax,ebx
        mov [resulDigit],eax  
    
     carryHandle:
        cmp dword[resulDigit],7
        jle noCarry
        sub dword[resulDigit],8
        mov dword[carry],1
        jmp makeResult  
    
    noCarry:
        mov dword[carry],0
    
    makeResult:
        mov eax,[result_ptr]
        mov BL, byte[resulDigit]
        add BL,'0'
        mov ecx,[max_size]
        mov byte[eax+ecx],BL
        sub dword[max_size],1
        sub dword[size1],1
        sub dword[size2],1
        jmp operation
    
     loopEnd1:

    
        ;remove leading zeroes
        mov dword[leading_zeroes],0
        mov eax,[leading_zeroes]
        mov ebx, [result_ptr]

    leadingZeros: 
        cmp byte [ebx+eax],'0'
        jne toShortherStr
        inc eax
        jmp leadingZeros

    toShortherStr:

        mov dword[leading_zeroes],eax
        mov eax,dword[leading_zeroes]
        cmp eax,0
        jle result
        mov eax,[result_size]
        add eax,2
        mov ebx,[leading_zeroes]
        sub eax,ebx;eax=resultSize+1-leadingZeroes
       
        mov [new_ptr_size],eax
        
        cmp dword[new_ptr_size],0
        jne makeShorter
        
        pushad
        mov ecx,2
        push ecx
        call malloc;make a new result str in max_size size
        add esp,4
        mov [new_result_ptr],eax
        
        mov byte[eax],'0'
        mov byte[eax+1],0
        popad
        ;free old result
        call freePointer
        mov eax,[new_result_ptr]
        mov [result_ptr],eax  
        jmp result

makeShorter:
        pushad
        inc dword[new_ptr_size]
        mov eax, [new_ptr_size]
        add eax,1
        push eax
        push 1
        call calloc
        add esp,8
        mov [new_result_ptr],eax
        popad

        mov esi,0
        mov ebx,[leading_zeroes]
        mov ecx,[new_result_ptr]
        mov edi,dword [new_ptr_size]
       
    shorther:
        cmp  esi,edi
        jge freeOld
        mov edx,[result_ptr]
        add edx,esi
        mov AL, byte [edx+ebx]
        mov byte[ecx+esi],AL
        inc esi
        jmp shorther

    freeOld:
        ;free old result
        call freePointer
        mov eax,[new_result_ptr]
        mov [result_ptr],eax

result:

        mov eax,[result_ptr]
        printString eax
        ;free result
         call freePointer
        
    end:
        ret

incTop:
    pushad
    cmp dword[top],-1
    je restTop
    add dword[top],4
    jmp finInc
restTop:
    mov dword[top],0
finInc:
    popad
    ret
    
decTop:
    pushad
    cmp dword[top],0
    jle finDec
    sub dword[top],4
finDec:
    popad
    ret
printCalc:
    pushad
    push calc 
    push format_calc
    call printf ;print calc:
    add esp, 8
    popad
    ret

freePointer:
    pushad
    mov eax,[result_ptr]
    push eax
    call free
    add esp,4
    popad
    ret
