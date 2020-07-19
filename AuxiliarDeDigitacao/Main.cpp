#include <iostream>
#include <chrono>
#include "ModeloMarkoviano.hpp"

int main(void)
{
    // Inicialização:
    auto inicio{std::chrono::system_clock::now()};
    ModeloMarkoviano modelo{"dataset.txt"};
    auto fim{std::chrono::system_clock::now()};
    std::chrono::duration<double,std::milli> tempo{fim - inicio};
    std::cout << "\nEntrada de dados concluída em " << tempo.count() << " Milissegundos!";
    inicio = std::chrono::system_clock::now();
    modelo.ProcessarEntradas();
    fim = std::chrono::system_clock::now();
    tempo = (fim - inicio);
    std::cout << "\nProcessamento das entradas concluído em " << tempo.count() << " Milissegundos!";
    std::cout << "\n\nO dataset lido possui " << modelo.GetQtPalavras() << " palavras diferentes!";
    // Teste:
    std::cout << "\n\n----------------------------------------------------------\n";
    std::string frase, palavra;
    std::cout << "\nDigite a primeira palavra: ";
    std::cin >> palavra;
    frase = palavra;
    while (true)
    {
        std::cout << "\nFrase até agora: " << frase << "\n";
        const auto sugestoes{modelo.GetSugestoes(palavra)};
        if (sugestoes.size() > 0)
        {
            unsigned short cont{1};
            for (auto i{sugestoes.cbegin()}; (i != sugestoes.cend() && cont <= NumPalavrasSugerir); i++, cont++)
                if (i->first > 0)
                    std::cout << "\n" << (cont) << ": " << (i->second);
            std::cout << "\n\nEscolha uma opção ou digite a próxima palavra: ";
            std::cin >> palavra;
            if (palavra.size() == 1 && (palavra[0] >= 48 && palavra[0] <= 57))
            {
                unsigned short opcao = (palavra[0] - 48);
                if (opcao <= sugestoes.size())
                    for (auto i{sugestoes.cbegin()}; (i != sugestoes.cend() && opcao > 0); i++, opcao--)
                        palavra = i->second;
            }
        }
        else
        {
            std::cout << "\nSem Sugestões\n\nDigite a próxima palavra: ";
            std::cin >> palavra;
        }
        frase += " ";
        frase += palavra;
    }
    return 0;
}