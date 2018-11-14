#include "pch.h"
#include "ConversaoLib.h"


ConversaoLib::ConversaoLib()
{
	// Inicializa a variável no construtor pois durante o loop ela pode assumir valores diferentes
	isSectionData = false;
}

ConversaoLib::~ConversaoLib()
{
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
		switch (infoInstrucao.opcodesInstrucoes)
		{
		case ADD:
			ConversaoLib::codigoConvertido << ConversaoLib::converteAdd(tokenDaLinha.operando[0]);
			break;
		case SUB:
			ConversaoLib::codigoConvertido << ConversaoLib::converteSub(tokenDaLinha.operando[0]);
			break;
		case MULT:
			break;
		case DIV:
			break;
		case JMP:
			break;
		case JMPN:
			break;
		case JMPP:
			break;
		case JMPZ:
			break;
		case COPY:
			break;
		case LOAD:
			break;
		case STORE:
			break;
		case INPUT:
			break;
		case OUTPUT:
			break;
		case STOP:
			break;
		case C_INPUT:
			break;
		case C_OUTPUT:
			break;
		case S_INPUT:
			break;
		case S_OUTPUT:
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
			// Verificamos se já podemos colocar o acumulador(Variável global responsável pela grande maioria das operações)
			// Se a section .data já foi criada
			if (!criouAcc) {
				ConversaoLib::criaAcumulador(tokenDaLinha.operando[0]);
			}
			break;
		case SPACE:
			ConversaoLib::codigoConvertido << converteSpace(tokenDaLinha.label, tokenDaLinha.operando[0]);
			break;
		case CONST:
			ConversaoLib::codigoConvertido << converteConst(tokenDaLinha.label, tokenDaLinha.operando[0]);
			break;
		case EQU:
			ConversaoLib::codigoConvertido << converteEqu(tokenDaLinha.label, tokenDaLinha.operando[0]);
			break;
		case IF:
			ConversaoLib::codigoConvertido << converteIf(tokenDaLinha.operando[0]);
			break;
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
		return "section .text";
	}
	else if ((operando == "data" || operando == "bss") && !ConversaoLib::isSectionData) {
		ConversaoLib::isSectionData = true;
		return "section .data";
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

std::string ConversaoLib::converteAdd(std::string operando){
	return "add acc, " + operando;
}

std::string ConversaoLib::converteSub(std::string operando) {
	return "sub acc, " + operando;
}

// Escreve no arquivo de saída a declaração do acumulador
void ConversaoLib::criaAcumulador(std::string dataOuBss) {
	pulaLinhaDeCodigo();
	if (isSectionData && (dataOuBss == "data" || dataOuBss == "bss")) {
		// Cria o acumulador embaixo da declaração de .data
		ConversaoLib::codigoConvertido << "acc: resw 2";
		ConversaoLib::criouAcc = true;
	}
}

int ConversaoLib::converteOperandoParaInteiro(std::string operando) {
	return std::stoi(operando);
}

bool ConversaoLib::isOperandoNumeroInteiro(const std::string &str) {
	return str.find_first_not_of("0123456789") == std::string::npos;
}