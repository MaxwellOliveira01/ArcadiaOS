// TODO: Fix the implementation
#include "user_queue.hpp"
#include <stdexcept>
#include <string>

UserQueue::UserQueue() {}

ProcessData UserQueue::dequeue() {
    ProcessData process;
    if (!high_priority_queue.empty()) {
        process = high_priority_queue.front();
        high_priority_queue.pop_front();
        
    } else if (!medium_priority_queue.empty()) {
        process = medium_priority_queue.front();
        medium_priority_queue.pop_front();
        
    } else if (!low_priority_queue.empty()) {
        process = low_priority_queue.front();
        low_priority_queue.pop_front();
        
    } else {
        throw std::runtime_error("Todas as filas de usuário estão vazias");
    }
    ProcessManipulator::resetWaitingTime(&process); // Reseta o tempo de espera após a realimentação
    return process;
}

bool UserQueue::enqueue(ProcessData& p) {
    bool isQueued = false;
    switch (p.priority) {
        case 1:
            if (high_priority_queue.size() >= MAX_QUEUE_SIZE) {
                return false; // Fila cheia
            }
            high_priority_queue.push_back(p);
            isQueued = true;
            break;
        case 2:
            if (medium_priority_queue.size() >= MAX_QUEUE_SIZE) {
                return false; // Fila cheia
            }
            medium_priority_queue.push_back(p);
            isQueued = true;
            break;
        default:
            if (low_priority_queue.size() >= MAX_QUEUE_SIZE) {
                return false; // Fila cheia
            }
            low_priority_queue.push_back(p);
            isQueued = true;
            break;
    }
    return isQueued;
}

bool UserQueue::isEmpty() {
    return high_priority_queue.empty() && medium_priority_queue.empty() && low_priority_queue.empty();
}

int UserQueue::size() {
    return high_priority_queue.size() + medium_priority_queue.size() + low_priority_queue.size();
}

void UserQueue::checkWaitingTime(const int& maxWaitingTime) {

    std::vector<ProcessData> toReinsert;
    std::deque<ProcessData>* queues[] = {
        &high_priority_queue,
        &medium_priority_queue,
        &low_priority_queue
    };

    for(auto &q : queues) {
        auto it = q->begin();
        while(it != q->end()) {
            if(it->waitingTime >= maxWaitingTime) {
                // Realimenta o processo para a fila seguinte de m
                ProcessManipulator::aging(&*it);
                ProcessManipulator::resetWaitingTime(&*it); // Reseta o tempo de espera após a realimentação
                toReinsert.push_back(*it);
                it = q->erase(it);
            } else {
                ProcessManipulator::incrementWaitingTime(&*it);
                it = next(it);
            }
        }
    }

    for(auto &p : toReinsert) {
        // Adiciona o processo na fila apropriada
        enqueue(p);
    }
}