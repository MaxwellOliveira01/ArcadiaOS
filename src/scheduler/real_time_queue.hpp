#ifndef REAL_TIME_QUEUE_HPP
#define REAL_TIME_QUEUE_HPP

#include <deque>
#include "../process/process.hpp"

class RealTimeQueue {
private:
    std::deque<ProcessData> queue;
    static const int MAX_PROCESSES = 1000;

public:
    RealTimeQueue();
    
    // Adiciona um processo no final da fila (FIFO)
    bool enqueue(const ProcessData& process);
    
    // Remove e retorna o primeiro processo da fila
    ProcessData dequeue();
    
    // Verifica se a fila está vazia
    bool isEmpty() const;
    
    // Retorna o número de processos atualmente na fila
    int size() const;
    
    // Verifica se a fila atingiu o limite máximo (1000 processos)
    bool isFull() const;
};

#endif