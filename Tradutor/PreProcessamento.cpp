#include "pch.h"
#include "PreProcessamento.h"
using namespace std;


PreProcessamento::PreProcessamento(std::vector<Tokenizador::TokensDaLinha> tokensDaLinhaList) : tokensDaLinhaList(
	tokensDaLinhaList) {

	bool entrouEmText = false;
	for (int i = 0; i < tokensDaLinhaList.size(); i++) {
		if (tokensDaLinhaList[i].operacao == "section" && tokensDaLinhaList[i].operando[0] == "text") {
			entrouEmText = true;
		}
		if (tokensDaLinhaList[i].operacao == "equ") {
			if (!entrouEmText) {
				TabelaLib tabelaLib;

				if (!tabelaLib.rotuloJaExistenteNaTabelaDeSimbolos(tokensDaLinhaList[i].label)) {
					tabelaLib.insereSimboloNaTabelaDeSimbolos(
						tokensDaLinhaList[i].label,
						InfoDeSimbolo(
							converteStringParaInt(tokensDaLinhaList[i].operando[0]),
							converteStringParaInt(tokensDaLinhaList[i].operando[0]),
							true,
							converteStringParaInt(tokensDaLinhaList[i].operando[0]),
							false));
				}
				else {
					// ErrorLib erro(tokensDaLinhaList[i].numeroDaLinha, "Simbolo \"" + tokensDaLinhaList[i].label + "\" ja definido", "Semântico");
				}
				tokensDaLinhaList.erase(tokensDaLinhaList.begin() + i);
				i = 0;

			}
			else {
				// ErrorLib erro(tokensDaLinhaList[i].numeroDaLinha, "A diretiva EQU deve ser definida antes de SECTION TEXT", "Semântico");
			}
		}
	}


	showTabelaDeSimbolos();
	int i = 0;
	while (i < tokensDaLinhaList.size()) {
		if (tokensDaLinhaList[i].operacao == "if") {
			std::string::size_type tamanhoTokenList = tokensDaLinhaList.size();
			TabelaLib tabelaLib;
			bool ehSimbolo = tabelaLib.rotuloJaExistenteNaTabelaDeSimbolos(tokensDaLinhaList[i].operando[0]);
			bool apagaProximaLinha = false;
			if (ehSimbolo) {
				InfoDeSimbolo infoDeSimbolo = tabelaLib.obtemSimboloNaTabelaDeSimbolos(tokensDaLinhaList[i].operando[0]);
				if (infoDeSimbolo.valorConstante != 0) {
					apagaProximaLinha = false;
				}
				else {
					apagaProximaLinha = true;
				}
			}
			else if (isOperandoNumero(tokensDaLinhaList[i].operando[0])) {
				if (converteStringParaInt(tokensDaLinhaList[i].operando[0]) == 0) {
					apagaProximaLinha = true;
				}
				else {
					apagaProximaLinha = false;
				}
			}
			else {
				// ErrorLib errorLib = ErrorLib(tokensDaLinhaList[i].numeroDaLinha, "Operando invalido para IF", "Sintatico");
			}
			if (i == tamanhoTokenList) { //Nada a ser feito, tamanho da lista é igual ao indice(ultima linha)
				apagaProximaLinha = false;
			}
			if (apagaProximaLinha) {
				tokensDaLinhaList.erase(tokensDaLinhaList.begin() + i, tokensDaLinhaList.begin() + (i + 2));
				i = 0; // Volta para a posição 0 pois as posições mudaram, então é possível que um if tenha aparecido antes
			}
			else {
				tokensDaLinhaList.erase(tokensDaLinhaList.begin() + i); // Apaga o if atual, já que já foi avaliado
				i++;
			}
		}
		else {
			i++;
		}
	}
	setTokensDaLinhaList(tokensDaLinhaList);
}


PreProcessamento::~PreProcessamento()
{
}

int PreProcessamento::converteStringParaInt(std::string operando) {
	return std::stoi(operando);
}

void PreProcessamento::showTabelaDeSimbolos() {
	TabelaLib tabelaLib;
	std::map<std::string, InfoDeSimbolo> tabelaDeSimbolos = tabelaLib.getTabelaDeSimbolos();
	std::string isExterno;
	std::cout << "Tabela de Símbolos: " << std::endl;
	for (auto &tabelaDeSimbolo : tabelaDeSimbolos) {
		isExterno = tabelaDeSimbolo.second.isExtern ? "true" : "false";
		std::cout << "Simbolo: " << tabelaDeSimbolo.first << std::endl;
		std::cout << "Valor: " << tabelaDeSimbolo.second.endereco << std::endl;
		std::cout << "Externo? : " << isExterno << std::endl;
	}
}

bool PreProcessamento::isOperandoNumero(std::string operando) {
	return !operando.empty() && std::find_if(operando.begin(),
		operando.end(), [](char c) { return !std::isdigit(c); }) == operando.end();
}

std::vector<Tokenizador::TokensDaLinha> PreProcessamento::getTokensDaLinhaList() {
	return tokensDaLinhaList;
}

void PreProcessamento::setTokensDaLinhaList(const std::vector<Tokenizador::TokensDaLinha> tokensDaLinhaList) {
	PreProcessamento::tokensDaLinhaList = tokensDaLinhaList;
}
