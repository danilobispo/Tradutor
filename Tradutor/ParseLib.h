#pragma once
#include "ParseLib.h"
#include "Tokenizador.h"
#include "TabelaLib.h"
#include <vector>
#include <iostream>
#include <regex>
#include <sstream>
using namespace std;
class ParseLib
{
public:
	ParseLib();
	~ParseLib();
	std::vector<Tokenizador::TokensDaLinha> parseTokens(std::string arquivoConteudo);


	Tokenizador::TokensDaLinha parseLinha(std::string linha, int linhaContador);
	std::string parseLabel(std::string linha);


	std::string parseOperacao(std::string linha, bool hasLabel);

	std::vector<std::string> parseOperando(std::string linha, int numeroDeOperandos, bool hasLabel);

	int converteOperandoHexaParaInteiro(std::string operando);
	

	std::string removeComentarios(std::string linha);
	std::string removeEspacosEmBrancoExtras(std::string arquivoConteudo);
	std::string removeTabulacoes(std::string fileString);
	std::string juntaLabelEOperacao(std::string arquivo);
	std::vector<std::string> separaEmLinhas(std::string fileString);
};

