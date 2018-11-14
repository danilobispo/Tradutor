#pragma once
#include "TabelaLib.h"
#include "Tokenizador.h"
#include <iostream>
#include <sstream>
#include <vector>

/* Classe respons�vel por obter a instru��o e fazer o c�digo em NASM correspondente */
class ConversaoLib
{
public:
	ConversaoLib();
	ConversaoLib(std::vector<Tokenizador::TokensDaLinha> tokensDaLinha);
	~ConversaoLib();

	std::vector<Tokenizador::TokensDaLinha> tokensCodigo;
	bool isSectionData;

	std::stringstream codigoConvertido;
	void verificaEConverteOperacao(std::string operacao);
	void verificaEConverteOperacao(Tokenizador::TokensDaLinha tokenDaLinha);
	void pulaLinhaDeCodigo();
	std::string converteSection(std::string operando);
	void showCodigoConvertido();
	void verificaInstrucao(std::string instrucao);
	void verificaDiretiva(std::string diretiva);
	void converteAdd();
};

