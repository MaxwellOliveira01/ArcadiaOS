#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <vector>

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
        static void aging(ProcessData* process) {
            // Reduz a prioridade do processo para evitar starvation
            if (process->priority > 0) {
                process->priority--;
            }
        }

        static void setType(ProcessData* process) {
            if (process->priority == 0) {
                process->realTime = true;
            } else {
                process->realTime = false;
            }
        }

        static void setType(std::vector<ProcessData>& processes) {
            for (auto& process : processes) {
                setType(&process);
            }
        }
};

#endif