section .text
global _start
 _start:

push ecx 
push edx 
push eax 
push ebx 
mov ecx, old_data
mov edx, 12
; Tamanho de inteiro é constante(10 para o número, 1 para o sinal e 1 para o '/n')
call escrever_function_modificado
mov esi, old_data ; Guarda o valor de leitura da string em esi
call converte_ascii_inteiro
mov dword [old_data], eax 
pop ebx
pop eax
pop edx
pop ecx

mov eax, [old_data]
l1: mov ebx, dword [dois] 
idiv ebx
push eax 
pop dword [new_data]

mov ebx, dword [dois] 
imul ebx 
jo overflow
push eax 
pop dword [tmp_data]

mov eax, [old_data]
sub eax, dword [tmp_data]
push eax 
pop dword [tmp_data]

push eax
push ebx
push ecx
push edx
mov dword eax, [tmp_data]
call converte_int_para_string
pop edx
pop ecx
pop ebx
pop eax

push eax
push ebx
mov eax, [new_data]
mov [old_data], eax
pop ebx
pop eax

mov eax, [old_data]
cmp eax, 0
jg l1
jmp sair
sair: 
	mov eax, 1
	mov ebx, 0
	int 80h

converte_ascii_inteiro:	
	;Frame de pilha
	push ebp
	mov ebp, esp
	;Converte para INT

	
	dec eax; tira o '/n'
	mov dword [inteiro_tamanho], eax  ; Guarda o tamanho da string

	mov edi, 10 ; Guarda em edi o valor de 10, que multiplicaremos elevado a posiÃ§Ã£o do nÃºmero
	mov ecx, [inteiro_tamanho] ; Guarda o tamanho do nÃºmero em ecx
	xor eax, eax ; Zera eax
	xor ebx, ebx ; Zera ebx

	mov bl, [esi] ; Pega um char da string
	cmp bl, '-' ; Verifica se char == '-'
	jne checagem_de_sinal_num1 ; Se nÃ£o for, checamos se Ã© '+'
	inc eax ; eax = 1
	inc esi
	dec ecx
	jmp parse_sinal_num1
checagem_de_sinal_num1: 
	cmp bl, '+'; Se nÃ£o for tambÃ©m, assumimos que Ã© positivo
	jne parse_sinal_num1
	inc esi
	dec ecx
parse_sinal_num1: 
	push eax ; Guardamos o eax com o valor do endereÃ§o 
	xor eax, eax ; Removemos o valor 1, para evitar problemas na escrita
lop_num1_int: 
	mov bl, [esi] ; Movemos o char seguinte, subtraÃ­mos 0x30 ('0')
	sub bl, '0'
	mul edi; eax = eax * 10 ; Multiplicamos a posiÃ§Ã£o por 10
	mov bh, 0 ; Limpamos a parte superior de bh, para evitar problemas na soma
	add eax, ebx ; Somamos ao resultado total
	inc esi ; AvanÃ§amos em uma posiÃ§Ã£o no indice em chars
	loop lop_num1_int ; Faremos isso atÃ© o esi ser 0
	
	pop ebx
	cmp ebx, 1
	jne fim_parse_num1
	neg eax

	fim_parse_num1:
	pop ebp
	ret
	
escrever_function_modificado:
	; NÃƒÂ£o guarda o eax pois ele serÃƒÂ¡ utilizado em seguida
	;Frame de pilha
	push ebp
	mov ebp, esp
	;VariÃƒÂ¡veis locais ebx
	push ebx
	mov ebx, 2
	mov eax, 3
	int 80h
	pop ebx
	pop ebp
	ret
	
	
converte_int_para_string:
  ;Frame de pilha
  push ebp
  mov ebp, esp
  mov esi, response_saida; Agora colocamos esi para apontar para o endereÃƒÂ§o do nosso novo inteiro 
  mov ebx, 10 ; ebx novamente recebe 10, pois iremos fazer a desconversÃƒÂ£o
  xor ecx, ecx ; Zeramos ecx
  cmp eax, 0 ; Comparamos eax com 0
  jge pula_negacao; se eax >=0, nÃƒÂ£o precisamos fazer a negaÃƒÂ§ÃƒÂ£o
  neg eax; caso contrÃƒÂ¡rio, negamos o valor eax
  push dword 1; lanÃƒÂ§amos o valor 1 na pilha pra lembrar que o nÃƒÂºmero ÃƒÂ© negativo
  jmp inicia_conversao ; Iniciamos a conversÃƒÂ£o
pula_negacao: 
push dword 0; Caso seja positivo, lembramos que o nÃƒÂºmero ÃƒÂ© positivo
inicia_conversao:
  xor edx, edx ; Zeramos edx
  div ebx 
  add dl, '0'; somamos '0'
  push edx; edx entra na pilha
  inc ecx; aumentamos uma posiÃƒÂ§ÃƒÂ£o
  cmp eax, 0
  jne inicia_conversao ; Enquanto eax nÃƒÂ£o for 0, refazemos o loop
  mov eax, ecx
  inc eax
  inc esi
  ; Escreve o valor no endereÃƒÂ§o de esi
lp2: 
  pop edx
  mov [esi], dl
  inc esi; 
  loop lp2
  pop edx
  cmp edx, 1
  jne positivo
  ;pop ecx;
  mov esi, response_saida
  mov [esi], byte '-'
  jmp fim
positivo: mov esi, response_saida
  mov [esi], byte '+'
fim:
mov [response_size], eax

mov edx, dword [response_size]
mov ecx, response_saida
call print_function
pop ebp
ret

print_function:
	;Frame de pilha
	push ebp
	mov ebp, esp
	;VariÃ¡veis locais eax, ebx
	push eax
	push ebx
	mov eax, 4
	mov ebx, 1
	int 80h
	pop ebx
	pop eax
	pop ebp
	ret
	
escrever_function:
	;Frame de pilha
	push ebp
	mov ebp, esp
	;VariÃ¡veis locais eax, ebx
	push eax
	push ebx
	mov eax, 3
	mov ebx, 0
	int 80h
	pop ebx
	pop eax
	pop ebp
	ret
	
overflow:
	mov ecx, mensagemOverflow
	mov edx, 26
	call print_function
	jmp sair
section .data
mensagemOverflow: db "Overflow em multiplicacao!", 26

dois: dd 2
section .bss
response_saida resb 30
response_size resd 2
inteiro_tamanho resd 1
inteiro_string resb 30

old_data: resd 1
new_data: resd 1
tmp_data: resd 1
