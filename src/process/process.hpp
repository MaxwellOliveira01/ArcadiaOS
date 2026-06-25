#ifndef PROCESS_HPP
#define PROCESS_HPP

struct ProcessData {
    int pid;
    int initTime;
    int priority;
    int cpuTime;
    int workingSetSize;
    bool requiresPrinter;
    bool requiresScanner;
    bool requiresModem;
    bool requiresSata;

    // Variavel de controle para o escalonador
    int queueLevel = 0;
    bool realTime;

    int executedTime = 0; // Tempo de CPU já utilizado
};

class ProcessManipulator {
    public:
        static void aging(ProcessData& process) {
            // Reduz a prioridade do processo para evitar starvation
            if (process.priority > 0) {
                process.priority--;
            }
        }
};

#endif