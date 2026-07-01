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
            ResourceManager& resourceManager
        );

    private:
        // Lista de usos de IO por processo, cada Id de processo possui um IOAllocation que indica quantos recursos de cada tipo ele está usando
        std::map<int, IOAllocation> ListIOUsed;

        void admitNewProcesses(std::vector<ProcessData>& processes, Scheduler& scheduler, int clock);

        bool allDone(std::vector<ProcessData>& processes, Scheduler& scheduler, int clock);
        
        ProcessData* getNext(Scheduler& scheduler, int& timeUsed);

        void executeOneTick(ProcessData* current, PageTable& actPageTable, ResourceManager& resourceManager);

        void doMemoryReference(ProcessData* current, PageTable& actPageTable);

        void tryIO(ProcessData* current, ResourceManager& resourceManager);

        bool processFinished(ProcessData* current);

        bool quantumExpired(ProcessData* current, int timeUsed, Scheduler& scheduler);

        void freeResources(ProcessData* current, ResourceManager& resourceManager);

        // Tira o processo da lista de processos que estão usando IO
        void clearListIOUsed(int pid);

};

#endif