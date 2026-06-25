#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <deque>
#include "../process/process.hpp"
#include "real_time_queue.hpp"
#include "user_queue.hpp"
#include "global_queue.hpp"

class Scheduler {
    private:
        GlobalQueue globalQueue; // Fila de processos que não foram enfileirados
        RealTimeQueue realTimeQueue;
        UserQueue userQueue;
        
    public:
        const int QUANTUM_TIME = 1; // Tempo de quantum para processos de usuário

        Scheduler();

        void initAddProcess(ProcessData& process);

        std::tuple<ProcessData, int> getProcess();

        void reAddProcess(ProcessData& process);

        bool isEmpty() const;

};

#endif