#include "pch.h"
#include "ConversaoLib.h"


ConversaoLib::ConversaoLib()
{
}

ConversaoLib::~ConversaoLib()
{
}

void ConversaoLib::verificaOperacao(std::string operacao)
{
	TabelaLib tabelaLib;
	if (tabelaLib.isInstrucao(operacao)) {
		std::cout << "Instru��o \n";
	}
	else if (tabelaLib.isDiretiva(operacao)) {
		std::cout << "Diretiva \n";
	}
}
