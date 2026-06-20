#include "user_queue.hpp"
#include <stdexcept>
#include <string>

UserQueue::UserQueue() {}

int UserQueue::getHighestPriorityQueue() const {
    // Retorna a fila com maior prioridade que não está vazia
    // Fila 0 > Fila 1 > Fila 2
    for (int i = 0; i < MAX_QUEUES; i++) {
        if (!queues[i].empty()) {
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

void UserQueue::enqueue(const ProcessData& process, int queueLevel) {
    // Validar nível da fila
    if (queueLevel < 0 || queueLevel >= MAX_QUEUES) {
        throw std::invalid_argument("Nivel de fila invalido. Deve estar entre 0 e 2.");
    }
    
    // Verificar se a fila está cheia
    if (isQueueFull(queueLevel)) {
        throw std::overflow_error("Fila " + std::to_string(queueLevel) + 
                                " cheia: limite de 1000 processos atingido");
    }
    
    QueuedProcess qp;
    qp.data = process;
    
    // Adiciona na fila especificada
    queues[queueLevel].push(qp);
}

QueuedProcess UserQueue::dequeue() {
    int highestQueue = getHighestPriorityQueue();
    
    if (highestQueue == -1) {
        throw std::underflow_error("Nenhum processo disponivel nas filas");
    }
    
    // Retorna o processo de maior prioridade da fila de maior prioridade
    QueuedProcess next = queues[highestQueue].top();
    queues[highestQueue].pop();
    
    return next;
}

bool UserQueue::isEmpty() const {
    for (int i = 0; i < MAX_QUEUES; i++) {
        if (!queues[i].empty()) {
            return false;
        }
    }
    return true;
}

int UserQueue::size() const {
    int total = 0;
    
    for (int i = 0; i < MAX_QUEUES; i++) {
        total += queues[i].size();
    }
    return total;
}
