#include "input_parser.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std; // remover?

namespace {
    
    string trimEnd(string s) {
        while((int)s.size() && isspace(s.back())) {
            s.pop_back();
        }
        return s;
    }

    string trim(string s) {
        s = trimEnd(s);
        reverse(s.begin(), s.end());

        s = trimEnd(s);
        reverse(s.begin(), s.end());

        return s;
    }

    vector<string> splitCsv(const string& line) {
        vector<string> out;
        stringstream ss(line);
        string token;
        while(getline(ss, token, ',')) {
            out.push_back(trim(token));
        }
        return out;
    }

    ifstream openOrThrow(string filename) {
        ifstream in(filename);
        if(!in.is_open()) {
            throw runtime_error("Erro ao abrir arquivo " + filename);
        }
        return in;
    }

    bool isBlank(string& line) {
        for(char c : line) {
            if(!isspace(c)) {
                return false;
            }
        }
        return true;
    }

}

namespace InputParser {

    vector<ProcessData> parseProcesses(const string& filename) {
        ifstream in = openOrThrow(filename);
        vector<ProcessData> processes;
        string line;

        int pid = 0;

        while (getline(in, line)) {
            if(isBlank(line)) {
                continue;
            }

            vector<string> fields = splitCsv(line);

            if((int)fields.size() != 8) {
                throw runtime_error("Linha invalida `" + line + "`, esperado 8 campos");
            }
            
            ProcessData p;
            p.pid = pid++;
            p.initTime = stoi(fields[0]);
            p.priority = stoi(fields[1]);
            p.cpuTime = stoi(fields[2]);
            p.workingSetSize = stoi(fields[3]);
            p.requiresPrinter = stoi(fields[4]) != 0;
            p.requiresScanner = stoi(fields[5]) != 0;
            p.requiresModem = stoi(fields[6]) != 0;
            p.requiresSata = stoi(fields[7]) != 0;

            processes.push_back(p);
        }

        return processes;
    }
    
    pair<FileSystemInit, vector<FileOperation>> parseFiles(const string& filename) {
        ifstream in = openOrThrow(filename);
        FileSystemInit init;

        vector<FileOperation> ops;
        string line;

        auto nextLine = [&]() -> bool {
            while(getline(in, line)) {
                if (!isBlank(line)) {
                    return true;
                }
            }
            return false;
        };

        if(!nextLine()) {
            throw runtime_error(filename + ": total de blocos faltando");
        }
        init.totalBlocks = stoi(trim(line));

        if(!nextLine()) {
            throw runtime_error(filename + ": numero de arquivos existentes faltando");
        }
        int count = stoi(trim(line));

        for(int i = 0; i < count; i++) {
            if(!nextLine()) {
                throw runtime_error(filename + ": aquivo existente faltando");
            }

            vector<string> fields = splitCsv(line);

            if((int)fields.size() != 3) {
                throw runtime_error(filename + ": aquivo existente invalido: " + line);
            }

            FileEntry entry;
            entry.name = fields[0];
            entry.startBlock = stoi(fields[1]);
            entry.numBlocks = stoi(fields[2]);
            init.existingFiles.push_back(entry);
        }

        while(nextLine()) {
            vector<string> fields = splitCsv(line);
            if((int)fields.size() != 3 && (int)fields.size() != 4) {
                throw runtime_error(filename + ": operacao invalida : " + line);
            }
            FileOperation op;
            op.pid = stoi(fields[0]);
            op.opCode = stoi(fields[1]);
            op.fileName = fields[2];
            op.numBlocks = (int)fields.size() == 4 ? stoi(fields[3]) : 0;
            ops.push_back(op);
        }

        return {init, ops};

    }
    
    vector<vector<int>> parseStrings(const string& filename) {
        ifstream in = openOrThrow(filename);
        vector<vector<int>> all;
        string line;

        while(getline(in, line)) {
            if(isBlank(line)) {
                continue;
            }

            vector<string> fields = splitCsv(line);
            vector<int> pages;
            
            for(auto& f : fields) {
                pages.push_back(stoi(f));
            }

            all.push_back(pages);
        }

        return all;

    }

}