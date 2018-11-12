#include "pch.h"
#include <iostream>
#include "ErrorLib.h"

/**
* Se esse construtor for chamado, o erro ir� redirecionar o usu�rio para o fim do programa ap�s uma mensagem de erro.
* @param linhaDoErro
* @param mensagemDeErro
* @param tipoErro
*/
ErrorLib::ErrorLib(int linhaDoErro, const std::string &mensagemDeErro, const std::string &tipoErro) : linhaDoErro(
	linhaDoErro), mensagemDeErro(mensagemDeErro), tipoErro(tipoErro) {
	std::cout << "Erro!" << mensagemDeErro << " Na linha:" << linhaDoErro << " Tipo de Erro: " << tipoErro << std::endl;
	std::cout << "Aperte qualquer bot�o para sair: " << std::endl;
	getchar();
	exit(1);
}

