#ifndef PAGE_TABLE_HPP
#define PAGE_TABLE_HPP

#include <unordered_map>
#include <vector>

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
    int pageFaults;
    int headIdx;
    int tailIdx;
    int workingSetSize;
    int offSet;

    // Funcao auxiliar do pageHit
    int addPage(int page);

public:

    PageTable(int workingSetSize, int firstPage, bool realTime);
    
    // Tabela de paginas local: guarda o indice do no no vetor localLRU
    std::unordered_map<int, int> pTable;  
    
    // Vetor que simula a Fila LRU local
    std::vector<FrameNode> localLRU;

    // Funcao chamada quando a CPU referencia uma pagina
    int pageHit(int page);

    // Getters uteis
    int getPageFaults() const { return pageFaults; }
    int getActivePages() const { return activePages; }
};

#endif // PAGE_TABLE_HPP