#include "pch.h"
#include "ConversaoLib.h"


ConversaoLib::ConversaoLib()
{
	isSectionText = false;
}

ConversaoLib::~ConversaoLib()
{
}

void ConversaoLib::criarArquivoDeSaida()
{
	std::ofstream arquivoSaida = std::ofstream("arquivo_saida.asm");
	arquivoSaida << ConversaoLib::codigoConvertido.str();
	arquivoSaida.close();

}

// Verifica o tipo de instrução e redireciona para o método mais adequado(diretiva ou instrução)
// Feita apenas para facilitar o debug do projeto
void ConversaoLib::verificaEConverteOperacao(Tokenizador::TokensDaLinha tokenDaLinha)
{
	// Variável criada para verificar se a section data já foi criada, 
	// caso sim, o código deverá ignorar quando o .bss ou o .data for chamado novament

	TabelaLib tabelaLib;
	if (tabelaLib.isInstrucao(tokenDaLinha.operacao)) {
		// TODO: Verificar se tem Label, se sim, colocar na instrução

		
		std::cout << "Instrução \n";
		InfoDeInstrucoes infoInstrucao = tabelaLib.getInstrucao(tokenDaLinha.operacao);
		// Insere label na frente de instruções
		if (tokenDaLinha.label != "") {
			ConversaoLib::codigoConvertido << tokenDaLinha.label + ": ";
		}
		switch (infoInstrucao.opcodesInstrucoes)
		{	
		case ADD:
			ConversaoLib::codigoConvertido << ConversaoLib::converteAdd(tokenDaLinha.operando[0]);
			break;
		case SUB:
			ConversaoLib::codigoConvertido << ConversaoLib::converteSub(tokenDaLinha.operando[0]);
			break;
		case MULT:
			ConversaoLib::codigoConvertido << ConversaoLib::converteMult(tokenDaLinha.operando[0]);
			break;
		case DIV:
			ConversaoLib::codigoConvertido << ConversaoLib::converteDiv(tokenDaLinha.operando[0]);
			break;
		case JMP:
			ConversaoLib::codigoConvertido << ConversaoLib::converteJmp(tokenDaLinha.operando[0]);
			break;
		case JMPN:
			ConversaoLib::codigoConvertido << ConversaoLib::converteJmpn(tokenDaLinha.operando[0]);
			break;
		case JMPP:
			ConversaoLib::codigoConvertido << ConversaoLib::converteJmpp(tokenDaLinha.operando[0]);
			break;
		case JMPZ:
			ConversaoLib::codigoConvertido << ConversaoLib::converteJmpz(tokenDaLinha.operando[0]);
			break;
		case COPY:
			ConversaoLib::codigoConvertido << ConversaoLib::converteCopy(tokenDaLinha.operando[0], tokenDaLinha.operando[1]);
			break;
		case LOAD:
			ConversaoLib::codigoConvertido << ConversaoLib::converteLoad(tokenDaLinha.operando[0]);
			break;
		case STORE:
			ConversaoLib::codigoConvertido << ConversaoLib::converteStore(tokenDaLinha.operando[0]);
			break;
		case INPUT:
			ConversaoLib::codigoConvertido << ConversaoLib::converteInput(tokenDaLinha.operando[0]);
			break;
		case OUTPUT:
			ConversaoLib::codigoConvertido << ConversaoLib::converteOutput(tokenDaLinha.operando[0]);
			break;
		case STOP:
			ConversaoLib::codigoConvertido << ConversaoLib::converteStop();
			break;
		case C_INPUT:
			ConversaoLib::codigoConvertido << ConversaoLib::converteCInput(tokenDaLinha.operando[0]);
			break;
		case C_OUTPUT:
			ConversaoLib::codigoConvertido << ConversaoLib::converteCOutput(tokenDaLinha.operando[0]);
			break;
		case S_INPUT:
			ConversaoLib::codigoConvertido << ConversaoLib::converteSInput(tokenDaLinha.operando[0], tokenDaLinha.operando[1]);
			break;
		case S_OUTPUT:
			ConversaoLib::codigoConvertido << ConversaoLib::converteSOutput(tokenDaLinha.operando[0], tokenDaLinha.operando[1]);
			break;
		}
	}
	else if (tabelaLib.isDiretiva(tokenDaLinha.operacao)) {
		std::cout << "Diretiva \n";
	
		InfoDeDiretivas infoDiretiva = tabelaLib.getDiretiva(tokenDaLinha.operacao);
		switch (infoDiretiva.diretivasKey)
		{
		case SECTION:
			// Em nenhum caso, section tem mais de um operando, logo o primeiro deve ser data, bss ou text
			ConversaoLib::codigoConvertido << converteSection(tokenDaLinha.operando[0]);
			
			break;
		case SPACE:
			ConversaoLib::codigoConvertido << converteSpace(tokenDaLinha.label, tokenDaLinha.operando[0]);
			break;
		case CONST:
			ConversaoLib::codigoConvertido << converteConst(tokenDaLinha.label, tokenDaLinha.operando[0]);
			break;

		// 19-11-2018: Casos abaixo removidos pois o programa deve ser pré-processado antes de ser traduzido
		/*case EQU:
			ConversaoLib::codigoConvertido << converteEqu(tokenDaLinha.label, tokenDaLinha.operando[0]);
			break;
		case IF:
			ConversaoLib::codigoConvertido << converteIf(tokenDaLinha.operando[0]);
			break;*/
		}
	}
	pulaLinhaDeCodigo();
}
// Adiciona pulo de linha ao final de cada instrução convertida
void ConversaoLib::pulaLinhaDeCodigo()
{
	ConversaoLib::codigoConvertido << "\n";
}

