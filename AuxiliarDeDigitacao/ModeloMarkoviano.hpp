#ifndef _MODELOMARKOVIANO_
#define _MODELOMARKOVIANO_

#include <vector>
#include <set>
#include <string>
#include <fstream>
#include "Definicoes.hpp"

class ModeloMarkoviano
{
private:
    std::set<std::string> Palavras{};
    std::vector<std::vector<std::string>> Frases{};
    std::vector<std::vector<double>*>* MatrizDeProbabilidade{};
    // Funções auxiliares:
    void LerPalavras();
    void InicializarMatriz();
    void DeixarMinusculo(std::string& palavra) const;
    std::string ProximaPalavra(const std::string& palavra, const std::vector<std::string>& frase) const;
    unsigned short ContarOcorrencias(const std::string& palavra) const;
    short IndiceDaPalavra(std::string palavra) const;
    std::string PalavraNoIndice(unsigned short indice) const;
public:
    ModeloMarkoviano(const std::string& arquivo);
    void ProcessarEntradas();
    std::set<std::pair<double,std::string>> GetSugestoes(std::string palavra) const;
    unsigned int GetQtPalavras() const;
};

#endif