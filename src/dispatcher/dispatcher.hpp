
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
#include <iostream>
#include <vector>
#include <string>

class Dispatcher {
    private:
        int nextPid;

        std::string toString(const ProcessData& process = ProcessData()) const;

    public:
        Dispatcher() : nextPid(1) {};

        ProcessData createProcessFromInput(const std::vector<int>& inputData);

};

#endif