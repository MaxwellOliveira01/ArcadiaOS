#include <iostream>
#include <string>

#include "utils/input_parser.hpp"
#include "io/resource_manager.hpp"

//Dispatcher
#include "dispatcher/dispatcher.hpp"

//Scheduler
#include "scheduler/scheduler.hpp"

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


    // Dispatcher: Cria processos
    Dispatcher dispatcher;
    std::string output_string;
    
    // Print dos processos criados
    std::cout << "Processos (" << processes.size() << "):\n";
    for(const auto &p : processes) {
        output_string = dispatcher.toString(p);
        std::cout << output_string;
    }

    // Escalonar os processos criados.
    Scheduler scheduler;
    for (auto &p : processes) {
        scheduler.initAddProcess(p);
    }

    // Execução dos processos
    while(!scheduler.isEmpty()) {
        auto [process, queueType] = scheduler.getProcess();
        if (queueType == -1) {
            std::cout << "Nenhum processo disponivel para execucao\n";
            break;
        }

        // debug
        std::cout << "Process " << process.pid << " =>\n";
        std::cout << "  queueType = " << queueType << "\n"
            << "  cpuTime = " << process.cpuTime << "\n";

        int timeUsed = 0;
        while((timeUsed < scheduler.QUANTUM_TIME || queueType == 0) && process.executedTime < process.cpuTime) {
            //1. Simula execução de 1ms de CPU
            timeUsed++;
            process.executedTime++;

            cout << "  Executou=" << timeUsed << " vez.\n";

            
            
            /*Operações realizadas pelo dispatcher*/
            // Executa instruções do processo

            // Executa operaçoes no sistema de arquivos
        }
        // end debug

        if (process.executedTime < process.cpuTime) {
            // Se o processo não terminou, re-adiciona na fila
            scheduler.reAddProcess(process);
        }


    }

    // Mostra o Mapa de ocupação do disco

    // Mostra o Número de falta de páginas

    // Encerra Execução

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


    return 0;
}