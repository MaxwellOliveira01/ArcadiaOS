
/*
O QUE FALTA IMPLEMENTAR:
- Implementação de escalonamento do primeiro processo das filas.
- Contador de uso da CPU.
- Implementação de realimentação (aging) para evitar starvation.
- Implementação de E/S
- Impressão na tela das operações do sistema de arquivos de cada processo.

*/

#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include "../process/process.hpp"
#include "utils/input_parser.hpp"

#include <iostream>
#include <vector>
#include <string>

class Dispatcher {
    private:
        int nextPid = 0;

    public:
        Dispatcher();

        std::string toString(const ProcessData& process) const;

        std::vector<ProcessData> initProcess (std::vector<ProcessData>* processes, const int currentClock);
};

#endif