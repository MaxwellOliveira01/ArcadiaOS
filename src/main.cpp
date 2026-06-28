#include <iostream>
#include <string>
#include <climits>

#include "utils/input_parser.hpp"
#include "io/resource_manager.hpp"

//Dispatcher
#include "dispatcher/dispatcher.hpp"

//Scheduler
#include "scheduler/scheduler.hpp"

// Memory Manager
#include "memory/memory_manager.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    
    if (argc != 4) {
        std::cerr << "Uso " << (argc > 0 ? argv[0] : "./dispatcher")
            << " processes.txt files.txt string.txt\n";
            return 1;
    }

    std::string processesFile = argv[1];
    std::string filesFile = argv[2];
    std::string stringFile = argv[3];

    std::cout << "ArcadiaOS\n"
        << " processes: " << processesFile << "\n"
        << " files: " << filesFile << "\n"
        << " strings: " << stringFile << "\n";


    std::vector<ProcessData> processes = InputParser::parseProcesses(processesFile);
    std::vector<std::vector<int>> pageRefs = InputParser::parseStrings(stringFile);
    auto [fsInit, fileOps] = InputParser::parseFiles(filesFile);


    // Declaração de variáveis
    Dispatcher dispatcher; // Gerenciador de processos
    Scheduler scheduler; // Escalonador de processos
    OperationResult fileSystemResult; // Resultado da operação do sistema de arquivos
    MemoryManager memoryManager; // Gerenciador de memória
    std::vector<ProcessData> readyProcesses; // Processos prontos para execução
    ProcessData* currentProcess = nullptr; // Processo em execução
    std::string output_string;
    int timeUsed, clock = 0, systemOp = 0;
    
    
    std::cout << "Processos (" << processes.size() << "):\n";
    for(const auto &p : processes) {
        // Passa as operações de disco e referências de memória para os processos
        ProcessManipulator::setDiskOperations(const_cast<ProcessData*>(&p), std::move(fileOps));
        ProcessManipulator::setMemoryReferences(const_cast<ProcessData*>(&p), std::move(pageRefs[p.pid]));

        // Print dos processos criados
        output_string = dispatcher.toString(p);
        std::cout << output_string;
    }

    FileSystem fs(fsInit, processes);
    ResourceManager resourceManager;
    std::unordered_map<int, PageTable> pageTables;

    // ----------------- ÁREA DE EXECUÇÃO ----------------- //
    while(true) {
        clock++; // Incrementa o relógio do sistema
        
        // if debug
        std::cout << "\n\n\nClock: " << clock << "\n";
        // end debug
        
        // Aumenta o tempo de espera dos processos nas filas
        scheduler.checkWaitingTime();

        // verifica se tem processo para iniciasr no clock atual
        readyProcesses = dispatcher.initProcess(&processes, clock);
        
        if (!readyProcesses.empty()) {
            scheduler.scaleProcess(readyProcesses);
        }

        if (currentProcess == nullptr) {
            if (!dispatcher.checkPendingProcesses(&processes, clock) && scheduler.isEmpty()) {
                std::cout << "Todos os processos foram executados.\n";
                break;
            }
            
            // Verifica se ainda existe processo no escalonador
            if (scheduler.isEmpty()) {
                continue; // Nenhum processo pronto para execução, incrementa o relógio
            }

            currentProcess = scheduler.getProcess();

            // Tenta alocar os recursos que vai precisar
            if(!currentProcess->realTime && !resourceManager.tryAllocate(*currentProcess)) {
                scheduler.feedbackProcess(*currentProcess);
                currentProcess = nullptr;
                continue;
            }

            // debug
            std::cout << "Process " << currentProcess->pid << " =>\n";

            // Estipulador de Quota
            if (!currentProcess->realTime) {
                timeUsed = 0;
            }

        }


        // -------------------- EXECUÇÃO DO PROCESSO -------------------- //

        // ------ > VERIFICA SE TEM QUE EXERCUTAR INSTRUÇÕES < ------ //
        if (currentProcess->executedTime < currentProcess->cpuTime) {
            currentProcess->executedTime++;

            //debug: Simula a execução de 1 instrução de CPU
            std::cout << "  P" << currentProcess->pid << " instruction " << currentProcess->executedTime << std::endl;

        }
        

        // ------ > VERIFICA SE TEM QUE FAZER OPERAÇÕES NO DISCO < ------ //
        else if ((int)currentProcess->diskOperations.size() > 0) {
            systemOp++;
            // Executa a operação de disco
            FileOperation op = currentProcess->diskOperations.front();
            currentProcess->diskOperations.erase(currentProcess->diskOperations.begin());

            fileSystemResult = fs.execute(op);

            // Print do resultado da operação de disco
            if (fileSystemResult.success) {
                std::cout << "  \033[1mOperação " << systemOp << " => Sucesso\033[0m\n";
                std::cout << "  Processo " << currentProcess->pid << " "
                    << (op.opCode == 0 ? "criou" : "deletou") << " o arquivo: "
                    << op.fileName << (op.opCode == 0 ? "(criou " + std::to_string(op.numBlocks) + " blocos)" : "") << std::endl;
            } else {
                std::cout << "  \033[1mOperação " << systemOp << " => Falha\033[0m\n";
                std::cout << "  Processo " << currentProcess->pid << " :" << fileSystemResult.message << std::endl;
            }
        } 

        
        // ------ > VERIFICA SE TEM QUE FAZER REFERÊNCIAS DE MEMÓRIA < ------ //
        else if ((int)currentProcess->memoryReferences.size() > 0) {
            // Executa a referência de memória
            int ref = currentProcess->memoryReferences.front();
            currentProcess->memoryReferences.erase(currentProcess->memoryReferences.begin());

            auto it = pageTables.find(currentProcess->pid);
            if(it == pageTables.end()) {
                it = pageTables.emplace(
                    currentProcess->pid,
                    PageTable(&memoryManager, currentProcess)
                ).first;
            }

            int frame = it->second.pageHit(ref);
            
            //debug: Simula a execução da referência de memória
            std::cout << "  Process " << currentProcess->pid << " ref page: "
                    << ref << " -> frame " << frame << "\n";

        } 
        
        // -----> OPERAÇÕES I/O DE RECURSOS <----- //
        // Impressora
        else if (currentProcess->requiresPrinter) {
            // Simula a operação de I/O da impressora
            std::cout << "  Process " << currentProcess->pid << " is using the printer." << std::endl;
            currentProcess->requiresPrinter = false; // Marca que a operação foi concluída
        }

        // Scanner
        else if (currentProcess->requiresScanner) {
            // Simula a operação de I/O do scanner
            std::cout << "  Process " << currentProcess->pid << " is using the scanner." << std::endl;
            currentProcess->requiresScanner = false; // Marca que a operação foi concluída
        }

        // Modem
        else if (currentProcess->requiresModem) {
            // Simula a operação de I/O do modem
            std::cout << "  Process " << currentProcess->pid << " is using the modem." << std::endl;
            currentProcess->requiresModem = false; // Marca que a operação foi concluída
        }

        // Drivers 
        else if (currentProcess->requiresSata) {
            // Simula a operação de I/O dos drivers
            std::cout << "  Process " << currentProcess->pid << " is using the drivers." << std::endl;
            currentProcess->requiresSata = false; // Marca que a operação foi concluída
        }

        else {
            // Nenhuma operação restante, o processo terminou
            std::cout << "  Process " << currentProcess->pid << " has completed execution." << std::endl;
            currentProcess = nullptr;

        }

        timeUsed++;

        if(currentProcess != nullptr) {
            if (currentProcess->executedTime >= currentProcess->cpuTime && currentProcess->diskOperations.size() == 0 && currentProcess->memoryReferences.size() == 0) {
                // Processo terminou, libera recursos e remove da fila
                std::cout << "  Process " << currentProcess->pid << " has completed execution." << std::endl;
                if(!currentProcess->realTime) {
                    resourceManager.release(*currentProcess);
                }
                currentProcess = nullptr;
            }
            
            // -------------------- FIM DA EXECUÇÃO DO PROCESSO -------------------- //
            
            if (!currentProcess->realTime && timeUsed >= scheduler.QUANTUM_TIME) {
                // Se o processo de usuário atingiu o tempo de quantum, realimenta-o
                scheduler.feedbackProcess(*currentProcess);
                currentProcess = nullptr;
                
            }
        }
    }


    /* ----- > Operações finais < ----- */

    // Mostra o disco
    std::cout << "\n\033[1mMapa de ocupação do disco:\033[0m\n" << fs.getDiskMap() << " \n";

    // Mostra o numero de falta de páginas
    std::cout << "\n\033[1mNúmero de faltas de páginas por processo:\033[0m\n";
    for(auto &p : processes) {
        cout << "P " << p.pid << " = " << p.pageFaults << " faltas de paginas\n";
    }

