%macro pushToStack 1
    pushad
    call incTop
    mov eax,[stack]
    mov ebx,[top]
    mov ecx,[%1]
    mov dword[eax+ebx],ecx
    popad
%endmacro
%macro popFromStack 1
    pushad
    mov eax,[stack]
    mov ebx,[top]
    mov ecx,[eax+ebx]
    mov [%1],ecx
    call decTop
    popad
%endmacro
%macro freePointer 1
    pushad
    mov eax,[%1]
    push eax
    call free
    add esp,4
    popad
%endmacro
%macro printString 1
    pushad
    push %1
    push format_result
    call printf
    add esp,8
    popad
%endmacro
%macro printDebug 1
    pushad
    push %1
    push format_calc
    push dword[stderr] 
    call fprintf
    add esp,12
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
	format_result: db "%s",10,0	; format string
    format_calc: db "%s",0	; format string
    format_number: db "%d",10,0	; format number
    format_char: db "%c",10,0	; format number
    calc: db "calc: " , 0
    bad_args: db "not enough args" , 0
    err_overflow:db "Error: Operand Stack Overflow",0
    err_insufficient: db "Error: Insufficient Number of Arguments on Stack" ,0


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
    need_to_free:resb 4;flag to know if to free result_ptr
    temp_pointer:resb 4 ;use to print and free right after
    dup_pointer:resb 4;use to duplicate
    opertion_number:resb 4;use for count the opertion number
    helper1:resb 4;use for count the opertion number
    helper2:resb 4;use for count the opertion number
    debug_flag:resb 4;flag for debug

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
    mov dword[helper1],0
    mov dword[helper2],0
    mov dword[need_to_free],0
    mov dword[capacity],5
    mov dword[stack_size],0
    mov dword[top],-4
    mov dword [debug_flag],0
    mov ebp, esp
    mov esi,1
    call argsCheck
    call myCalc
    ret
    
popAndFree:       
    cmp dword[top],-1
    je end
    popFromStack input1
    freePointer input1
    jmp popAndFree
    
end:
    mov eax,[opertion_number]
    add eax,[helper2]

    printNumber eax
    freePointer current
    freePointer stack
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
    jle resetTop
    sub dword[top],4
    jmp finDec
resetTop:
    mov dword[top],-1
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

popAndPrint:
    pushad
    popFromStack temp_pointer
    mov eax,[temp_pointer]
    mov esi,0
whereIsEnter:
    cmp byte[eax+esi],10
    je removeEnter
    cmp byte[eax+esi],0
    je removeEnter
    inc esi
    jmp whereIsEnter

removeEnter:
    mov byte[eax+esi],0
    printString eax
    freePointer temp_pointer
    freePointer current
    popad
    jmp receivingInput

duplicate:
    freePointer current
    pushad
    popFromStack temp_pointer
    mov esi,0
    mov eax,[temp_pointer]
countSizeDup: 
    cmp byte[eax+esi],0
    je preWork
    inc esi
    jmp countSizeDup

preWork:
    pushad
    inc esi
    push esi
    call malloc
    add esp,4
    mov [dup_pointer],eax
    popad
    mov eax,[temp_pointer]
    mov ebx,[dup_pointer]
    mov esi,0

    dupWork:
        cmp byte[eax+esi],0
        je doneDup
        mov byte CL,[eax+esi]
        mov byte [ebx+esi],CL
        inc esi
        jmp dupWork
    doneDup:
        mov byte [ebx+esi],0
        cmp dword[debug_flag],0
        je dontPrintDupDebug
        printDebug ebx
    dontPrintDupDebug:
        pushToStack temp_pointer
        pushToStack dup_pointer    
        popad
        jmp receivingInput

bytesNumber:
    pushad
    popFromStack temp_pointer
    mov eax,[temp_pointer]
    mov ebx,0;number of digits
digitCount:
    cmp byte[eax+ebx],10
    je bytesCalc
    cmp byte[eax+ebx],0
    je bytesCalc
    inc ebx
    jmp digitCount

bytesCalc:

    ;*3
    mov ecx,ebx
    shl BL,1
    add ebx,ecx
    ;now we have the num of bits in ebx, we will remove the leading zeros
    mov eax,[temp_pointer]
    cmp byte[eax],'1'
    je removeTwo
    cmp byte[eax],'3'
    jg finReduce
    sub ebx,1
    jmp finReduce
removeTwo:
    sub ebx,2
finReduce:
    freePointer temp_pointer
    freePointer current
    ;/4
    shr BL,3
    cmp ebx,0
    jne roundUp
    add ebx,1
    jmp withoutCarry
  
roundUp:
    mov ecx,7
    and ecx,ebx
    cmp ecx,0
    je withoutCarry
    add ebx,1

withoutCarry:
    mov esi,0
    mov eax,ebx;we have in eax the bytes number as integer in dec basis
    mov edx,0
    mov ebx,8

divideByEight:
    div ebx
    push edx
    inc esi
    mov edx,0
    cmp eax,0
    jg divideByEight
    mov eax,esi
    add eax,2
    pushad
    push eax
    call malloc
    add esp,4
    mov [dup_pointer],eax
    popad
    mov eax,[dup_pointer]
    mov byte[eax+esi+1],0
    mov byte[eax+esi],10
    cmp esi,0
    mov ebx,0
    ;sub esi,1
    mov edi,esi
    mov esi,0
