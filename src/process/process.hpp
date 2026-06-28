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
        // Reduz a prioridade do processo para evitar starvation
        static void aging(ProcessData* process);

        // Define as operações de disco que o processo deverá realizar.
        static void setDiskOperations(ProcessData* process, const std::vector<FileOperation>&& ops);

        // Define as referências de memória que o processo deverá referenciar.
        static void setMemoryReferences(ProcessData* process, const std::vector<int>&& refs);

        // Incrementa o tempo de espera do processo na fila.
        static void incrementWaitingTime(ProcessData* process);

        // Reseta o tempo de espera do processo na fila.
        static void resetWaitingTime(ProcessData* process);
};

#endif