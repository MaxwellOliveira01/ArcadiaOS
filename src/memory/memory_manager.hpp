#ifndef MEMORY_MANAGER_HPP
#define MEMORY_MANAGER_HPP

#include <vector>
#include "page_table.hpp"

// Abstracao da RAM fisica simulada
struct PhysicalFrame {
    bool isOccupied = false;
    int processId = -1;
    int pageId = -1;
};

class MemoryManager {
private:
    // A mempria RAM simulada de tamanho fixo 20 (0-7 Kernel, 8-19 Usuario)
    std::vector<PhysicalFrame> ram;

public:
    MemoryManager() : ram(20) {}

    void allocatePhysicalFrame(int frameId, int pid, int pageId);
};

#endif // MEMORY_MANAGER_HPP