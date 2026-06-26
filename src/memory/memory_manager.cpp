#include "memory_manager.hpp"

void MemoryManager::allocatePhysicalFrame(int frameId, int pid, int pageId) {
    if (frameId >= 0 && frameId < 20) {
        ram[frameId].isOccupied = true;
        ram[frameId].processId = pid;
        ram[frameId].pageId = pageId;
    }
}