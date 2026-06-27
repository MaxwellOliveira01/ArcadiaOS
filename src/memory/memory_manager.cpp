#include "memory_manager.hpp"

// Aloca um frame fisico na memoria
void MemoryManager::allocatePhysicalFrame(int frameId, int pid, int pageId) {
    if (frameId >= 0 && frameId < 20) {
        ram[frameId].isOccupied = true;
        ram[frameId].processId = pid;
        ram[frameId].pageId = pageId;
    }
}

// Libera um frame fisico especifico se necessario
void MemoryManager::freePhysicalFrame(int frameId) {
    if (frameId >= 0 && frameId < 20) {
        ram[frameId].isOccupied = false;
        ram[frameId].processId = -1;
        ram[frameId].pageId = -1;
    }
}