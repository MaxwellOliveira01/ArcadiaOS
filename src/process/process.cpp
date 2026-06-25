#include "process.hpp"

void ProcessManipulator::setDiskOperations(ProcessData* process, const std::vector<FileOperation>&& ops) {
    for (const auto& op : ops) {
        if (op.pid == process->pid) {
            process->diskOperations.push_back(op);
        }
    }
}

void ProcessManipulator::setMemoryReferences(ProcessData* process, const std::vector<int>&& refs) {
    for (const auto& ref : refs) {
        process->memoryReferences.push_back(ref);
    }
}