// Converte a section com base no operando(TEXT, DATA ou BSS)
std::string ConversaoLib::converteSection(std::string operando) {
	if (operando == "text") {
		ConversaoLib::isSectionText = true;
		return "section .text\nglobal _start\n _start:\n";
	}
	else if (operando == "data") {
		if (ConversaoLib::isSectionText) {
			adicionaFuncoesEmText();
			ConversaoLib::isSectionText = false;
		}
		return adicionaSectionEVariaveisEmData();
	}
	else if (operando == "bss") {
		if (ConversaoLib::isSectionText) {
			adicionaFuncoesEmText();
			ConversaoLib::isSectionText = false;
		}
		return adicionaSectionEVariaveisEmBss();
	}
	else return "";
}

std::string ConversaoLib::converteSpace(std::string label, std::string operando)
{
	int num;
	if (isOperandoNumeroInteiro(operando)) { // Decimal
		num = converteOperandoParaInteiro(operando);
	}
	else { // Hexadecimal
		num = std::stoi(operando, nullptr, 16);
	}

	return label + ": resd " + std::to_string(num);	
}

std::string ConversaoLib::converteConst(std::string label, std::string operando)
{
	int num;
	if (isOperandoNumeroInteiro(operando)) { // Decimal
		num = converteOperandoParaInteiro(operando);
	}
	else { // Hexadecimal
		num = std::stoi(operando, nullptr, 16);
	}

	return label + ": dd " + std::to_string(num);
}

std::string ConversaoLib::converteEqu(std::string label, std::string operando)
{
	int num;
	if (isOperandoNumeroInteiro(operando)) { // Decimal
		num = converteOperandoParaInteiro(operando);
	}
	else { // Hexadecimal
		num = std::stoi(operando, nullptr, 16);
	}

	return label + " equ " + std::to_string(num);
}

std::string ConversaoLib::converteIf(std::string operando) {
	//TODO: Fazer esse método
	return std::string();
}

// Função para debug 
void ConversaoLib::showCodigoConvertido() {
	std::cout << ConversaoLib::codigoConvertido.str();
}

void ConversaoLib::adicionaFuncoesEmText()
{
	std::ifstream arquivoAuxiliarText = std::ifstream("./Files/funcoes_aninhadas.asm");
	ConversaoLib::codigoConvertido << arquivoAuxiliarText.rdbuf();
	arquivoAuxiliarText.close();
}

std::string ConversaoLib::adicionaSectionEVariaveisEmData()
{
	std::string stringData = "section .data\n";
	stringData.append("mensagemOverflow: db \"Overflow em multiplicacao!\", 26\n");
	return stringData;
}

std::string ConversaoLib::adicionaSectionEVariaveisEmBss()
{
	std::string stringData = "section .bss\n";
	stringData.append("response_saida resb 30\n");
	stringData.append("response_size resd 2\n");
	stringData.append("inteiro_tamanho resd 1\n");
	stringData.append("inteiro_string resb 30\n");
	return stringData;
}

std::string ConversaoLib::converteAdd(std::string operando){
	return "add eax, dword[" + operando+"]";
}

std::string ConversaoLib::converteSub(std::string operando) {
	return "sub eax, dword [" + operando + "]";
}

std::string ConversaoLib::converteMult(std::string operando)
{
	//Supomos que eax já tem o operando 1(acc)
	return "mov ebx, dword [" + operando + "] \nimul ebx \njo overflow";
}

std::string ConversaoLib::converteDiv(std::string operando) {
	//Supomos que eax já tem o operando 1(acc)
	return "mov ebx, dword [" + operando + "] \nidiv ebx";
}

std::string ConversaoLib::converteJmp(std::string operando) {
	return "jmp " + operando;
}

std::string ConversaoLib::converteJmpn(std::string operando) {
	return "cmp eax, 0\njl " + operando;
}

std::string ConversaoLib::converteJmpp(std::string operando) {
	return "cmp eax, 0\njg " + operando;
}

std::string ConversaoLib::converteJmpz(std::string operando) {
	return "cmp eax, 0\nje " + operando;
}

