#include "user_queue.hpp"
#include <stdexcept>
#include <vector>

UserQueue::UserQueue() : currentProcess(nullptr), currentQueueLevel(-1), totalWaitingTime(0) {}

UserQueue::~UserQueue() {
    if (currentProcess) {
        delete currentProcess;
        currentProcess = nullptr;
    }
}

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

void UserQueue::performAging() {
    // Aumenta a prioridade de processos que esperam há muito tempo
    // Reduzindo seu valor de prioridade (prioridade = 0 é máximo)
    for (int i = 1; i < MAX_QUEUES; i++) {  // Começa na fila 1
        // Extrair todos, incrementar waitingTime, re-adicionar se necessário
        std::vector<QueuedProcess> temp;
        while (!queues[i].empty()) {
            QueuedProcess queued = queues[i].top();
            queues[i].pop();
            
            queued.waitingTime++;
            // Se esperou muito, promove para fila anterior
            if (queued.waitingTime >= AGING_THRESHOLD && i > 0) {
                queued.data.priority--;  // Aumenta a prioridade
                queued.waitingTime = 0;  // Reset do contador
                queues[i - 1].push(queued);  // Promove para fila anterior
            } else {
                temp.push_back(queued);
            }
        }
        // Re-adicionar processos que não foram promovidos
        for (auto& p : temp) {
            queues[i].push(p);
        }
    }
}

void UserQueue::enqueue(const ProcessData& process) {
    QueuedProcess qp;
    qp.data = process;
    qp.quantumUsed = 0;
    qp.waitingTime = 0;
    
    // Novos processos entram na fila 0 (maior prioridade)
    queues[0].push(qp);
}

QueuedProcess UserQueue::dequeue() {
    // Se há um processo em execução, não pode fazer dequeue
    if (currentProcess != nullptr) {
        throw std::logic_error("Ja existe um processo em execucao. Chame finishCurrentProcess() primeiro.");
    }
    
    int highestQueue = getHighestPriorityQueue();
    
    if (highestQueue == -1) {
        throw std::underflow_error("Nenhum processo disponivel nas filas");
    }
    
    // Realiza aging antes de selecionar
    performAging();
    
    // Revalidar após aging (em caso de mudança de filas)
    highestQueue = getHighestPriorityQueue();
    
    QueuedProcess next = queues[highestQueue].top();
    queues[highestQueue].pop();
    
    // Define como processo atual
    currentProcess = new QueuedProcess(next);
    currentQueueLevel = highestQueue;
    currentProcess->quantumUsed = 0;
    
    return next;
}

void UserQueue::tick() {
    if (currentProcess == nullptr) {
        return;  // Nenhum processo em execução
    }
    
    currentProcess->quantumUsed++;
    
    // Se completou o quantum (1ms), move para próxima fila
    if (currentProcess->quantumUsed >= QUANTUM_MS) {
        // Não move para próxima fila se já está na fila 2
        if (currentQueueLevel < MAX_QUEUES - 1) {
            currentProcess->data.priority++;  // Reduz a prioridade
            currentProcess->quantumUsed = 0;
            currentProcess->waitingTime = 0;
            
            // Adiciona na próxima fila
            queues[currentQueueLevel + 1].push(*currentProcess);
        } else {
            // Já está na fila 2, volta para o início da fila 2
            currentProcess->quantumUsed = 0;
            currentProcess->waitingTime = 0;
            queues[MAX_QUEUES - 1].push(*currentProcess);
        }
        
        // Limpa o processo atual
        delete currentProcess;
        currentProcess = nullptr;
        currentQueueLevel = -1;
    }
    
    // Incrementa contador total para aging
    totalWaitingTime++;
}

bool UserQueue::isEmpty() const {
    if (currentProcess != nullptr) {
        return false;  // Há um processo em execução
    }
    
    for (int i = 0; i < MAX_QUEUES; i++) {
        if (!queues[i].empty()) {
            return false;
        }
    }
    return true;
}

int UserQueue::size() const {
    int total = (currentProcess != nullptr) ? 1 : 0;
    
    for (int i = 0; i < MAX_QUEUES; i++) {
        total += queues[i].size();
    }
    return total;
}

QueuedProcess* UserQueue::getCurrentProcess() const {
    return currentProcess;
}

int UserQueue::getCurrentQueueLevel() const {
    return currentQueueLevel;
}

void UserQueue::finishCurrentProcess() {
    if (currentProcess != nullptr) {
        delete currentProcess;
        currentProcess = nullptr;
        currentQueueLevel = -1;
    }
}
