// TODO: Fix the implementation
#ifndef USER_QUEUE_HPP
#define USER_QUEUE_HPP

#include <deque>
#include <queue>
#include <vector>
#include <algorithm>
#include "../process/process.hpp"

class UserQueue {
private:
    // 3 filas de prioridade: 0 (alta), 1 (média), 2 (baixa)
    // Cada fila usa priority_queue ordenada por prioridade (preempção por prioridade)    
    std::deque<ProcessData> high_priority_queue;
    std::deque<ProcessData> medium_priority_queue;
    std::deque<ProcessData> low_priority_queue;

    void orderQueues();

public:
    UserQueue();
    
    // Adiciona um novo processo na fila especificada
    // queueLevel: 0 (alta), 1 (média), 2 (baixa)
    // Para novos processos, use queueLevel = 0
    // Para realimentação, o dispatcher incrementa priority e chama com queueLevel apropriado
    bool enqueue(ProcessData& p);
    
    // Remove e retorna o processo de maior prioridade entre todas as filas
    ProcessData dequeue();

    bool isEmpty();

    int size();

};

#endif