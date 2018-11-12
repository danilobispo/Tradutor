#include "pch.h"
#include "ParseLib.h"
using namespace std;

ParseLib::ParseLib()
{
}


ParseLib::~ParseLib()
{
}

std::vector<Tokenizador::TokensDaLinha> ParseLib::parseTokens(std::string arquivoConteudo) {
	arquivoConteudo = removeComentarios(arquivoConteudo);
	arquivoConteudo = removeTabulacoes(arquivoConteudo);
	arquivoConteudo = removeEspacosEmBrancoExtras(arquivoConteudo);
	arquivoConteudo = juntaLabelEOperacao(arquivoConteudo);
	std::vector<std::string> codeLines = this->separaEmLinhas(arquivoConteudo);
	//for (auto& linha : codeLines) {
		// Remo��o dessa �rea do c�digo pois operandos negativos como no CONST eram parseados incorretamente
		/*else if (linha.find('+') != std::string::npos || linha.find('-') != std::string::npos) {
			std::string::size_type i = 0;
			std::string::size_type posicao = linha.find('+');
			if (posicao == std::string::npos) {
				posicao = linha.find('-');
			}
			i = posicao;
			i++;
			while (linha.at(i) == ' ') {
				i++;
			}
			if (linha.at(posicao) != ' ') {
				linha.erase(std::remove_if(linha.begin() + posicao, linha.end(), isspace), linha.end());
				i = posicao;
				i--;
			}
			while (linha.at(i) == ' ') {
				i--;
			}
			if (linha.at(posicao) != ' ') {
				linha.erase(std::remove_if(linha.begin() + i, linha.end(), isspace), linha.end());
				i = posicao;
				i--;
			}
		}*/
		//}
	int contadorLinha = 1;
	std::vector<Tokenizador::TokensDaLinha> listTokensDaLinha;
	int contadorPosicao = 0;
	codeLines.erase(remove(codeLines.begin(), codeLines.end(), ""), codeLines.end());
	for (auto &codeLine : codeLines) {
		Tokenizador::TokensDaLinha tokensDaLinha = parseLinha(codeLine, contadorLinha);
		listTokensDaLinha.push_back(tokensDaLinha);
		contadorLinha++;
	}


	//DEBUG
	for (auto &i : listTokensDaLinha) {
		std::cout << "Label: " << i.label << std::endl;
		std::cout << "Operacao: " << i.operacao << std::endl;
		for (auto it = i.operando.begin(); it != i.operando.end(); ++it) {
			std::cout << "Operando: " << *it << std::endl;
		}
		std::cout << "Numero da Linha: " << i.numeroDaLinha << std::endl << std::endl;
	}
	return listTokensDaLinha;
}

Tokenizador::TokensDaLinha ParseLib::parseLinha(std::string linha, int linhaContador) {
	TabelaLib tabelaLib;
	// Transforma em lowercase
	std::transform(linha.begin(), linha.end(), linha.begin(), ::tolower);

	// Separa os elementos da linha, para isso, usamos um parse para obter cada token da linha
	// <R�tulo>: <Opera��o> <Operando>; <Coment�rios>
	// <R�tulo>
	// Para obtermos a label, executamos o m�todo parseLabel, que retorna uma string com a label ou uma string vazia
	// quando n�o temos a label na linha
	std::string label = parseLabel(linha);

	// <Opera��o>
	// Para obtermos a opera��o, simplesmente precisamos verificar se uma label foi declarada, caso n�o tenha sido, o
	// primeiro elemento daquela linha ser� a opera��o
	std::string operacao = parseOperacao(linha, !label.empty());
	std::vector<std::string> operandos;

	if (tabelaLib.isDiretiva(operacao)) {
		InfoDeDiretivas infoDeDiretivas = tabelaLib.getDiretiva(operacao);
		// DEBUG Diretiva
		//        std::cout << "infos de diretiva: " << std::endl;
		//        std::cout << "diretiva: " << infoDeDiretivas.diretivasKey << std::endl;
		//        std::cout << "/numero de operandos: " << infoDeDiretivas.numeroDeOperandos << std::endl;
		//        std::cout << "tamanho: " << infoDeDiretivas.tamanho << std::endl ;
		//        std::cout << "isPre: " << infoDeDiretivas.isPre << std::endl << std::endl;

		operandos = parseOperando(linha, infoDeDiretivas.numeroDeOperandos, !label.empty());
		// DEBUG Operando
		for (auto &labelOperando : operandos) {
			//            std::cout << "Operando: " << labelOperando << std::endl;
		}
		if (infoDeDiretivas.tamanho == -1) {
		}
		//  TODO: Realocar essa parte do m�todo em outro lugar
		//        setContadorPosicao(contadorPosicao + infoDeDiretivas.tamanho);
	}
	else if (tabelaLib.isInstrucao(operacao)) {
		InfoDeInstrucoes infoDeInstrucoes = tabelaLib.getInstrucao(operacao);
		// DEBUG Instru��o
		//        std::cout << "Infos de instrucao: " << std::endl;
		//        std::cout << "Instrucao: " << infoDeInstrucoes.opcodesInstrucoes << std::endl;
		//        std::cout << "Numero de operandos: " << infoDeInstrucoes.numeroDeOperandos << std::endl;
		//        std::cout << "Tamanho: " << infoDeInstrucoes.tamanho << std::endl << std::endl;
		operandos = parseOperando(linha, infoDeInstrucoes.numeroDeOperandos, !label.empty());
		// DEBUG Operando
		for (auto &labelOperando : operandos) {
			//            std::cout << "Operando: " << labelOperando << std::endl;
		}
		//        setContadorPosicao(contadorPosicao + infoDeInstrucoes.tamanho);
	}
	else {
		std::cout << "Opera��o inexistente!" << std::endl;
	}
	return Tokenizador::TokensDaLinha(label, operacao, operandos, linhaContador);
}


