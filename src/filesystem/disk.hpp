#ifndef DISK_HPP
#define DISK_HPP

#include <string>
#include <vector>

using namespace std; // acho que vai dar conflito

class Disk {
public:

    Disk(int totalBlocks);

    // Aloca usando o algoritmo first-fit
    // retorna o primeiro bloco se der certo
    // retorna -1 se der errado
    int tryAllocate(string name, int numBlocks); // first fit

    // libera um range de numBlocks comecando de startBlock
    void freeBlocks(int startBlock, int numBlocks);

    // vai user usado pra inicializar com arquivos que ja existiriam na memoria
    // tem que ser diferente do allocate pq aqui é posicao fixa,
    // nao tem firstFit
    // se tiver sobreposicao com algum outro OU se estourar a memoria, retorna falso
    // caso contrario deu certo e retorna verdadeiro
    bool placeExisting(string name, int start, int numBlocks);

    // imprime o mapa, "0" se o bloco tiver livre
    // ou o nome do processo que tem aquele bloco
    string getMemoryMap();

private:
    
    vector<string> blocks;

    // verifica se um range tá disponivel
    bool isRangeAvailable(int start, int numBlocks);

    // seta o range pra `name`, nao verifica se tá disponivel
    // confia que quem chama verifica isRangeAvailableAntes
    void allocate(string name, int start, int numBlocks);

};

#endif