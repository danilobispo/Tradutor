#pragma once
#include <string>

class ErrorLib {
private:
	int linhaDoErro;
	std::string mensagemDeErro;
	std::string tipoErro; //L�xico, sint�tico ou sem�ntico
public:
	ErrorLib(int linhaDoErro, const std::string &mensagemDeErro, const std::string &tipoErro);
};