std::string ParseLib::removeEspacosEmBrancoExtras(std::string arquivoConteudo) {

	//remove duplos espa�os
	std::string::size_type posicaoEspaco = arquivoConteudo.find("  ");
	while (posicaoEspaco != std::string::npos) {
		arquivoConteudo.erase(arquivoConteudo.begin() + posicaoEspaco);
		posicaoEspaco = arquivoConteudo.find("  ");
	}

	//remove duplos pulos de linha
	posicaoEspaco = arquivoConteudo.find("\n\n");
	while (posicaoEspaco != std::string::npos) {
		arquivoConteudo.erase(arquivoConteudo.begin() + posicaoEspaco);
		posicaoEspaco = arquivoConteudo.find("\n\n");
	}

	////remove duplos pulos no final do arquivo
	//posicaoEspaco = arquivoConteudo.find("\n" + EOF);
	//while (posicaoEspaco != std::string::npos) {
	//	arquivoConteudo.erase(arquivoConteudo.begin() + posicaoEspaco);
	//	posicaoEspaco = arquivoConteudo.find("\n" + EOF);
	//}

	//remove duplos pulos no inicio do arquivo
	while (arquivoConteudo[0] == '\n') {
		arquivoConteudo.erase(arquivoConteudo.begin());
	}

	//remove espa�os antes de \n
	posicaoEspaco = arquivoConteudo.find(" \n");
	while (posicaoEspaco != std::string::npos) {
		arquivoConteudo.erase(arquivoConteudo.begin() + posicaoEspaco);
		posicaoEspaco = arquivoConteudo.find(" \n");
	}

	//remove espa�os depois de \n
	posicaoEspaco = arquivoConteudo.find("\n ");
	while (posicaoEspaco != std::string::npos) {
		arquivoConteudo.erase(arquivoConteudo.begin() + posicaoEspaco + 1);
		posicaoEspaco = arquivoConteudo.find("\n ");
	}

	//remove espa�os antes de :
	posicaoEspaco = arquivoConteudo.find(" :");
	while (posicaoEspaco != std::string::npos) {
		arquivoConteudo.erase(arquivoConteudo.begin() + posicaoEspaco);
		posicaoEspaco = arquivoConteudo.find(" :");
	}

	return arquivoConteudo;

	/*std::regex labelComPuloDeLinha("(.+:)(\n)");
	arquivoConteudo = std::regex_replace(arquivoConteudo, labelComPuloDeLinha, "$1 ");
	std::regex labelComLetraLogoAposDoisPontos("(.+:)(\\w)");
	arquivoConteudo = std::regex_replace(arquivoConteudo, labelComLetraLogoAposDoisPontos, "$1 $2");*/

	//std::string output;
	//output.clear();
	//unique_copy(arquivoConteudo.begin(),
	//	arquivoConteudo.end(),
	//	std::back_insert_iterator<std::string>(output),
	//	[](char a, char b) { return isspace(static_cast<unsigned char>(a)) && isspace(static_cast<unsigned char>(b)); });



	//return output;
}

