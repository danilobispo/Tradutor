#include "pch.h"
#include "ConversaoLib.h"


ConversaoLib::ConversaoLib()
{
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
		return "section .text";
	}
	else if (operando == "data") {
		
		return "section .data";
	}
	else if (operando == "bss") {
		return "section .bss";
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
	return "add eax, " + operando;
}

std::string ConversaoLib::converteSub(std::string operando) {
	return "sub eax, " + operando;
}

std::string ConversaoLib::converteMult(std::string operando)
{
	//todo: Estender o sinal para edx antes de fazer a operação
	//TODO: Implementar
	return std::string();
}

// Escreve no arquivo de saída a declaração do acumulador
//void ConversaoLib::criaAcumulador(std::string dataOuBss) {
//	pulaLinhaDeCodigo();
//	if (isSectionData && (dataOuBss == "data" || dataOuBss == "bss")) {
//		// Cria o acumulador embaixo da declaração de .data
//		ConversaoLib::codigoConvertido << "acc: resw 2";
//		//19-11-18: Removido pois o acumulador é o próprio eax
//		//ConversaoLib::criouAcc = true;
//	}
//}

int ConversaoLib::converteOperandoParaInteiro(std::string operando) {
	return std::stoi(operando);
}

bool ConversaoLib::isOperandoNumeroInteiro(const std::string &str) {
	return str.find_first_not_of("0123456789") == std::string::npos;
}