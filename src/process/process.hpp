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
    
    // Variavel de controle para o escalonador
    int queueLevel = 0;
    
    int executedTime = 0; // Tempo de CPU já utilizado
    
    bool realTime = false;

    // Operações no disco
    std::vector<FileOperation> diskOperations;

    // Referências a memória
    std::vector<int> memoryReferences;
};

class ProcessManipulator {
    public:
        static void aging(ProcessData* process) {
            // Reduz a prioridade do processo para evitar starvation
            process->priority = max(process->priority - 1, 1);
        }

        static void setDiskOperations(ProcessData* process, const std::vector<FileOperation>&& ops);

        static void setMemoryReferences(ProcessData* process, const std::vector<int>&& refs);
};

#endif