#include "scheduler.hpp"
#include <stdexcept>

Scheduler::Scheduler() {}

void Scheduler::orderProcesses() {
    // Ordena a fila global de processos por prioridade (menor prioridade primeiro)
    std::sort(globalQueue.begin(), globalQueue.end(), [](const ProcessData& a, const ProcessData& b) {
        return a.priority < b.priority; // Menor prioridade primeiro
    });
}

void Scheduler::scaleProcess(std::vector<ProcessData>& processes) {
    // Adiciona todos os processos à fila global e ordena por prioridade
    globalQueue.insert(globalQueue.end(), processes.begin(), processes.end());
    orderProcesses();

    // Admite os processos na fila apropriada
    for (auto& process : globalQueue) {
        if (admit(process)) {
            // Se admitido, remove da fila global
            removeGlobalProcess(process);
        }
    }
}


bool Scheduler::admit(ProcessData& process) {
    // Admite o processo na fila apropriada
    if (process.realTime) {
        return realTimeQueue.enqueue(process);
    } else {
        return userQueue.enqueue(process);
    }
}

ProcessData* Scheduler::getProcess() {
    // Retorna o próximo processo a ser executado
    static ProcessData process;
    ProcessData *globalProcess = globalQueue.empty() ? nullptr : &globalQueue.front();
    
    if (!realTimeQueue.isEmpty()) {
        process = realTimeQueue.dequeue();
    } else if (!userQueue.isEmpty()) {
        process = userQueue.dequeue();
    } else {
        throw std::runtime_error("Nenhum processo disponível para execução");
    }

    if (globalProcess != nullptr) {
        // Se houver um processo na fila global, tenta adiciona-lo à fila apropriada
        if (admit(*globalProcess)) {
            removeGlobalProcess(*globalProcess);
        }
    }
    return &process;
}

void Scheduler::feedbackProcess(ProcessData& process) {
    // Realimenta o processo de volta à fila apropriada
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

void Scheduler::removeGlobalProcess(ProcessData& process) {
    auto it = std::find_if(globalQueue.begin(), globalQueue.end(), [&process](const ProcessData& p) {
        return p.pid == process.pid;
    });
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