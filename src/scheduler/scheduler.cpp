#include "scheduler.hpp"
#include <stdexcept>

Scheduler::Scheduler() {}

void Scheduler::initAddProcess(ProcessData& process) {
    /* Recebe um processo e o adiciona a uma fila
    se priority = 0 -> fila de tempo real
    se priority != 0 -> fila de usuários
    */
   bool is_queued;
    if (process.priority == 0) {
        process.realTime = true;
        is_queued = realTimeQueue.enqueue(process);
    } else {
        process.realTime = false;
        is_queued = userQueue.enqueue(process);
        
    }
    if (!is_queued) {
        globalQueue.push_back(process);
    }
}

std::tuple<ProcessData, int> Scheduler::getProcess() {
    // Retorna o próximo processo e um inteiro referente a qual fila esse provesso estava.
    int type_queue = -1; // 0 = fila de tempo real, 1 = fila de usuários, -1 = nenhuma fila
    bool is_queued = false;
    ProcessData process;
    if (!realTimeQueue.isEmpty()) {
        process = realTimeQueue.dequeue();
        type_queue = 0; // Indica fila de tempo real
        
    } else if (!userQueue.isQueueEmpty(0) || !userQueue.isQueueEmpty(1) || !userQueue.isQueueEmpty(2)) {
        process = userQueue.dequeue();
        type_queue = 1; // Indica fila de usuários

    }

    ProcessData global_process;
    if (!globalQueue.empty()) {
        if (type_queue == -1 || type_queue == 0) {
            global_process = globalQueue.firstOccurrence(true);
            is_queued = realTimeQueue.enqueue(global_process);

        }else{
            global_process = globalQueue.firstOccurrence(false);
            is_queued = userQueue.enqueue(global_process);
        }

        if (is_queued) {
            globalQueue.pop(global_process);
        }
    }

    return std::make_tuple(process, type_queue);


}

void Scheduler::reAddProcess(ProcessData& process) {
    /* Realoca processo de usuário na fila*/

    // 
    ProcessManipulator::aging(process);

    bool is_queued = userQueue.enqueue(process);

    if (!is_queued) {
        globalQueue.push_back(process);
    }
}

bool Scheduler::isEmpty() const {
    return realTimeQueue.isEmpty() && userQueue.isQueueEmpty(0) && userQueue.isQueueEmpty(1) && userQueue.isQueueEmpty(2) && globalQueue.empty();
}