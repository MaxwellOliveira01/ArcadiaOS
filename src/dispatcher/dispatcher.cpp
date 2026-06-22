#include "dispatcher.hpp"
#include <stdexcept>


Dispatcher::Dispatcher() {}

ProcessData Dispatcher::createProcessFromInput(const std::vector<int>& inputData) {
    if (inputData.size() != 8) {
        throw std::invalid_argument("Parametros invalidos.");
    }

    ProcessData process;
    // Cria o processo com os dados base
    process.pid = nextPid;
    nextPid++;

    process.initTime = inputData[0];

    // Checa se a prioridade do processo não é menor que 0
    if (inputData[1] < 0){
        throw std::invalid_argument("Prioridade deve ser um numero inteiro positivo.");
    }    
    process.priority = inputData[1];

    process.cpuTime = inputData[2];
    process.workingSetSize = inputData[3];
    process.requiresPrinter = (inputData[4] == 1) ? true : false;
    process.requiresScanner = (inputData[5] == 1) ? true : false;
    process.requiresModem = (inputData[6] == 1) ? true : false;
    process.requiresSata = (inputData[7] == 1) ? true : false; 

    // Mostra os dados do processo criados.
    std::string string_process = toString(process);
    std::cout << string_process << std::endl;
    
    return process;
}

std::string Dispatcher::toString(const ProcessData& process = ProcessData()) const {
    return "dispatcher =>\n",
            "   PID: " + std::to_string(process.pid) + "\n" +
            "   frame: " + std::to_string(process.workingSetSize) + "\n" +
            "   priority: " + std::to_string(process.priority) + "\n" +
            "   time: " + std::to_string(process.cpuTime) + "\n" +
            "   printer: " + std::to_string(process.requiresPrinter ? 1 : 0) + "\n" +
            "   scanner: " + std::to_string(process.requiresScanner ? 1 : 0) + "\n" +
            "   modems: " + std::to_string(process.requiresModem ? 1 : 0) + "\n" +
            "   drivers: " + std::to_string(process.requiresSata ? 1 : 0) + "\n";
}