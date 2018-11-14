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
		std::cout << "Instrução \n";
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
			// Verificamos se já podemos colocar o acumulador(Variável global responsável pela grande maioria das operações)
			// Se a section .data já foi criada
			if (!criouAcc) {
				ConversaoLib::criaAcumulador(tokenDaLinha.operando[0]);
			}
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

// Função para debug 
void ConversaoLib::showCodigoConvertido() {
	std::cout << ConversaoLib::codigoConvertido.str();
}

void ConversaoLib::criaAcumulador(std::string operacao) {
	pulaLinhaDeCodigo();
	if (isSectionData && (operacao == "data" || operacao == "bss")) {
		// Cria o acumulador embaixo da declaração de .data
		ConversaoLib::codigoConvertido << "acc resw 2";
		ConversaoLib::criouAcc = true;
	}
}