std::string ParseLib::removeComentarios(std::string arquivoConteudo) {
	// Coment�rios s�o definidos por um ponto e v�rgula antes do coment�rio em si, ent�o
	// separaremos a arquivoConteudo pelo delimitador ;
	// Verificamos antes se a arquivoConteudo possui coment�rios, e ent�o fazemos a remo��o de string
	std::string::size_type posicaoComentario = posicaoComentario = arquivoConteudo.find(';');
	while (posicaoComentario != std::string::npos) {

		std::string::size_type posicaoFinal = posicaoComentario;
		while (arquivoConteudo[posicaoFinal] != '\n') {
			posicaoFinal++;
		}

		arquivoConteudo.erase(arquivoConteudo.begin() + posicaoComentario, arquivoConteudo.begin() + posicaoFinal);
		posicaoComentario = arquivoConteudo.find(';');
	}
	return arquivoConteudo;
}

std::string ParseLib::removeTabulacoes(std::string fileString) {
	// Remove tabula��es transformando-as em espa�os em branco, caso os espa�os em branco sejam redundantes, 
	// o passo de remover espa�os em branco ir� remover a redund�ncia.
	fileString = std::regex_replace(fileString, std::regex("\t"), " ");
	// fileString.erase(std::remove(fileString.begin(), fileString.end(), '\t'), fileString.end());
	return fileString;
}

std::string ParseLib::juntaLabelEOperacao(std::string arquivo)
{
	std::string::size_type posicao = arquivo.find(":\n");
	while (posicao != std::string::npos) {
		arquivo[posicao + 1] = ' ';
		posicao = arquivo.find(":\n");
	}
	return arquivo;
}

std::vector<std::string> ParseLib::separaEmLinhas(std::string fileString) {
	std::vector<std::string> strings;
	const std::string delimiter = "\n";

	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	while ((pos = fileString.find(delimiter, prev)) != std::string::npos) {
		strings.push_back(fileString.substr(prev, pos - prev));
		prev = pos + 1;
	}

	// To get the last substring (or only, if delimiter is not found)
	strings.push_back(fileString.substr(prev));

	return strings;
}

std::string ParseLib::parseLabel(std::string linha) {
	std::string label;
	// Provavelmente a label vir� no seguinte formato:
	// LABEL:
	// Ent�o o programa precisa separar a string dos dois pontos e retornar a label original, a ser inserida na
	// tabela de s�mbolos.
	std::string::size_type posicaoLabel = linha.find(':');
	if (posicaoLabel != std::string::npos) {
		label = linha.substr(0, posicaoLabel);
		// DEBUG
		//        std::cout << "parseLabel label: " << label << std::endl;
		return label;
	}
	else {
		//        std::cout << "Nao existem labels nessa linha!" << std::endl;
		return "";
	}
}

std::string ParseLib::parseOperacao(std::string linha, bool hasLabel) {
	// Uma operacao � uma palavra ap�s uma label(caso a linha tenha label)
	// ou a primeira palavra caso a linha n�o tenha label
	// Em caso de label, separamos a linha por espa�os em brancos (o c�digo garante que, nesse ponto, a label e a
	// opera��o s�o separados por pelo menos um espa�o) e obtemos a primeira palavra:
	std::vector<std::string> tokensLinhas;
	std::istringstream iss(linha);
	std::copy(std::istream_iterator<std::string>(iss),
		std::istream_iterator<std::string>(),
		std::back_inserter(tokensLinhas));

	if (hasLabel) {
		//std::cout << "Linha Operacao/Diretiva:" << tokensLinhas[1] << std::endl;
		return tokensLinhas[1];
	}
	else {
		//std::cout << "Linha Operacao/Diretiva:" << tokensLinhas[0] << std::endl;
		return tokensLinhas[0];
	}
}

