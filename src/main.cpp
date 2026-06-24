#include <iostream>
#include <string>

#include "utils/input_parser.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    
    if (argc != 4) {
        cerr << "Uso " << (argc > 0 ? argv[0] : "./dispatcher")
            << " processes.txt files.txt string.txt\n";
            return 1;
    }

    string processesFile = argv[1];
    string filesFile = argv[2];
    string stringFile = argv[3];

    cout << "ArcadiaOS\n"
        << " processes: " << processesFile << "\n"
        << " files: " << filesFile << "\n"
        << " strings: " << stringFile << "\n";


    vector<ProcessData> processes = InputParser::parseProcesses(processesFile);
    vector<vector<int>> pageRefs = InputParser::parseStrings(stringFile);
    auto [fsInit, fileOps] = InputParser::parseFiles(filesFile);

    // if debug

    cout << "Processos (" << processes.size() << "):\n";
    for(auto &p : processes) {
        cout << " PID=" << p.pid
            << " init=" << p.initTime
            << " priority=" << p.priority
            << " cpu=" << p.cpuTime
            << " ws=" << p.workingSetSize
            << " io[p,s,m,sata]=[" << p.requiresPrinter
            << "," << p.requiresScanner
            << "," << p.requiresModem
            << "," << p.requiresSata
            << "]\n";
    }

    cout << "Disco: " << fsInit.totalBlocks << " blocos, "
        << (int)fsInit.existingFiles.size() << " arquivos existentes\n";
    
    for(auto &f: fsInit.existingFiles) {
        cout << " " << f.name << " start=" << f.startBlock << " size=" << f.numBlocks << "\n";
    }

    cout << "Operacoes de arquivo (" << (int)fileOps.size() << "):\n";
    for(int i = 0; i < (int)fileOps.size(); i++) {
        auto &op = fileOps[i];
        cout << " [" << i << "] pid=" << op.pid
            << " op=" << (op.opCode == 0 ? "create" : "delete")
            << " name=" << op.fileName;
        
        if (op.opCode == 0) {
            cout << " blocks=" << op.numBlocks;
        }
        cout << "\n";
    }

    cout << "Strings de referencia (" << (int)pageRefs.size() << " processos):\n";
    for(int i = 0; i < (int)pageRefs.size(); i++) {
        cout << " p" << i << ": " << pageRefs[i].size() << " referencias\n";
    }

    //end if debug


    return 0;
}