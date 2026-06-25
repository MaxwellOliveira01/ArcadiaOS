#include "global_queue.hpp"
#include <algorithm>

void GlobalQueue::push_back(const ProcessData& process) {
    queue.push_back(process);
}

bool GlobalQueue::empty() const {
    return queue.empty();
}

void GlobalQueue::pop(ProcessData& process) {
    // Remove o processo da fila global
    auto it = std::find_if(queue.begin(), queue.end(), [&](const ProcessData& p) {
        return p.pid == process.pid;
    });
    if (it != queue.end()) {
        queue.erase(it);
    }
}

ProcessData GlobalQueue::firstOccurrence(const bool& realTime) {
    // Retorna a primeira ocorrência de um processo na fila global que tiver valor de realtime igual ao parâmetro realTime
    auto it = std::find_if(queue.begin(), queue.end(), [&](const ProcessData& p) {
        return p.realTime == realTime;
    });
    if (it != queue.end()) {
        return *it;
    } else {
        return ProcessData{}; // Retorna um processo vazio se não houver ocorrência
    }
}