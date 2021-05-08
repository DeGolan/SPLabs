section	.rodata	
    calc_string : db "calc:" , 0
    print_string_format: db "%s",10,0	; format string



section .bss
    cap : resb 4 ;stack capacity
    stck: resb 4 ;number of objects in stack
    input: resb 80 ;input buffer
    pointer: resb 4 ;will hold pointer to node


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

main:
    
    mov dword [stck], 0
    mov dword [cap], 5 ; default

    cmp dword [esp+4], 1 ;check if the user entered capacity
    je mainLoop

    mov eax, [esp+8] ;move args to eax
    mov eax, [eax+4] ;save first argument to eax
    mov dword[cap], eax ;save capacity
    
mainLoop:
    ;first thing in main loop is print "calc:" and get input
    printCalc:
        push calc_string ;print calc
        push print_string_format
        call printf
        add esp, 8
        push dword [stdin] ;geting input
        push 80 
        push input
        call fgets
        add esp, 12
        
        mov esi, 0; will point to the last digit of input
    inputSize:
        add esi,1
        mov byte AL,[input+esi]
        cmp byte AL,0
        jne inputSize
        sub esi,1 ; remove one to point at the last digit and not 0

    ;start creating list to input to stack
    initList:
        mov ecx, 7 ; node size (3 data, 4 next node)
        pushad
        push ecx
        call calloc ;eax will hold pointer to the new node
        add esp, 4
        mov [pointer],eax
        popad
        mov eax,[pointer]


        mov dword ebx, 1 ;bit index
        mov byte CH, 1  ;bit checker
    ;insert data to node    
    insertData:
        cmp byte CH,8 ;we read 3 bits
        je reset
        cmp esi,0 ;finshed to read number
        je  fin
        sub esi,1 ;move to next digit
    cont1:
        mov byte DL, CH
        mov CL,[input+esi]
        sub CL,'0'  ;convert from char to oct digit
        AND DL, CL ;check if the wanted bit is on
        cmp byte DL, CH  ;if so, add to the data the nubmer
        jne nextBit
        add dword [eax+ebx],ebx ;add to the data the number
        
    nextBit:
       
        shl CH,1
        shl ebx,1
        cmp dword ebx,16777216 ;if the 25'th bit is on
        jne insertData
        ;new link
        mov edx, eax; save the head
        mov ecx, 7            
        pushad
        push ecx
        call calloc; ;eax will hold pointer to the new node
        add esp,4
        mov [pointer],eax
        popad
        mov eax,[pointer] ;eax=curr
        mov [edx+3],eax   ;pointing to the new node


     reset:
        mov dword ebx, 1 ;bit index
        mov byte CH, 1  ;bit checker
        jmp insertData
      
    fin:
        mov dword [edx+3], 0 ;NULL

        ret

        



                                        

    





       






    