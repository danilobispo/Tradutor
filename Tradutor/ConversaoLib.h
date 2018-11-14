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
	bool criouAcc;

	std::stringstream codigoConvertido;
	void verificaEConverteOperacao(Tokenizador::TokensDaLinha tokenDaLinha);
	void criaAcumulador(std::string dataOuBss);
	int converteOperandoParaInteiro(std::string operando);
	bool isOperandoNumeroInteiro(const std::string & str);	
	void pulaLinhaDeCodigo();
	void showCodigoConvertido();
	
	//Convers�o de instru��es
	std::string converteAdd(std::string operando);

	std::string converteSub(std::string operando);

	// Convers�o de diretivas
	std::string converteSection(std::string operando);
	std::string converteSpace(std::string label, std::string operando);
	std::string converteConst(std::string label, std::string operando);
	std::string converteEqu(std::string label, std::string operando);
	std::string converteIf(std::string operando);
};

