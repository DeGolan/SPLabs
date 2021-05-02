section .data
 str: db "Hello, Infected File",10,0
section .text
global code_start
global code_end
global infector
global infection
global _start
global system_call
extern main
_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc

    call    main        ; int main( int argc, char *argv[], char *envp[] )

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop
        
system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller

code_start:
infection:
    push    ebp             ; Save caller state
    mov     ebp, esp
    pushad                  ; Save some more caller state
    mov dword ebx, str
    push  22
    push  str
    push  1
    push  4

    call system_call

    add esp, 16
    popad
    mov esp,ebp
    pop ebp
    ret


infector:
    push    ebp             ; Save caller state
    mov     ebp, esp
    pushad                  ; Save some more caller state
    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost... 
    push  0777
    push  1025
    push  eax
    push 5

    call system_call ;open for append
    add esp, 16

    mov ecx,eax  ;eax=fd that returns from open
    mov ebx, code_end
    sub ebx, code_start
    push ebx
    mov ebx, code_start
    push ebx
    push eax
    push 4

    call system_call; append to output file
    add esp, 16

    push ecx
    push 6

    call system_call ;close fd
    add esp,8
    popad
    mov esp,ebp
    pop ebp
    ret

code_end:


    




    

