#ifndef PAGE_TABLE_HPP
#define PAGE_TABLE_HPP

#include <unordered_map>
#include <vector>

class MemoryManager;
struct ProcessData;

// Representa o no da lista ligada
struct FrameNode {
    int pageId;
    int physicalFrame; // O ID real mapeado na RAM global (0-7 ou 8-19)
    int prevIdx;
    int nextIdx;

    FrameNode(int pageId, int realFrame, int last, int nxt) 
        : pageId(pageId), physicalFrame(realFrame), prevIdx(last), nextIdx(nxt) {}
};

class PageTable {
private:
    int activePages;
    int headIdx;
    int tailIdx;
    int offSet;

    MemoryManager* memoryManager;
    ProcessData* process;

    // Tabela de paginas local: guarda o indice do no no vetor localLRU
    std::unordered_map<int, int> pTable;  

    // Vetor que simula a Fila LRU local
    std::vector<FrameNode> localLRU;

    // Funcao auxiliar do pageHit
    int addPage(int page);

public:

    PageTable(MemoryManager* memMgr, ProcessData* proc);

    // Destrutor para liberar os frames fisicos globalmente
    ~PageTable();

    // Funcao chamada quando a CPU referencia uma pagina
    int pageHit(int page);
};

#endif // PAGE_TABLE_HPP