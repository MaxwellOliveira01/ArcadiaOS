#include "real_time_queue.hpp"
#include <stdexcept>

RealTimeQueue::RealTimeQueue() {}

bool RealTimeQueue::enqueue(const ProcessData& process) {
    if (isFull()) {
        return false;
    }
    queue.push_back(process);
    return true;
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
