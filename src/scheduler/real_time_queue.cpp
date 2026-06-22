#include "real_time_queue.hpp"
#include <stdexcept>

RealTimeQueue::RealTimeQueue() {}

void RealTimeQueue::enqueue(const ProcessData& process) {
    if (isFull()) {
        throw std::overflow_error("Fila cheia: limite de 1000 processos atingido");
    }
    queue.push_back(process);
}

ProcessData RealTimeQueue::dequeue() {
    if (isEmpty()) {
        throw std::underflow_error("Fila vazia: nenhum processo disponivel para execucao");
    }
    ProcessData process = queue.front();
    queue.pop_front();
    return process;
}

bool RealTimeQueue::isEmpty() const {
    return queue.empty();
}

int RealTimeQueue::size() const {
    return queue.size();
}

bool RealTimeQueue::isFull() const {
    return queue.size() >= MAX_PROCESSES;
}
