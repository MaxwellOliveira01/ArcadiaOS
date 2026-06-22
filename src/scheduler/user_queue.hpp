#ifndef USER_QUEUE_HPP
#define USER_QUEUE_HPP

#include <deque>
#include <queue>
#include <vector>
#include "../process/process.hpp"

// Estrutura para rastrear processos na fila
struct QueuedProcess {
    ProcessData data;
    
    // Comparador para priority_queue (menor prioridade = maior urgência)
    bool operator>(const QueuedProcess& other) const {
        return data.priority > other.data.priority;
    }
};

class UserQueue {
private:
    // 3 filas de prioridade: 0 (alta), 1 (média), 2 (baixa)
    // Cada fila usa priority_queue ordenada por prioridade (preempção por prioridade)
    std::priority_queue<QueuedProcess, std::deque<QueuedProcess>, std::greater<QueuedProcess>> queues[3];
    
    static const int MAX_QUEUES = 3;
    static const int MAX_PROCESSES_PER_QUEUE = 1000;  // Limite de 1000 processos por fila

    std::vector<int> queuePIDs = {};
    
    // Verifica se uma fila atingiu o limite máximo
    bool isQueueFull(int queueLevel) const;
    
    // Encontra a fila com processo de maior prioridade
    int getHighestPriorityQueue() const;

    // Adiciona o PID do processo à lista de PIDs
    void addPID(int pid);

    // Verifica se o PID já existe na lista e retorna a quantidade de ocorrências.
    int countPIDOccurrences(int pid) const;

public:
    UserQueue();
    
    // Adiciona um novo processo na fila especificada
    // queueLevel: 0 (alta), 1 (média), 2 (baixa)
    // Para novos processos, use queueLevel = 0
    // Para realimentação, o dispatcher incrementa priority e chama com queueLevel apropriado
    void enqueue(const ProcessData& process);
    
    // Remove e retorna o processo de maior prioridade entre todas as filas
    QueuedProcess dequeue();
    
    // Verifica se uma fila especifica está vazia
    bool isQueueEmpty(int queueLevel) const;
    
    // Retorna o número total de processos nas filas
    int size() const;
};

#endif