std::string ConversaoLib::converteCopy(std::string src, std::string dest)
{
	std::string copyString = "";
	copyString.append("push eax\n");
	copyString.append("push ebx\n");
	copyString.append("mov eax, [" + src + "]\n");
	copyString.append("mov ["+dest+"], eax\n");
	copyString.append("pop ebx\n");
	copyString.append("pop eax\n");

	return copyString;
}

std::string ConversaoLib::converteLoad(std::string operando)
{
	//ACC <- MEM[OP]
	return "mov eax, [" + operando + "]";
}

std::string ConversaoLib::converteStore(std::string operando)
{
	//MEM[OP] <- ACC
	std::string storeString = "";
	storeString.append("push eax \n");
	storeString.append("pop dword ["+operando+"]\n");
	return storeString;
}

std::string ConversaoLib::converteInput(std::string operando)
{
	std::string inputString = "";
	inputString.append("push ecx \n"); 
	inputString.append("push edx \n");
	inputString.append("push eax \n");
	inputString.append("push ebx \n");
	inputString.append("mov ecx, " + operando +"\n");
	inputString.append("mov edx, 12\n");
	inputString.append("; Tamanho de inteiro é constante(10 para o número, 1 para o sinal e 1 para o '/n')\n");
	inputString.append("call escrever_function_modificado\n");
	inputString.append("mov esi, " + operando + " ; Guarda o valor de leitura da string em esi\n");
	inputString.append("call converte_ascii_inteiro\n");
	inputString.append("mov dword ["+operando+"], eax \n");
	inputString.append("pop ebx\n");
	inputString.append("pop eax\n");
	inputString.append("pop edx\n");
	inputString.append("pop ecx\n");

	return inputString;
}

std::string ConversaoLib::converteOutput(std::string operando)
{
	std::string outputString = "";
	outputString.append("push eax\n");
	outputString.append("push ebx\n");
	outputString.append("push ecx\n");
	outputString.append("push edx\n");
	outputString.append("mov dword eax, ["+operando+"]\n");
	outputString.append("call converte_int_para_string\n");
	outputString.append("pop edx\n");
	outputString.append("pop ecx\n");
	outputString.append("pop ebx\n");
	outputString.append("pop eax\n");

	return outputString;
}

int ConversaoLib::converteOperandoParaInteiro(std::string operando) {
	return std::stoi(operando);
}

bool ConversaoLib::isOperandoNumeroInteiro(const std::string &str) {
	return str.find_first_not_of("0123456789") == std::string::npos;
}

std::string ConversaoLib::converteStop()
{
	return "jmp sair";
}

std::string ConversaoLib::converteCInput(std::string operando)
{
	std::string inputChar = "";
	inputChar.append("; c_input\n");
	inputChar.append("; Printa apenas um char\n");
	inputChar.append("; Supondo que está em ASCII\n");
	inputChar.append("; ex: C_INPUT A\n");
	inputChar.append("; ecx = a e edx = 1, já que é apenas um char em ASCII\n");
	inputChar.append("push ecx\n");
	inputChar.append("push edx\n");
	inputChar.append("mov ecx,"+operando+"\n");
	inputChar.append("mov edx, 1\n");
	inputChar.append("call escrever_function\n");
	inputChar.append("pop edx\n");
	inputChar.append("pop ecx\n");

	return inputChar;
}

std::string ConversaoLib::converteCOutput(std::string operando)
{
	std::string outputChar = "";	
	outputChar.append("; c_output\n");
	outputChar.append("; Printa apenas um char\n");
	outputChar.append("; Supondo que está em ASCII\n");
	outputChar.append("; ex: C_OUTPUT A\n");
	outputChar.append("; ecx = a e edx = 1, já que é apenas um char em ASCII\n");
	outputChar.append("push ecx\n");
	outputChar.append("push edx\n");
	outputChar.append("mov ecx, "+operando+"\n");
	outputChar.append("mov edx, 1\n");
	outputChar.append("call print_function\n");
	outputChar.append("pop edx\n");
	outputChar.append("pop ecx\n");
	return outputChar;
}

std::string ConversaoLib::converteSInput(std::string operando, std::string size)
{
	std::string sInput = "";
	sInput.append("push ecx\n");
	sInput.append("push edx\n");
	sInput.append("mov ecx, "+operando+"\n");
	sInput.append("mov edx, "+size+"\n");
	sInput.append("call escrever_function\n");
	sInput.append("pop edx\n");
	sInput.append("pop ecx\n");

	return sInput;
}

std::string ConversaoLib::converteSOutput(std::string operando, std::string size)
{
	std::string oOutput = "";
	oOutput.append("push ecx\n");
	oOutput.append("push edx\n");
	oOutput.append("mov ecx, "+operando+"\n");
	oOutput.append("mov edx, "+size+"\n");
	oOutput.append("call print_function\n");
	oOutput.append("pop edx\n");
	oOutput.append("pop ecx\n");

	return oOutput;

}
