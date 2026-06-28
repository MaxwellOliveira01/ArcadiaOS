#include "dispatcher.hpp"
#include <stdexcept>
#include <algorithm>

#include "../process/process.hpp"

Dispatcher::Dispatcher() {}

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
    if((int)process.memoryReferences.size() == 0) {
        for (const auto& ref : process.memoryReferences) {
           output += std::to_string(ref) + ", ";
        }
        // Remove the trailing comma and space
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
        }
    }

    // Return a default-constructed ProcessData if no process is found
    return readyProcesses;
}

bool Dispatcher::checkPendingProcesses(std::vector<ProcessData>* processes, const int currentClock){
    // Verifica se ainda existe processo pendente.
    for (const auto& process : *processes) {
        if (process.initTime >= currentClock) {
            return true; // There are pending processes
        }
    }
    return false; // No pending processes
}