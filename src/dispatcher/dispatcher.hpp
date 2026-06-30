#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include "../process/process.hpp"
#include "../scheduler/scheduler.hpp"
#include "../memory/memory_manager.hpp"
#include "../memory/page_table.hpp"
#include "../io/resource_manager.hpp"
#include "utils/input_parser.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

class Dispatcher {
    public:
        Dispatcher();

        // Retorna uma representação em string do processo
        std::string toString(const ProcessData& process) const;

        // Verifica se existem processos para serem inicializados
        std::vector<ProcessData*> initProcess(std::vector<ProcessData>* processes, const int currentClock);

        // Verifica se ainda existe processo pendente para ser inicializado
        bool checkPendingProcesses(std::vector<ProcessData>* processes, const int currentClock);

        // Imprime os processos
        void printProcesses(std::vector<ProcessData>& processes, std::vector<std::vector<int>>& pageRefs);

        void start(
            std::vector<ProcessData>& processes,
            Scheduler& scheduler,
            MemoryManager& memoryManager,
            ResourceManager& resourceManager,
            std::unordered_map<int, PageTable>& pageTables
        );

    private:

        void admitNewProcesses(std::vector<ProcessData>& processes, Scheduler& scheduler, int clock);

        bool allDone(std::vector<ProcessData>& processes, Scheduler& scheduler, int clock);
        
        ProcessData* getNext(Scheduler& scheduler, ResourceManager& resourceManager, int& timeUsed);

        void executeOneTick(ProcessData* current, std::unordered_map<int, PageTable>& pageTables, MemoryManager& memoryManager);

        void doMemoryReference(ProcessData* current, std::unordered_map<int, PageTable>& pageTables, MemoryManager& memoryManager);

        bool tryIO(ProcessData* current);

        bool processFinished(ProcessData* current);

        bool quantumExpired(ProcessData* current, int timeUsed, Scheduler& scheduler);

        void finalizeProcess(ProcessData* current, ResourceManager& resourceManager);

};

#endif