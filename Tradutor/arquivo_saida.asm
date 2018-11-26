section .text
global _start
 _start:

push ecx 
push edx 
push eax 
push ebx 
mov ecx, a
mov edx, 12
; Tamanho de inteiro � constante(10 para o n�mero, 1 para o sinal e 1 para o '/n')
call escrever_function_modificado
mov esi, a ; Guarda o valor de leitura da string em esi
call converte_ascii_inteiro
mov dword [a], eax 
pop ebx
pop eax
pop edx
pop ecx

push eax
push ebx
push ecx
push edx
mov dword eax, [a]
call converte_int_para_string
pop edx
pop ecx
pop ebx
pop eax

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

	mov edi, 10 ; Guarda em edi o valor de 10, que multiplicaremos elevado a posição do número
	mov ecx, [inteiro_tamanho] ; Guarda o tamanho do número em ecx
	xor eax, eax ; Zera eax
	xor ebx, ebx ; Zera ebx

	mov bl, [esi] ; Pega um char da string
	cmp bl, '-' ; Verifica se char == '-'
	jne checagem_de_sinal_num1 ; Se não for, checamos se é '+'
	inc eax ; eax = 1
	inc esi
	dec ecx
	jmp parse_sinal_num1
checagem_de_sinal_num1: 
	cmp bl, '+'; Se não for também, assumimos que é positivo
	jne parse_sinal_num1
	inc esi
	dec ecx
parse_sinal_num1: 
	push eax ; Guardamos o eax com o valor do endereço 
	xor eax, eax ; Removemos o valor 1, para evitar problemas na escrita
lop_num1_int: 
	mov bl, [esi] ; Movemos o char seguinte, subtraímos 0x30 ('0')
	sub bl, '0'
	mul edi; eax = eax * 10 ; Multiplicamos a posição por 10
	mov bh, 0 ; Limpamos a parte superior de bh, para evitar problemas na soma
	add eax, ebx ; Somamos ao resultado total
	inc esi ; Avançamos em uma posição no indice em chars
	loop lop_num1_int ; Faremos isso até o esi ser 0
	
	pop ebx
	cmp ebx, 1
	jne fim_parse_num1
	neg eax

	fim_parse_num1:
	pop ebp
	ret
	
escrever_function_modificado:
	; NÃ£o guarda o eax pois ele serÃ¡ utilizado em seguida
	;Frame de pilha
	push ebp
	mov ebp, esp
	;VariÃ¡veis locais ebx
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
  mov esi, response_saida; Agora colocamos esi para apontar para o endereÃ§o do nosso novo inteiro 
  mov ebx, 10 ; ebx novamente recebe 10, pois iremos fazer a desconversÃ£o
  xor ecx, ecx ; Zeramos ecx
  cmp eax, 0 ; Comparamos eax com 0
  jge pula_negacao; se eax >=0, nÃ£o precisamos fazer a negaÃ§Ã£o
  neg eax; caso contrÃ¡rio, negamos o valor eax
  push dword 1; lanÃ§amos o valor 1 na pilha pra lembrar que o nÃºmero Ã© negativo
  jmp inicia_conversao ; Iniciamos a conversÃ£o
pula_negacao: 
push dword 0; Caso seja positivo, lembramos que o nÃºmero Ã© positivo
inicia_conversao:
  xor edx, edx ; Zeramos edx
  div ebx 
  add dl, '0'; somamos '0'
  push edx; edx entra na pilha
  inc ecx; aumentamos uma posiÃ§Ã£o
  cmp eax, 0
  jne inicia_conversao ; Enquanto eax nÃ£o for 0, refazemos o loop
  mov eax, ecx
  inc eax
  inc esi
  ; Escreve o valor no endereÃ§o de esi
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
	;Variáveis locais eax, ebx
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
	;Variáveis locais eax, ebx
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
section .bss
response_saida resb 30
response_size resd 2
inteiro_tamanho resd 1
inteiro_string resb 30

a: resd 1
section .data
mensagemOverflow: db "Overflow em multiplicacao!", 26

