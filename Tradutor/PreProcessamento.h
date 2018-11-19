#pragma once
#include "Tokenizador.h"
#include "TabelaLib.h"
#include <cctype>
#include <iostream>
#include <vector>
class PreProcessamento
{
public:
	PreProcessamento();
	PreProcessamento(std::vector<Tokenizador::TokensDaLinha> tokensDaLinhaList);
	~PreProcessamento();
	int converteStringParaInt(std::string operando);
	void showTabelaDeSimbolos();
	bool isOperandoNumero(std::string operando);
	std::vector<Tokenizador::TokensDaLinha> getTokensDaLinhaList();
	void setTokensDaLinhaList(const std::vector<Tokenizador::TokensDaLinha> tokensDaLinhaList);
private:
	std::vector<Tokenizador::TokensDaLinha> tokensDaLinhaList;
};

