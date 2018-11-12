#pragma once
#include <vector>
#include <string>
class Tokenizador
{
public:
	Tokenizador();
	~Tokenizador();

	struct TokensDaLinha {
		std::string label;
		std::string operacao;
		std::vector<std::string> operando;
		// Dados adicionais para facilitar
		int numeroDaLinha;

		TokensDaLinha(const std::string &label, const std::string &operacao, const std::vector<std::string> &operando,
			int numeroDaLinha);
	};

	std::vector<TokensDaLinha> listaDeTokensDoArquivo;

	void adicionarTokenDaLinha(Tokenizador::TokensDaLinha linha);
};

