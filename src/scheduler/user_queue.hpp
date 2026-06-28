// TODO: Fix the implementation
#ifndef USER_QUEUE_HPP
#define USER_QUEUE_HPP

#include <deque>
#include <queue>
#include <vector>
#include <algorithm>
#include "../process/process.hpp"

class UserQueue {
public:
    UserQueue();
    
    // Adiciona um novo processo na fila especificada
    // priority: 0 (alta), 1 (média), 2 (baixa)
    bool enqueue(ProcessData* p);
    
    // Remove e retorna o processo de maior prioridade entre todas as filas
    ProcessData* dequeue();

    // Retorna se as filas estão vazias
    bool isEmpty();

    // Retorna o tamanho total das filas de usuário
    int size();

    // Verifica se um processo está esperando muito tempo para ser escalonado e aplica realimentação se necessário
    void checkWaitingTime(const int& maxWaitingTime);

private:
    // 3 filas de prioridade: 0 (alta), 1 (média), 2 (baixa)
    // Cada fila usa priority_queue ordenada por prioridade (preempção por prioridade)    
    std::deque<ProcessData*> high_priority_queue;
    std::deque<ProcessData*> medium_priority_queue;
    std::deque<ProcessData*> low_priority_queue;

    const long unsigned int MAX_QUEUE_SIZE = 1000; // Tamanho máximo da fila de processos de usuário
};

#endif