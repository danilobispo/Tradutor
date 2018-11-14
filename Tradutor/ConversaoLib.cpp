#include "pch.h"
#include "ConversaoLib.h"


ConversaoLib::ConversaoLib()
{
	isSectionData = false;
}

ConversaoLib::~ConversaoLib()
{
}


// Verifica o tipo de instru��o e redireciona para o m�todo mais adequado(diretiva ou instru��o)
// Feita apenas para facilitar o debug do projeto
void ConversaoLib::verificaEConverteOperacao(Tokenizador::TokensDaLinha tokenDaLinha)
{
	// Vari�vel criada para verificar se a section data j� foi criada, 
	// caso sim, o c�digo dever� ignorar quando o .bss ou o .data for chamado novament

	TabelaLib tabelaLib;
	if (tabelaLib.isInstrucao(tokenDaLinha.operacao)) {
		std::cout << "Instru��o \n";
		InfoDeInstrucoes infoInstrucao = tabelaLib.getInstrucao(tokenDaLinha.operacao);
		switch (infoInstrucao.opcodesInstrucoes)
		{
		case ADD:
			break;
		case SUB:
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
			break;
		case CONST:
			break;
		case EQU:
			break;
		case IF:
			break;
		}
	}
	pulaLinhaDeCodigo();
}
// Adiciona pulo de linha ao final de cada instru��o convertida
void ConversaoLib::pulaLinhaDeCodigo()
{
	codigoConvertido << "\n";
}

// Converte a section com base no operando(TEXT, DATA ou BSS)
std::string ConversaoLib::converteSection(std::string operando) {
	if (operando == "text") {
		return "section .text";
	}
	else if ((operando == "data" || operando == "bss") && !isSectionData) {
		isSectionData = true;
		return "section .data";
	}
	else return "";
}

// Fun��o para debug 
void ConversaoLib::showCodigoConvertido() {
	std::cout << codigoConvertido.str();
}
