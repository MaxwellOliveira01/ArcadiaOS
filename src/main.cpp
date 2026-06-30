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

struct SimulationInputs {
    vector<ProcessData> processes;
    vector<vector<int>> pageRefs;
    FileSystemInit fsInit;
    vector<FileOperation> fileOps;
};

bool validateArgs(int argc, char* argv[]);
SimulationInputs loadInputs(string p, string f, string s);
void printFinalReport(FileSystem& fs, std::vector<ProcessData>& processes);
void runFileOperations(FileSystem& fs, std::vector<FileOperation>& fileOps);

int main(int argc, char* argv[]) {
    if(!validateArgs(argc, argv)) {
        return 1;
    }

    auto [processes, pageRefs, fsInit, fileOps] 
        = loadInputs(argv[1], argv[2], argv[3]);

    Dispatcher dispatcher;
    Scheduler scheduler;
    MemoryManager memoryManager;
    ResourceManager resourceManager;
    FileSystem fs(fsInit, processes);

    unordered_map<int, PageTable> pageTables;

    dispatcher.printProcesses(processes, pageRefs);
    
    dispatcher.start(processes, scheduler, 
        memoryManager, resourceManager, pageTables);

    runFileOperations(fs, fileOps);
    printFinalReport(fs, processes);

}

bool validateArgs(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Uso " << (argc > 0 ? argv[0] : "./dispatcher")
            << " processes.txt files.txt string.txt\n";
            return false;
    }

    cout << "ArcadiaOS\n"
        <<" processos: " << argv[1] << "\n"
        <<" arquivos: " << argv[2] << "\n"
        <<" strings: " << argv[3] << "\n";
    
    return true;
}

SimulationInputs loadInputs(string p, string f, string s) {
    SimulationInputs in;
    in.processes = InputParser::parseProcesses(p);
    in.pageRefs = InputParser::parseStrings(s);
    tie(in.fsInit, in.fileOps) = InputParser::parseFiles(f);
    return in;
}

void printFinalReport(FileSystem& fs, std::vector<ProcessData>& processes) {
    cout << "\nMapa de ocupacao do disco\n" << fs.getDiskMap() << "\n";

    cout << "Numero de faltas de paginas por processo:\n";
    for(auto &p : processes) {
        cout << "P" << (p.pid) << " = " << p.pageFaults << " faltas de paginas\n";
    }
}

void runFileOperations(FileSystem& fs, std::vector<FileOperation>& fileOps) {
    cout << "\nResultado das operacoes de disco:\n";
    
    int systemOp = 0;

    for(auto& op : fileOps) {
        systemOp++;

        auto result = fs.execute(op);

        if(result.success) {
            auto opName = (op.opCode == 0) ? "criou" : "deletou";

            cout << " Operacao " << systemOp << " => Sucesso\n"
                << " Processo " << op.pid << " "
                << opName << " o arquivo: " << op.fileName
                << (op.opCode == 0 ? " (" + to_string(op.numBlocks) + " blocos)" : "")
                << "\n";

        } else {
            cout << " Operacao " << systemOp << " => Falha\n" << "  " << result.message << "\n";
        }

    } 

}