buildStr:
    cmp esi,edi
    je endOfByteCount
    pop ebx
    add ebx,'0'
    mov byte[eax+esi],BL
    inc esi
    jmp buildStr

endOfByteCount:
    mov eax,[dup_pointer]
    cmp dword[debug_flag],0
    je dontPrintNDebug
    printDebug eax

dontPrintNDebug:
    pushToStack dup_pointer
    popad
    jmp receivingInput

    argsCheck:
        cmp esi,[ebp+4]
        je initStack
        ;check if debug or cap
        mov ecx,esi
        inc esi
        shl ecx,2
        mov ebx,[ebp+8]
        mov ecx,[ebx+ecx]
        cmp byte [ecx],"-"
        jne getCapacity
        mov dword[debug_flag],1
        jmp argsCheck

    getCapacity:
        getOctalValue ecx,argsCheck,temp1
        getOctalValue ecx+1,singleDigitCapacity,temp2
        mov CL,byte[temp1]
        shl CL,3
        add [capacity],CL
        mov CL,byte[temp2]
        add [capacity],CL
        sub dword[capacity],5
        jmp argsCheck

    singleDigitCapacity:
        mov CL,byte[temp1]
        add [capacity],CL
        sub dword[capacity],5
        jmp argsCheck

    initStack:
        mov eax,[capacity]
        shl eax,2 ;4 bytes per pointer 
        pushad
        push eax
        call malloc
        add esp,4
        mov [stack],eax
        popad
        mov dword[top],-1
        ;change cap size to fit top*4
        mov eax,[capacity]
        shl eax,2
        sub eax,4
        mov [capacity],eax
        mov dword[opertion_number],0;init
        ret 

myCalc:
    receivingInput:
        call printCalc
        getString current
        mov eax,[current]
        cmp dword[debug_flag],0
        je dontPrintInputDebug
        printDebug eax

    dontPrintInputDebug:
        cmp byte [eax],'0'
        jl isOperator
        cmp byte [eax],'7'
        jg isOperator
        ;curr is a number
        mov eax,[capacity]
        ;check overflow
        cmp dword eax,[top]
        jne notFull
        printString err_overflow
        freePointer current
        jmp receivingInput
    notFull:
        pushToStack current
        jmp receivingInput

    isOperator:
        mov eax,[current]
        cmp byte[eax],'q'
        je popAndFree
        ;count num of ops
        pushad
        cmp dword[helper1],7
        je resetHelper1
        jmp countOpNum

resetHelper1:
        mov dword[helper1],-1
        add dword[helper2],2
countOpNum:
        inc dword[helper1]
        inc dword[opertion_number]
        mov eax,[opertion_number]
        popad
        cmp dword [top],-1
        jne notEmpty
        printString err_insufficient
        freePointer current
        jmp receivingInput
    notEmpty:
        cmp byte[eax],'p'
        je popAndPrint
        cmp byte[eax],'d'
        je duplicate
        cmp byte[eax],'n'
        je bytesNumber
        ;check if there are 2 elem at least at the stack
        cmp dword[top],4
        jge enoughOperands
        printString err_insufficient
        freePointer current
        jmp receivingInput

    enoughOperands:
        mov dword[operator],eax
        popFromStack input2
        popFromStack input1
        mov eax,0
        mov ebx,[input1]
    sizeCheck1:;check the digit number of number1 
        cmp byte [ebx+eax],0
        je cont1
        cmp byte [ebx+eax],10
        je cont1
        inc eax
        jmp sizeCheck1

    cont1:
        mov [size1],eax
        mov eax,0

        ;check the digit number of number2
    sizeCheck2:
        mov ebx,[input2]
        cmp byte [ebx+eax],0
        je cont2
        cmp byte [ebx+eax],10
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
        mov [result_size],ebx
        jmp cont3

    sizeOneIsBigger:
        mov [max_size],eax
        mov [result_size],eax
       
    cont3:
        sub dword [size1],1
        sub dword [size2],1
        cmp  dword [need_to_free],0
        je noNeedToFree
        freePointer result_ptr
        mov dword [need_to_free],1

    noNeedToFree:
        mov eax,[max_size]
        add eax,3;!!check if need to return to 2!!!
        push eax
        call malloc;make a new result str in max_size size
        add esp,4
        mov [result_ptr],eax
        mov eax,[max_size]
        add eax,1
        mov ebx,[result_ptr]
        mov byte[ebx+eax],0;put Null at the end of the string
        mov dword[carry],0;reset carry

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
        cmp dword[new_ptr_size],1
        jne makeShorter
        ;if the result is 0
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
        freePointer result_ptr
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
        freePointer result_ptr
        mov eax,[new_result_ptr]
        mov [result_ptr],eax
    result:
        pushToStack result_ptr
        mov eax,[result_ptr]
        cmp dword[debug_flag],0
        je dontPrintResultDebug
        printString eax

    dontPrintResultDebug:
        freePointer input1
        freePointer input2
        freePointer operator 
        jmp receivingInput
       