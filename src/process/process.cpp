#include "process.hpp"
#include <algorithm>

void ProcessManipulator::aging(ProcessData* process) {
    process->priority = std::max(process->priority - 1, 1);
}

void ProcessManipulator::setDiskOperations(ProcessData* process, const std::vector<FileOperation>& ops) {
    for (const auto& op : ops) {
        if (op.pid == process->pid) {
            process->diskOperations.push_back(op);
        }
    }
}

void ProcessManipulator::setMemoryReferences(ProcessData* process, const std::vector<int>& refs) {
    for (const auto& ref : refs) {
        process->memoryReferences.push_back(ref);
    }
}

void ProcessManipulator::incrementWaitingTime(ProcessData* process) {
    process->waitingTime++;
}

void ProcessManipulator::resetWaitingTime(ProcessData* process) {
    process->waitingTime = 0;
}