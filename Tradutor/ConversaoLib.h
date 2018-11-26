#pragma once
#include "TabelaLib.h"
#include "Tokenizador.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>

/* Classe responsável por obter a instrução e fazer o código em NASM correspondente */
class ConversaoLib
{
public:
	ConversaoLib();
	ConversaoLib(std::vector<Tokenizador::TokensDaLinha> tokensDaLinha);
	~ConversaoLib();

	std::vector<Tokenizador::TokensDaLinha> tokensCodigo;
	std::stringstream codigoConvertido;
	bool isSectionText;

	void criarArquivoDeSaida();

	void verificaEConverteOperacao(Tokenizador::TokensDaLinha tokenDaLinha);
	int converteOperandoParaInteiro(std::string operando);
	bool isOperandoNumeroInteiro(const std::string & str);	
	void pulaLinhaDeCodigo();
	void showCodigoConvertido();
	

	void adicionaFuncoesEmText();
	std::string adicionaSectionEVariaveisEmData();
	std::string adicionaSectionEVariaveisEmBss();

	//Conversão de instruções
	std::string converteAdd(std::string operando);
	std::string converteSub(std::string operando);
	std::string converteMult(std::string operando);
	std::string converteDiv(std::string operando);
	std::string converteJmp(std::string operando);
	std::string converteJmpn(std::string operando);
	std::string converteJmpp(std::string operando);
	std::string converteJmpz(std::string operando);
	std::string converteCopy(std::string src, std::string dest);
	std::string converteLoad(std::string operando);
	std::string converteStore(std::string operando);
	std::string converteInput(std::string operando);
	std::string converteOutput(std::string operando);
	std::string converteStop();
	std::string converteCInput(std::string operando);
	std::string converteCOutput(std::string operando);
	std::string converteSInput(std::string operando, std::string size);
	std::string converteSOutput(std::string operando, std::string size);

	// Conversão de diretivas
	std::string converteSection(std::string operando);
	std::string converteSpace(std::string label, std::string operando);
	std::string converteConst(std::string label, std::string operando);
	std::string converteEqu(std::string label, std::string operando);
	std::string converteIf(std::string operando);
};

