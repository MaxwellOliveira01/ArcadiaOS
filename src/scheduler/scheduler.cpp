#include "scheduler.hpp"
#include <stdexcept>

Scheduler::Scheduler() {}

// Ordena a fila global de processos por prioridade (menor prioridade primeiro)
void Scheduler::orderProcesses() {
    std::sort(globalQueue.begin(), globalQueue.end(), [](const ProcessData* a, const ProcessData* b) {
        return a->priority < b->priority; // Menor prioridade primeiro
    });
}

void Scheduler::scaleProcess(std::vector<ProcessData*>& processes) {
    globalQueue.insert(globalQueue.end(), processes.begin(), processes.end());
    orderProcesses();
    
    // Admite os processos na fila apropriada
    auto it = globalQueue.begin();
    while(it != globalQueue.end()) {
        if(admit(*it)) {
            // Se admitido, remove da fila global
            it = globalQueue.erase(it);
        } else {
            it = next(it);
        }
    }

}

bool Scheduler::admit(ProcessData* process) {
    if (process->realTime) {
        return realTimeQueue.enqueue(process);
    } else {
        return userQueue.enqueue(process);
    }
}

ProcessData* Scheduler::getProcess() {
    ProcessData* process = nullptr;
    ProcessData* globalProcess = globalQueue.empty() ? nullptr : globalQueue.front();
    
    if (!realTimeQueue.isEmpty()) {
        process = realTimeQueue.dequeue();
    } else if (!userQueue.isEmpty()) {
        process = userQueue.dequeue();
    } else {
        throw std::runtime_error("Nenhum processo disponível para execução");
    }

    if (globalProcess != nullptr && admit(globalProcess)) {
        // Se houver um processo na fila global, tenta adiciona-lo à fila apropriada
        removeGlobalProcess(globalProcess);
    }

    return process;
}

void Scheduler::feedbackProcess(ProcessData* process) {
    
    process->priority = std::min(process->priority + 1, 3);

    if (!admit(process)) {
        globalQueue.push_back(process); // Se não puder admitir, mantém na fila global
    }
}

bool Scheduler::isEmpty() {
    return globalQueue.empty() && realTimeQueue.isEmpty() && userQueue.isEmpty();
}


void Scheduler::printQueues() {
    std::cout << "Global Queue: " << globalQueue.size() << " processes\n";
    std::cout << "Real-Time Queue: " << realTimeQueue.size() << " processes\n";
    std::cout << "User Queue: " << userQueue.size() << " processes\n";
}

void Scheduler::removeGlobalProcess(ProcessData* process) {
    auto it = std::find(globalQueue.begin(), globalQueue.end(), process);
    if (it != globalQueue.end()) {
        globalQueue.erase(it);
    }
}

void Scheduler::checkWaitingTime() {
    // Verifica o tempo de espera de cada processo na fila
    if (!userQueue.isEmpty()) {
        userQueue.checkWaitingTime(MAX_WAITING_TIME);
    }
}