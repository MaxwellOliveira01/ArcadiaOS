#include "scheduler.hpp"
#include <stdexcept>

Scheduler::Scheduler() {}

bool Scheduler::isEmpty() const {
    return processes.empty();
}

void Scheduler::addProcess(const ProcessData& process) {
    // Esse método aloca o processo na exata posição de acordo com o valor de InitTime.
    auto it = processes.begin();
    while (it != processes.end() && it->initTime <= process.initTime) {
        ++it;
    }
    processes.insert(it, process);
}

ProcessData Scheduler::getProcess() {
    if (isEmpty()) {
        throw std::runtime_error("Nenhum processo disponivel para execucao");
    }
    ProcessData process = processes.front();
    processes.pop_front();
    return process;
}

void Scheduler::scale(const ProcessData& process, RealTimeQueue& realTimeQueue, UserQueue& userQueue, bool& executed) {
    // Esse método é responsável por realocar um processo para a fila de prioridade inferior.
    // Ele remove o processo da fila atual e o adiciona novamente com prioridade reduzida.
    ProcessData scaledProcess = process;
    if (scaledProcess.priority == 0 && !executed) {
        // Se o processo for de alta prioridade e não tiver sido executado, aloca em realTimeQueue
        realTimeQueue.enqueue(scaledProcess);
    } else {
        // Caso contrário, aloca em userQueue.
        userQueue.enqueue(scaledProcess);
    }

