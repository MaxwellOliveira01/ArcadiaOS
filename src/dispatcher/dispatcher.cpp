#include "dispatcher.hpp"
#include <stdexcept>
#include <algorithm>

Dispatcher::Dispatcher() {}

void Dispatcher::start(
    std::vector<ProcessData>& processes,
    Scheduler& scheduler,
    MemoryManager& memoryManager,
    ResourceManager& resourceManager
) {

    int clock = 0;
    int timeUsed = 0;
    ProcessData* current = nullptr;
    PageTable* actPageTable = nullptr;

    while(true) {
        clock++;
        std::cout << "\n\n\nClock: " << clock << "\n";

        scheduler.checkWaitingTime();
        admitNewProcesses(processes, scheduler, clock);

        if(current == nullptr) {
            if(allDone(processes, scheduler, clock)) {
                std::cout << "Todos os processos foram executados.\n";
                break;
            }

            if(scheduler.isEmpty()) {
                continue;
            }

            current = getNext(scheduler, timeUsed);
            if(current == nullptr) {
                continue;
            }

            actPageTable = new PageTable(&memoryManager, current);
            
            // Se o processo estiver na lista, quer dizer que ele ainda está usando IO, então libera os recursos que ele estava usando para usar novos.
            if (ListIOUsed.find(current->pid) != ListIOUsed.end()) {
                freeResources(current, resourceManager);
            } 

            std::cout << "Processo " << (current->pid) << " =>\n";
        }

        executeOneTick(current, *actPageTable, resourceManager);
        timeUsed++;

        if(processFinished(current)) {
            std::cout << " Processo " << (current->pid) << " foi completado com sucesso\n";
            freeResources(current, resourceManager);
            current = nullptr;
            delete actPageTable;
            actPageTable = nullptr;
        } else if(quantumExpired(current, timeUsed, scheduler)) {
            scheduler.feedbackProcess(current);
            current = nullptr;
            delete actPageTable;
            actPageTable = nullptr;
        }
    }
}

void Dispatcher::printProcesses(std::vector<ProcessData>& processes, std::vector<std::vector<int>>& pageRefs) {
    std::cout << "Processo (" << processes.size() << "):\n";
    for(auto &p : processes) {
        ProcessManipulator::setMemoryReferences(&p, pageRefs[p.pid]);
        std::cout << toString(p);
    }
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

    
    // for (const auto& op : process.diskOperations) {
    //     output += "   Disk Operation: (opCode, fileName, numBlocks) = "
    //             + std::to_string(op.opCode)
    //             + ", " + op.fileName
    //             + ", " + std::to_string(op.numBlocks) + "\n";
    // }

    output += "   Memory Reference: ";
    if((int)process.memoryReferences.size() != 0) {
        for (const auto& ref : process.memoryReferences) {
           output += std::to_string(ref) + ", ";
        }
        // Remove the trailing comma and space
        output.pop_back();
        output.pop_back();
    }

    return output + "\n";
}

std::vector<ProcessData*> Dispatcher::initProcess (std::vector<ProcessData>* processes, const int currentClock) {
    std::vector<ProcessData*> readyProcesses;
    for (auto& process : *processes) {
        if (process.initTime == currentClock) {
            readyProcesses.push_back(&process);
        }
    }

    return readyProcesses;
}

bool Dispatcher::checkPendingProcesses(std::vector<ProcessData>* processes, const int currentClock){
    // Verifica se ainda existe processo pendente.
    for (const auto& process : *processes) {
        if (process.initTime > currentClock) {
            return true; // There are pending processes
        }
    }
    return false; // No pending processes
}

// privado


void Dispatcher::admitNewProcesses(std::vector<ProcessData>& processes, Scheduler& scheduler, int clock) {
    std::vector<ProcessData*> ready = initProcess(&processes, clock);
    if(!ready.empty()) {
        scheduler.scaleProcess(ready);
    }
}

bool Dispatcher::allDone(std::vector<ProcessData>& processes, Scheduler& scheduler, int clock) {
    return !checkPendingProcesses(&processes, clock) && scheduler.isEmpty();
}
        
ProcessData* Dispatcher::getNext(Scheduler& scheduler, int& timeUsed) {
    ProcessData* next = scheduler.getProcess();

    if(!next->realTime) {
        timeUsed = 0;
    }

    return next;
}


// CPU + Memoria + IO
void Dispatcher::executeOneTick(ProcessData* current, PageTable& actPageTable, ResourceManager& resourceManager) {
    // 1 - CPU
    if((current->executedTime) < (current->cpuTime)) {
        current->executedTime++;
        std::cout << " P" << (current->pid) << " instrucao " << (current->executedTime) << "\n";
    }

    //2 - Memoria
    else if(!current->memoryReferences.empty()) {
        doMemoryReference(current, actPageTable);
    }

    // 3 - IO
    else if(!current->realTime) {
        tryIO(current, resourceManager);
    }

    // std::cout << " Processo " << (current->pid) << " completou sua execucao\n";
}

void Dispatcher::doMemoryReference(ProcessData* current, PageTable& actPageTable) {
    int ref = current->memoryReferences.front();
    current->memoryReferences.erase(current->memoryReferences.begin());

    int frame = actPageTable.pageHit(ref);
    std::cout << " Processo " << (current->pid) << " ref page: " << ref << " -> frame " << frame << "\n"; 
}

void Dispatcher::tryIO(ProcessData* p, ResourceManager& resourceManager) {
    ListIOUsed[p->pid] = resourceManager.tryAllocate(*p);

    IOAllocation ioUsed = ListIOUsed[p->pid];

    if (ioUsed.printersUsed == 0 && ioUsed.scannersUsed == 0 && ioUsed.modemsUsed == 0 && ioUsed.sataUsed == 0) {
        std::cout << " Processo " << (p->pid) << " requisitou IO, mas não conseguiu alocar nenhum recurso\n";
        return;
    }
    std::cout << " Processo " << (p->pid) << " requisitou IO:\n"
        << "   " << ioUsed.printersUsed << " impressoras\n"
        << "   " << ioUsed.scannersUsed << " scanners\n"
        << "   " << ioUsed.modemsUsed << " modems\n"
        << "   " << ioUsed.sataUsed << " drivers SATA\n\n";

}

bool Dispatcher::processFinished(ProcessData* current) {
    if (current != nullptr
        && (current->executedTime) >= current->cpuTime
        && current->memoryReferences.empty()){
            if(current->realTime) {
                return true;
            } else if (current->requiresPrinter <= 0
                && current->requiresScanner <= 0
                && current->requiresModem <= 0
                && current->requiresSata <= 0) {
                return true;
            }
        }
    return false;
}

bool Dispatcher::quantumExpired(ProcessData* current, int timeUsed, Scheduler& scheduler) {
    return current != nullptr && !current->realTime && timeUsed >= scheduler.QUANTUM_TIME;
}

void Dispatcher::freeResources(ProcessData* current, ResourceManager& resourceManager) {
    if(!current->realTime) {
        resourceManager.release(ListIOUsed[current->pid]);
        clearListIOUsed(current->pid);
    }
}

void Dispatcher::clearListIOUsed(int pid) {
    ListIOUsed.erase(pid);
}