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

            current = getNext(scheduler, resourceManager, timeUsed);
            if(current == nullptr) {
                continue;
            }

            actPageTable = new PageTable(&memoryManager, current);

            std::cout << "Processo " << (current->pid) << " =>\n";
        }

        executeOneTick(current, *actPageTable);
        timeUsed++;

        if(processFinished(current)) {
            std::cout << " Processo " << (current->pid) << " foi completado com sucesso\n";
            finalizeProcess(current, resourceManager);
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
        
ProcessData* Dispatcher::getNext(Scheduler& scheduler, ResourceManager& resourceManager, int& timeUsed) {
    ProcessData* next = scheduler.getProcess();

    if(!next->realTime && !resourceManager.tryAllocate(*next)) {
        scheduler.feedbackProcess(next);
        return nullptr;
    }

    if(!next->realTime) {
        timeUsed = 0;
    }

    return next;
}


// CPU + Memoria + IO
void Dispatcher::executeOneTick(ProcessData* current, PageTable& actPageTable) {
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
    else {
        tryIO(current);
    }

    // std::cout << " Processo " << (current->pid) << " completou sua execucao\n";
}

void Dispatcher::doMemoryReference(ProcessData* current, PageTable& actPageTable) {
    int ref = current->memoryReferences.front();
    current->memoryReferences.erase(current->memoryReferences.begin());

    int frame = actPageTable.pageHit(ref);
    std::cout << " Processo " << (current->pid) << " ref page: " << ref << " -> frame " << frame << "\n"; 
}

bool Dispatcher::tryIO(ProcessData* p) {
    struct IO {
        bool& flag;
        std::string name;
    };

    IO ios[] = { // Colocar esses nomes em ptbr ou deixar em ing?
        { p->requiresPrinter, "printer"},
        { p->requiresScanner, "scanner"},
        { p->requiresModem, "modem"},
        { p->requiresSata, "sata"},
    };

    for(auto& io : ios) {
        if(io.flag) {
            std::cout << " Process " << (p->pid) << " está usando o/a " << io.name << "\n";
            io.flag = false;
            return true; 
        }
    }

    return false;

}

bool Dispatcher::processFinished(ProcessData* current) {
    return current != nullptr
        && (current->executedTime) >= current->cpuTime
        && current->memoryReferences.empty()
        && !current->requiresPrinter
        && !current->requiresScanner
        && !current->requiresModem
        && !current->requiresSata;
}

bool Dispatcher::quantumExpired(ProcessData* current, int timeUsed, Scheduler& scheduler) {
    return current != nullptr && !current->realTime && timeUsed >= scheduler.QUANTUM_TIME;
}

void Dispatcher::finalizeProcess(ProcessData* current, ResourceManager& resourceManager) {
    if(!current->realTime) {
        resourceManager.release(*current);
    }
}