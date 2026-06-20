#ifndef USER_QUEUE_HPP
#define USER_QUEUE_HPP

#include <deque>
#include <queue>
#include "../process/process.hpp"

// Estrutura para rastrear informações adicionais do processo na fila
struct QueuedProcess {
    ProcessData data;
    int quantumUsed;      // Tempo usado neste quantum (ms)
    int waitingTime;      // Tempo esperando na fila (para aging)
    
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
    
    QueuedProcess* currentProcess;  // Processo sendo executado
    int currentQueueLevel;          // Nível da fila (0, 1 ou 2)
    int totalWaitingTime;           // Contador para aging
    
    static const int MAX_QUEUES = 3;
    static const int QUANTUM_MS = 1;           // 1 milisegundo por quantum
    static const int AGING_THRESHOLD = 5;     // Após 5 unidades, aging ocorre
    
    // Encontra a fila com processo de maior prioridade
    int getHighestPriorityQueue() const;
    
    // Realiza o envelhecimento (aging) de processos esperando
    void performAging();

public:
    UserQueue();
    ~UserQueue();
    
    // Adiciona um novo processo (entra na fila 0 - maior prioridade)
    void enqueue(const ProcessData& process);
    
    // Retorna o próximo processo a ser executado (remove da fila)
    QueuedProcess dequeue();
    
    // Simula a passagem de 1 quantum (1ms)
    // Move o processo atual para a próxima fila se quantum foi usado
    void tick();
    
    // Verifica se todas as filas estão vazias
    bool isEmpty() const;
    
    // Retorna o número total de processos nas filas
    int size() const;
    
    // Retorna o processo atualmente em execução (sem remover)
    QueuedProcess* getCurrentProcess() const;
    
    // Obtém o nível da fila atual
    int getCurrentQueueLevel() const;
    
    // Finaliza o processo atual (chamado quando processo termina sua execução)
    void finishCurrentProcess();
};

#endif