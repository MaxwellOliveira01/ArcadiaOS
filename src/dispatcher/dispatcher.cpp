#include "dispatcher.hpp"
#include <stdexcept>
#include <algorithm>

#include "../process/process.hpp"


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
    process.requiresPrinter = inputData[4];
    process.requiresScanner = inputData[5];
    process.requiresModem = inputData[6];
    process.requiresSata = inputData[7];

    // Mostra os dados do processo criados.
    std::string string_process = toString(process);
    std::cout << string_process << std::endl;
    
    return process;
}

std::string Dispatcher::toString(const ProcessData& process) const {
    std::string output = "dispatcher =>\n"
            "   PID: " + std::to_string(process.pid) + "\n" +
            "   frame: " + std::to_string(process.workingSetSize) + "\n" +
            "   priority: " + std::to_string(process.priority) + "\n" +
            "   time: " + std::to_string(process.cpuTime) + "\n" +
            "   printer: " + std::to_string(process.requiresPrinter ? 1 : 0) + "\n" +
            "   scanner: " + std::to_string(process.requiresScanner ? 1 : 0) + "\n" +
            "   modems: " + std::to_string(process.requiresModem ? 1 : 0) + "\n" +
            "   drivers: " + std::to_string(process.requiresSata ? 1 : 0) + "\n";

    
    for (const auto& op : process.diskOperations) {
        output += "   Disk Operation: (opCode, fileName, numBlocks) = "
                + std::to_string(op.opCode)
                + ", " + op.fileName
                + ", " + std::to_string(op.numBlocks) + "\n";
    }

    output += "   Memory Reference: ";
    for (const auto& ref : process.memoryReferences) {
        output += std::to_string(ref) + ", ";
    }
    // Remove the trailing comma and space
    if (!output.empty()) {
        output.pop_back();
        output.pop_back();
    }
    return output + "\n";
}

std::vector<ProcessData> Dispatcher::initProcess (std::vector<ProcessData>* processes, const int currentClock) {
    std::vector<ProcessData> readyProcesses = std::vector<ProcessData>();
    for (auto& process : *processes) {
        if (process.initTime == currentClock) {
            readyProcesses.push_back(process);

            // Descarta o processo da lista de processos
            processes->erase(std::remove_if(processes->begin(), processes->end(),
                [&process](const ProcessData& p) { return p.pid == process.pid; }),
                processes->end());
        }
    }

    // Return a default-constructed ProcessData if no process is found
    return readyProcesses;
}