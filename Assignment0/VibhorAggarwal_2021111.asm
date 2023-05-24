segment .data 
x	dd	0 
n	dd	0 
s	times 100 db 0 
fmt	db	"%d" , 0 
fmt1	db	"%s" , 0
fmt2	db 	"%d" , 0x0a, 0
fmt3	db 	"%s" , 0x0a, 0

segment .text
	global main 
	extern scanf, printf 
main : 
	push rbp 
	mov rbp , rsp
	lea	rdi , [fmt]
	lea	rsi, [x]  
	xor	eax , eax 
	call	scanf
	mov [n] , eax 
	lea	rdi , [fmt1]
	lea	rsi , [s]
	xor	eax , eax
	call	scanf
	lea	rdi , [fmt2] 
	mov	rsi , [x] 
	xor	eax , eax
	call printf
	lea	rdi , [fmt3] 
	lea	rsi , [s] 
	xor	eax , eax
	call printf
	leave
	ret
