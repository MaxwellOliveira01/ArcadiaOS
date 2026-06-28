#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "../filesystem/file_system.hpp"

struct ProcessData {
    int pid;
    int initTime;
    int priority;
    int cpuTime;
    int workingSetSize;
    int pageFaults = 0;
    bool requiresPrinter;
    bool requiresScanner;
    bool requiresModem;
    bool requiresSata;
    
    int executedTime = 0; // Tempo de CPU já utilizado
    
    bool realTime = false;
    int waitingTime = 0; // Tempo de espera na fila

    // Operações no disco
    std::vector<FileOperation> diskOperations;

    // Referências a memória
    std::vector<int> memoryReferences;
};

class ProcessManipulator {
    public:
        static void aging(ProcessData* process) {
            // Reduz a prioridade do processo para evitar starvation
            process->priority = std::max(process->priority - 1, 1);
        }

        static void setDiskOperations(ProcessData* process, const std::vector<FileOperation>&& ops);

        static void setMemoryReferences(ProcessData* process, const std::vector<int>&& refs);

        static void incrementWaitingTime(ProcessData* process) {
            process->waitingTime++;
        }

        static void resetWaitingTime(ProcessData* process) {
            process->waitingTime = 0;
        }
};

#endif