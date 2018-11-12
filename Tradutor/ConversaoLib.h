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
	~ConversaoLib();

	std::vector<Tokenizador::TokensDaLinha> tokensCodigo;

	std::stringstream codigoConvertido;
	void verificaOperacao(std::string operacao);
	void converteAdd();
};

