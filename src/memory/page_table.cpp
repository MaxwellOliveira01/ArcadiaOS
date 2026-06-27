#include "page_table.hpp"
#include "memory_manager.hpp"
#include "process.hpp"

// Inicializa o contexto de memoria local do processo (construtor)
PageTable::PageTable(MemoryManager* memMgr, ProcessData* proc) 
    : memoryManager(memMgr), process(proc), pTable(), localLRU() {
    
    activePages = 1;
    offSet = 0;

    if (!process->priority) {
        offSet = 8; // Processos de usuario começam no frame fisico 8
    }

    int firstPage = process->memoryReferences[0];
    pTable[firstPage] = 0; // Mapeia a pagina inicial para o indice 0 do vetor
    localLRU.emplace_back(firstPage, offSet, -1, -1);
    headIdx = tailIdx = 0;
}

PageTable::~PageTable() {
    // Varre todas as paginas mapeadas e desaloca seus frames fisicos na RAM global
    for (const auto& node : localLRU) {
        if (node.physicalFrame != -1) {
            memoryManager->freePhysicalFrame(node.physicalFrame);
        }
    }
}

// pageHit : Simula a CPU referenciando uma pagina virtual
int PageTable::pageHit(int page) {
    int frameIdx;
    
    // Se a pagina nao for encontrada na tabela local, ocorre um Page Fault
    if (pTable.find(page) == pTable.end()) {
        frameIdx = addPage(page); // addPage ja incrementa o fault e retorna o frameIdx correto
    }
    else {
        // Page Hit: a pagina ja esta mapeada na RAM local deste processo
        frameIdx = pTable[page];
        
        // Caso a pagina seja o Head (menos recentemente usada), ela vai para o final
        if (frameIdx == headIdx) {
            if (frameIdx != tailIdx) { 
                headIdx = localLRU[frameIdx].nextIdx;
                localLRU[headIdx].prevIdx = -1;
                localLRU[tailIdx].nextIdx = frameIdx;
                localLRU[frameIdx].prevIdx = tailIdx;
                localLRU[frameIdx].nextIdx = -1;
                tailIdx = frameIdx;
            }
        }
        // Caso a pagina esteja no meio da fila, desconecta e move para o final
        else if (frameIdx != tailIdx) {
            int lastFrame = localLRU[frameIdx].prevIdx;
            int nextFrame = localLRU[frameIdx].nextIdx;

            // Junta o seu frame anterior com o seu sucessor
            localLRU[lastFrame].nextIdx = nextFrame;
            localLRU[nextFrame].prevIdx = lastFrame;

            // Atualiza voce como o novo Tail (mais recentemente usada)
            localLRU[tailIdx].nextIdx = frameIdx;
            localLRU[frameIdx].prevIdx = tailIdx;
            localLRU[frameIdx].nextIdx = -1;
            tailIdx = frameIdx;
        }
        // else -> a pagina eh o tail, e nao precisa ser modificada
    }
    
    return frameIdx; // Retorna o indice logico do vetor
}

// addPage : Gerencia o Page Fault e a substituicao local se necessario
int PageTable::addPage(int page) {
    process->pageFaults++;
    
    // Se o processo ainda nao atingiu o limite maximo de seu Working Set
    if (activePages < process->workingSetSize) {
        pTable[page] = activePages;
        
        // Cria um novo no no final do vetor
        localLRU.emplace_back(page, activePages + offSet, tailIdx, -1);
        localLRU[tailIdx].nextIdx = activePages;
        tailIdx = activePages;
        activePages++;
    }
    else {
        // Pega o indice da pagina que esta ha mais tempo sem uso
        int lastFrame = headIdx;
        
        // Remove a associacao da pagina antiga da tabela de paginas
        int trashPage = localLRU[headIdx].pageId;
        pTable.erase(trashPage);
        
        // Atualiza quem sera o novo Head
        if (localLRU[lastFrame].nextIdx != -1) { 
            headIdx = localLRU[lastFrame].nextIdx;
            localLRU[headIdx].prevIdx = -1;
        }
        
        // Sobrescreve os dados do frame antigo com as informacoes da pagina nova
        localLRU[lastFrame].pageId = page;
        localLRU[lastFrame].prevIdx = tailIdx;
        localLRU[lastFrame].nextIdx = -1;
        pTable[page] = lastFrame;  // associa a pagina nova ao frame

        // Atualiza p tail
        localLRU[tailIdx].nextIdx = lastFrame;
        tailIdx = lastFrame;
    }

    memoryManager->allocatePhysicalFrame(localLRU[tailIdx].physicalFrame, process->pid, localLRU[tailIdx].pageId);
    
    return tailIdx; // Retorna o indice logico onde a pagina foi alocada
}