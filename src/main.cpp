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

    // Define o tipo do processo
    ProcessManipulator::setType(processes);

    // Escalonar os processos criados.
    Scheduler scheduler;
    scheduler.scaleProcess(processes);
    ProcessData running;

    // if debug
    std::cout << "\n\n\nFila de Processos:\n";
    scheduler.printQueues();

    // Execução dos processos
    while(!scheduler.isEmpty()) {
        running = scheduler.getProcess();

        // debug
        std::cout << "Process " << running.pid << " =>\n";
        std::cout << "  timeUsed = " << running.cpuTime - running.executedTime << "\n";

        int timeUsed = 0;
        while(running.executedTime < running.cpuTime) {
            //1. Simula execução de 1ms de CPU
            timeUsed++;
            running.executedTime++;

            cout << "  Executou=" << timeUsed << " vez.\n";

            
            
            /*Operações realizadas pelo dispatcher*/
            // Executa instruções do processo

            // Executa operaçoes no sistema de arquivos

            if (!running.realTime) break; // Se for processo de usuário, executa apenas 1ms de CPU
        }
        // end debug

        

        if (running.executedTime < running.cpuTime) {
            // Se o processo não terminou, re-adiciona na fila
            ProcessManipulator::aging(&running);

            // debug: Verifica se o processo está sofrendo envelhecimento
            std::cout << "  Aging: PID=" << running.pid
                << " priority=" << running.priority << "\n";

            // end debug.

            scheduler.feedbackProcess(running);
        }

        // debug
        std::cout << "\n\n\nFila de Processos:\n";
        scheduler.printQueues();
        std::cout << "\n\n\n";


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