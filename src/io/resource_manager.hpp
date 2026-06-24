#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "../process/process.hpp"

class ResourceManager {

public:

    const int TOTAL_PRINTERS = 2;
    const int TOTAL_SCANNERS = 1;
    const int TOTAL_MODEMS = 1;
    const int TOTAL_SATA = 2;

    ResourceManager();

    // Tenta alocar todos os recursos que esse processo pede
    // retorna true se consegue pegar TODOS
    // se faltar 1 ou mais, retorna false
    bool tryAllocate(ProcessData& process);

    // Libera todos os recursos que esse processo tinha pegado
    // Não verifica se esse processo já passou pelo tryAllocate
    // isso fica como responsabilidade de quem chama
    void release(ProcessData& process);

private:
    int printers;
    int scanners;
    int modems;
    int sata;
};

#endif