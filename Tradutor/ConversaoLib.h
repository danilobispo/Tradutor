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
	ConversaoLib(std::vector<Tokenizador::TokensDaLinha> tokensDaLinha);
	~ConversaoLib();

	std::vector<Tokenizador::TokensDaLinha> tokensCodigo;
	bool isSectionData;
	bool criouAcc;

	std::stringstream codigoConvertido;
	void verificaEConverteOperacao(std::string operacao);
	void verificaEConverteOperacao(Tokenizador::TokensDaLinha tokenDaLinha);
	void criaAcumulador();
	void criaAcumulador(std::string operacao);
	int converteOperandoParaInteiro(std::string operando);
	bool isOperandoNumeroInteiro(const std::string & str);	
	void pulaLinhaDeCodigo();
	void showCodigoConvertido();
	void verificaInstrucao(std::string instrucao);
	void verificaDiretiva(std::string diretiva);
	void converteAdd();
	std::string converteSection(std::string operando);
	std::string converteSpace(std::string label, std::string operando);
};