/*     
    // if debug

    std::cout << "Disco: " << fsInit.totalBlocks << " blocos, "
        << (int)fsInit.existingFiles.size() << " arquivos existentes\n";
    
    for(auto &f: fsInit.existingFiles) {
        std::cout << " " << f.name << " start=" << f.startBlock << " size=" << f.numBlocks << "\n";
    }

    std::cout << "Operacoes de arquivo (" << (int)fileOps.size() << "):\n";
    for(int i = 0; i < (int)fileOps.size(); i++) {
        auto &op = fileOps[i];
        std::cout << " [" << i << "] pid=" << op.pid
            << " op=" << (op.opCode == 0 ? "create" : "delete")
            << " name=" << op.fileName;
        
        if (op.opCode == 0) {
            std::cout << " blocks=" << op.numBlocks;
        }
        std::cout << "\n";
    }

    std::cout << "Strings de referencia (" << (int)pageRefs.size() << " processos):\n";
    for(int i = 0; i < (int)pageRefs.size(); i++) {
        std::cout << " p" << i << ": " << pageRefs[i].size() << " referencias\n";
    }

    ResourceManager resourceManager;
    std::vector<ProcessData> processesWithResources;
    for(auto &p : processes) {
        bool ok = resourceManager.tryAllocate(p);
        std::cout << "alloc PID=" << p.pid << " =>" << (ok ? "true" : "false") << "\n";
        if(ok) {
            processesWithResources.push_back(p);
        }
    }

    for(auto &p : processesWithResources) {
        resourceManager.release(p);
        std::cout << "release PID=" << p.pid << "\n";
    }

    //end if debug

 */
    return 0;
}