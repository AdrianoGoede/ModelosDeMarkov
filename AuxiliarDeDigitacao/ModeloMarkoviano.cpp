#include "ModeloMarkoviano.hpp"

// Funções auxiliares:

void ModeloMarkoviano::LerPalavras()
{
    for (auto i{this->Frases.cbegin()}; i < this->Frases.cend(); i++) // i Aponta para cada frase
        for (auto k{i->cbegin()}; k < i->cend(); k++) // k Aponta para cada palavra da frase
            this->Palavras.emplace(*k);
}

void ModeloMarkoviano::InicializarMatriz()
{
    this->MatrizDeProbabilidade = new std::vector<std::vector<double>*>{};
    for (unsigned short i{0}; i < this->Palavras.size(); i++)
        this->MatrizDeProbabilidade->push_back(new std::vector<double>{});
}

void ModeloMarkoviano::DeixarMinusculo(std::string& palavra) const
{
    for (auto i{palavra.begin()}; i < palavra.end(); i++)
        if ((*i >= 65) && (*i <= 90))
            *i += 32;
}

std::string ModeloMarkoviano::ProximaPalavra(const std::string& palavra, const std::vector<std::string>& frase) const
{
    for (auto i{frase.cbegin()}; i < frase.cend(); i++)
        if (*i == palavra)
        {
            if (i < (frase.cend() - 1))
                return *(i+1);
            return "*";
        }
    return "";
}

unsigned short ModeloMarkoviano::ContarOcorrencias(const std::string& palavra) const
{
    unsigned short ret{0};
    for (auto i{this->Frases.cbegin()}; i < this->Frases.cend(); i++) // i Aponta para cada frase
        for (auto k{i->cbegin()}; k < i->cend(); k++) // k Aponta para cada palavra da frase
            if (*k == palavra)
                ret++;
    return ret;
}

short ModeloMarkoviano::IndiceDaPalavra(std::string palavra) const
{
    this->DeixarMinusculo(palavra);
    short indice{0};
    for (auto i{this->Palavras.cbegin()}; i != this->Palavras.cend(); i++, indice++)
        if (*i == palavra)
            return indice;
    return -1;
}

std::string ModeloMarkoviano::PalavraNoIndice(unsigned short indice) const
{
    for (auto i{this->Palavras.cbegin()}; i != this->Palavras.cend(); i++, indice--)
        if (indice == 0)
            return *i;
    return "";
}

// Funções principais:

ModeloMarkoviano::ModeloMarkoviano(const std::string& arquivo)
{
    std::ifstream fs{arquivo};
    for (unsigned short i{0}; i < NumEntradas; i++)
    {
        this->Frases.push_back(std::vector<std::string>{});
        std::string palavra;
        while (true)
        {
            fs >> palavra;
            this->DeixarMinusculo(palavra);
            if (palavra != "*")
                this->Frases[i].push_back(palavra);
            else
                break;
        }
    }
    fs.close();
}

void ModeloMarkoviano::ProcessarEntradas()
{
    // Ler palavras:
    this->LerPalavras();
    // Criar e inicializar matriz:
    this->InicializarMatriz();
    // Calcular probabilidades:
    unsigned short indice{0};
    for (auto i{this->Palavras.cbegin()}; i != this->Palavras.cend(); i++, indice++) // i Aponta para cada palavra
    {
        const auto ocorrencias{this->ContarOcorrencias(*i)};
        for (auto k{this->Palavras.cbegin()}; k != this->Palavras.cend(); k++) // k Também
        {
            unsigned short cont{0};
            for (auto j{this->Frases.cbegin()}; j < this->Frases.cend(); j++) // j Aponta para cada frase
                if (this->ProximaPalavra(*i, *j) == *k)
                    cont++;
            this->MatrizDeProbabilidade->at(indice)->push_back((double)cont / (double)ocorrencias);
        }
    }
}

std::set<std::pair<double,std::string>> ModeloMarkoviano::GetSugestoes(std::string palavra) const
{
    std::set<std::pair<double,std::string>> ret{};
    this->DeixarMinusculo(palavra);
    const auto numcoluna{this->IndiceDaPalavra(palavra)};
    if (numcoluna != -1)
    {
        auto coluna{this->MatrizDeProbabilidade->at(numcoluna)};
        unsigned short indice{0};
        for (auto i{coluna->cbegin()}; i < coluna->cend(); i++, indice++)
            if (*i > 0)
                ret.emplace(std::make_pair(*i, this->PalavraNoIndice(indice)));
    }
    return ret;
}

unsigned int ModeloMarkoviano::GetQtPalavras() const {return this->Palavras.size();}