std::vector<std::string> ParseLib::parseOperando(std::string linha, int numeroDeOperandos, bool hasLabel) {
	// Um operando vem logo ap�s a opera��o, mas se a linha tiver label, a posi��o do operando pode mudar, logo passamos
	// isso como par�metro tamb�m
	// Obt�m os tokens da linha separadamente
	std::vector<std::string> tokensLinhas;
	std::istringstream iss(linha);
	std::copy(std::istream_iterator<std::string>(iss),
		std::istream_iterator<std::string>(),
		std::back_inserter(tokensLinhas));
	// se tiver label, a token[0] � o label e token[1] � a opera��o:
	std::vector<std::string> operandosString;
	std::vector<int> operandos;
	std::regex operandosHexa("0[xX][0-9a-fA-F]+");
	std::regex operandosHexaNegativo("-0[xX][0-9a-fA-F]+");

	if (hasLabel) {
		if (numeroDeOperandos == 0) { // Caso de algumas diretivas
			// Nada a ser feito
			if (tokensLinhas.size() != 2) {
				// TODO: Colocar n�mero correto da linha
				std::cout << "N�mero de operandos inv�lido!";
			}
		}
		else if (numeroDeOperandos == 1) { // Todos os outros
			if (tokensLinhas.size() != 3) {
				// TODO: Colocar n�mero correto da linha
				std::cout << "N�mero de operandos inv�lido!";
			}
			operandosString.push_back(tokensLinhas[2]);
		}
		else if (numeroDeOperandos == 2) { // Caso do COPY
			if (linha.find(", ") == std::string::npos) {
				std::cout << "COPY separado por v�rgula";
			}
			// Os dois est�o juntos e separados por uma v�rgula, logo � s� fazer uma substr para cada operando
			if (tokensLinhas.size() != 4) {
				// TODO: Colocar n�mero correto da linha
				std::cout << "N�mero de operandos inv�lido!";
			}
			std::string op1, op2;
			op1 = tokensLinhas[2].substr(0, tokensLinhas[2].find(','));
			op2 = tokensLinhas[3].substr(0, tokensLinhas[3].size());
			operandosString.push_back(op1);
			operandosString.push_back(op2);
		}
		else if (numeroDeOperandos == -1) { // Caso do SPACE 
			std::string operacao = tokensLinhas[1];
			if (operacao == "space") {
				if (tokensLinhas.size() == 3) {
					operandosString.push_back(tokensLinhas[2]); // Se tiver valor, o valor a frente � o espa�o
				}
				else if (tokensLinhas.size() == 2) {
					operandosString.push_back("1"); // Se n�o tiver, consideramos que o operando � 1
				}
				else {
					std::cout << "N�mero de operandos inv�lido!";
				}
			}
		}
		// Caso sem label, tokens relativos diminuem em uma posi��o no vetor
	}
	else {
		if (numeroDeOperandos == 0) { // Caso de algumas diretivas
			// Nada a ser feito
		}
		else if (numeroDeOperandos == 1) { // Todos os outros
			operandosString.push_back(tokensLinhas[1]);
			if (tokensLinhas.size() != 2) {
				// TODO: Colocar n�mero correto da linha
				std::cout << "N�mero de operandos inv�lido!";
			}
		}
		else if (numeroDeOperandos == 2) { // Caso do COPY
			if (linha.find(", ") == std::string::npos) {
				std::cout << "copy n separado por espa�o!";
			}
			if (tokensLinhas.size() != 3) {
				// TODO: Colocar n�mero correto da linha
				std::cout << "N�mero de operandos inv�lido!";
			}
			//Os dois est�o juntos e separados por uma v�rgula COM espa�o, logo � s� fazer uma substr para cada operando
			std::string op1, op2;
			std::string::size_type virgulaPos = tokensLinhas[1].find(',');
			op1 = tokensLinhas[1].substr(0, virgulaPos);
			op2 = tokensLinhas[2].substr(0, tokensLinhas[2].size());

			operandosString.push_back(op1);
			operandosString.push_back(op2);
		}
		else if (numeroDeOperandos == -1) { // Caso do SPACE 
			std::string operacao = tokensLinhas[0];
			int tamanho = tokensLinhas.size();
			if (operacao == "space") {
				if (tokensLinhas.size() == 2) {
					operandosString.push_back(tokensLinhas[1]);
				}
				else if (tokensLinhas.size() == 1) { operandosString.push_back("1"); }
				else {
					std::cout << "N�mero de operandos inv�lido!";
				}
			}
		}
	}

	// Verifica nessa sess�o se o operando em quest�o � um n�mero(decimal ou hexadecimal), ou um label
	for (auto &i : operandosString) {
		if (std::regex_match(i, operandosHexa) ||
			std::regex_match(i, operandosHexaNegativo)) { // Caso de operando ser hexadecimal:
			i = std::to_string(converteOperandoHexaParaInteiro(i));
		}
	}

	return operandosString;
}

int ParseLib::converteOperandoHexaParaInteiro(std::string operando) {
	return std::stoi(operando, nullptr, 16);
}



