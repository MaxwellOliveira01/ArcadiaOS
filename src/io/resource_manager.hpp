#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "../process/process.hpp"

struct IOAllocation {
    int printersUsed;
    int scannersUsed;
    int modemsUsed;
    int sataUsed;
};

class ResourceManager {

public:

    static constexpr int TOTAL_PRINTERS = 2;
    static constexpr int TOTAL_SCANNERS = 1;
    static constexpr int TOTAL_MODEMS = 1;
    static constexpr int TOTAL_SATA = 2;

    ResourceManager();

    // Tenta alocar todos os recursos que esse processo pede
    // retorna true se consegue pegar TODOS
    // se faltar 1 ou mais, retorna false
    IOAllocation tryAllocate(ProcessData& process);

    // Libera todos os recursos que esse processo tinha pegado
    // Não verifica se esse processo já passou pelo tryAllocate
    // isso fica como responsabilidade de quem chama
    void release(const IOAllocation& ioAlloc);

private:
    int printers;
    int scanners;
    int modems;
    int sata;
};

#endif