// Tradutor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "ParseLib.h"
#include "Tokenizador.h"
#include "ConversaoLib.h"
#include "PreProcessamento.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Tradutor.h"
using namespace std;

int main(int argc, char* argv[])
{
	std::ifstream arquivo;

	if (argc != 2) {
		std::cout << "Erro!, numero de parametros invalido!";
		getchar();
		return 1;
	}
	std::string nomeDoArquivo = argv[1];
	arquivo.open(nomeDoArquivo);
	if (arquivo.is_open() && arquivo.good()) {
		std::stringstream conteudoArquivo;
		conteudoArquivo << arquivo.rdbuf();
		std::cout << conteudoArquivo.str();
		ParseLib parseLib;
		std::vector<Tokenizador::TokensDaLinha> tokensDaLinha = parseLib.parseTokens(conteudoArquivo.str());
		PreProcessamento preprocess = PreProcessamento(tokensDaLinha);
		tokensDaLinha = preprocess.getTokensDaLinhaList();
		ConversaoLib conversor;
		for (auto linhaToken : tokensDaLinha) {
			conversor.verificaEConverteOperacao(linhaToken);
		}
		conversor.showCodigoConvertido();
		conversor.criarArquivoDeSaida();

		arquivo.close();
	}
	std::cout << "Aperte qualquer botão para sair!";
	getchar();
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
