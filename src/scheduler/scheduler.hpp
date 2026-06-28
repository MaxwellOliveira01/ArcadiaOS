#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <deque>
#include <algorithm>
#include <iostream>
#include "../process/process.hpp"
#include "real_time_queue.hpp"
#include "user_queue.hpp"

class Scheduler {
    private:
        std::deque<ProcessData> globalQueue; // Fila de processos que não foram enfileirados
        RealTimeQueue realTimeQueue;
        UserQueue userQueue;

        const int MAX_WAITING_TIME = 10; // Tempo máximo de espera antes de realimentar o processo (valor arbitrário)

        
    public:
        const int QUANTUM_TIME = 1; // Tempo de quantum para processos de usuário

        Scheduler();

        // Entrada do escalonador
        void scaleProcess(std::vector<ProcessData>& processes);

        void orderProcesses();

        bool admit(ProcessData& process);

        ProcessData* getProcess();

        void feedbackProcess(ProcessData& process);

        bool isEmpty();

        void printQueues();

        void removeGlobalProcess(ProcessData& process);

        void checkWaitingTime();
};

#endif