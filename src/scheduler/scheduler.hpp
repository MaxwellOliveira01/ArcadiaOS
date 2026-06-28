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
        // Escalona os processos em suas respectivas filas
        void scaleProcess(std::vector<ProcessData>& processes);

        void orderProcesses();

        // Tenta escalonar os processos na fila de tempo real ou de usuário.
        bool admit(ProcessData& process);

        // Retorna o processo de maior priodiade
        ProcessData* getProcess();

        // Tenta realocar o processo da fila global em uma fila específica.
        void feedbackProcess(ProcessData& process);

        // Verifica se todas as filas estão vazias
        bool isEmpty();

        // Imprime o estado atual das filas
        void printQueues();

        // Remove um processo da fila global
        void removeGlobalProcess(ProcessData& process);

        // Verifica o tempo de espera de cada processo na fila de usuário e aplica realimentação se necessário
        void checkWaitingTime();
};

#endif