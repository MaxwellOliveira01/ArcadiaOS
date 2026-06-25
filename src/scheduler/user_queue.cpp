// TODO: Fix the implementation
#include "user_queue.hpp"
#include <stdexcept>
#include <string>

UserQueue::UserQueue() {}

ProcessData UserQueue::dequeue() {
    if (!high_priority_queue.empty()) {
        ProcessData process = high_priority_queue.front();
        high_priority_queue.pop_front();
        process.queueLevel = 1;
        return process;
    } else if (!medium_priority_queue.empty()) {
        ProcessData process = medium_priority_queue.front();
        medium_priority_queue.pop_front();
        process.queueLevel = 2;
        return process;
    } else if (!low_priority_queue.empty()) {
        ProcessData process = low_priority_queue.front();
        low_priority_queue.pop_front();
        process.queueLevel = 2; // Mantém o nível da fila baixa
        return process;
    } else {
        throw std::runtime_error("Todas as filas de usuário estão vazias");
    }
}

bool UserQueue::enqueue(ProcessData& p) {
    bool isQueued = false;
    switch (p.queueLevel) {
        case 0:
            high_priority_queue.push_back(p);
            isQueued = true;
            break;
        case 1:
            medium_priority_queue.push_back(p);
            isQueued = true;
            break;
        case 2:
            low_priority_queue.push_back(p);
            isQueued = true;
            break;
        default:
            return false;  // Nível de fila inválido
    }
    orderQueues();
    return isQueued;
}

bool UserQueue::isEmpty() {
    return high_priority_queue.empty() && medium_priority_queue.empty() && low_priority_queue.empty();
}

int UserQueue::size() {
    return high_priority_queue.size() + medium_priority_queue.size() + low_priority_queue.size();
}

void UserQueue::orderQueues() {
    auto compare = [](const ProcessData& a, const ProcessData& b) {
        return a.priority > b.priority; // Maior prioridade primeiro
    };

    std::sort(high_priority_queue.begin(), high_priority_queue.end(), compare);
    std::sort(medium_priority_queue.begin(), medium_priority_queue.end(), compare);
    std::sort(low_priority_queue.begin(), low_priority_queue.end(), compare);
}