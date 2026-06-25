#include "user_queue.hpp"
#include <stdexcept>
#include <string>

UserQueue::UserQueue() {}

int UserQueue::getHighestPriorityQueue() const {
    // Retorna a fila com maior prioridade que não está vazia
    // Fila 0 > Fila 1 > Fila 2
    for (int i = 0; i < MAX_QUEUES; i++) {
        if (!isQueueEmpty(i)) {
            return i;
        }
    }
    return -1;  // Todas as filas vazias
}

bool UserQueue::isQueueFull(int queueLevel) const {
    // Verifica se uma fila atingiu o limite máximo de 1000 processos
    if (queueLevel < 0 || queueLevel >= MAX_QUEUES) {
        return false;
    }
    return queues[queueLevel].size() >= MAX_PROCESSES_PER_QUEUE;
}

bool UserQueue::enqueue(ProcessData& process) {
    // Verifica a quantidade de ocorrências do processo na lista de PIDs.
    int actualLevel = process.queueLevel;

    switch (actualLevel) {
        case 0:
            if (isQueueFull(actualLevel)) {
                return false;  // Fila de alta prioridade cheia
            }
            process.cpuTime = 1;
            queues[actualLevel].push(QueuedProcess{process});
            
            return true;
        case 1:
            if (isQueueFull(actualLevel)) {
                return false;  // Fila de média prioridade cheia
            }
            process.cpuTime = 2;
            queues[actualLevel].push(QueuedProcess{process});
            return true;
        default:
            if (isQueueFull(actualLevel)) {
                return false;  // Fila de baixa prioridade cheia
            }
            process.cpuTime = 2; // Mantém na fila de baixa prioridade
            queues[actualLevel].push(QueuedProcess{process});
            return true;
    }
}

ProcessData UserQueue::dequeue() {
    int highestPriorityQueue = getHighestPriorityQueue();
    if (highestPriorityQueue == -1) {
        throw std::runtime_error("Todas as filas estão vazias");
    }
    QueuedProcess process = queues[highestPriorityQueue].top();
    queues[highestPriorityQueue].pop();
    ProcessData p = process.data;
    return p;
}

bool UserQueue::isQueueEmpty(int queueLevel) const {
    if (queueLevel < 0 || queueLevel >= MAX_QUEUES) {
        throw std::invalid_argument("Nivel de fila invalido: " + std::to_string(queueLevel));
    }
    return queues[queueLevel].empty();
}

int UserQueue::size() const {
    int total = 0;
    
    for (int i = 0; i < MAX_QUEUES; i++) {
        total += queues[i].size();
    }
    return total;
}
