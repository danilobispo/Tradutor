#pragma once
#include "TabelaLib.h"
#include "Tokenizador.h"
#include <iostream>
#include <sstream>
#include <vector>

/* Classe responsável por obter a instrução e fazer o código em NASM correspondente */
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

