#include "input_parser.hpp"
#include "../io/resource_manager.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <cctype>

namespace {
    
    std::string trimEnd(std::string s) {
        while((int)s.size() && isspace(s.back())) {
            s.pop_back();
        }
        return s;
    }

    std::string trim(std::string s) {
        s = trimEnd(s);
        reverse(s.begin(), s.end());

        s = trimEnd(s);
        reverse(s.begin(), s.end());

        return s;
    }

    std::vector<std::string> splitCsv(const std::string& line) {
        std::vector<std::string> out;
        std::stringstream ss(line);
        std::string token;
        while(getline(ss, token, ',')) {
            out.push_back(trim(token));
        }
        return out;
    }

    std::ifstream openOrThrow(std::string filename) {
        std::ifstream in(filename);
        if(!in.is_open()) {
            throw std::runtime_error("Erro ao abrir arquivo " + filename);
        }
        return in;
    }

    bool isBlank(std::string& line) {
        for(char c : line) {
            if(!isspace(c)) {
                return false;
            }
        }
        return true;
    }

}

namespace InputParser {

    std::vector<ProcessData> parseProcesses(const std::string& filename) {
        std::ifstream in = openOrThrow(filename);
        std::vector<ProcessData> processes;
        std::string line;

        int pid = 0;

        while (getline(in, line)) {
            if(isBlank(line)) {
                continue;
            }

            std::vector<std::string> fields = splitCsv(line);

            if((int)fields.size() != 8) {
                throw std::runtime_error("Linha invalida `" + line + "`, esperado 8 campos");
            }
            
            ProcessData p;
            p.pid = pid++;
            p.initTime = stoi(fields[0]);
            p.priority = stoi(fields[1]);
            p.cpuTime = stoi(fields[2]);
            p.workingSetSize = stoi(fields[3]);
            p.requiresPrinter = stoi(fields[4]);
            p.requiresScanner = stoi(fields[5]);
            p.requiresModem = stoi(fields[6]);
            p.requiresSata = stoi(fields[7]);

            if(p.requiresPrinter > ResourceManager::TOTAL_PRINTERS) {
                throw std::runtime_error("Processo " + std::to_string(p.pid) + " requisita mais impressoras do que" +
                    "existem (max " + std::to_string(ResourceManager::TOTAL_PRINTERS) + ")");
            }
            
            if(p.requiresScanner > ResourceManager::TOTAL_SCANNERS) { 
                throw std::runtime_error("Processo " + std::to_string(p.pid) + " requisita mais scanners do que " +
                    "existem (max " + std::to_string(ResourceManager::TOTAL_SCANNERS) + ")");
            }
            
            if(p.requiresModem > ResourceManager::TOTAL_MODEMS) { 
                throw std::runtime_error("Processo " + std::to_string(p.pid) + " requisita mais modems do que " + 
                    "existem (max " + std::to_string(ResourceManager::TOTAL_MODEMS) + ")");
            }
            
            if(p.requiresSata > ResourceManager::TOTAL_SATA) { 
                throw std::runtime_error("Processo " + std::to_string(p.pid) + " requisita mais drivers SATA do que " + 
                    "existem (max " + std::to_string(ResourceManager::TOTAL_SATA) + ")");
            }

            p.realTime = (p.priority == 0);

            if(p.realTime && p.workingSetSize > 8) p.workingSetSize = 8;
            if(!p.realTime && p.workingSetSize > 12) p.workingSetSize = 12;

            processes.push_back(p);
        }

        return processes;
    }
    
    std::pair<FileSystemInit, std::vector<FileOperation>> parseFiles(const std::string& filename) {
        std::ifstream in = openOrThrow(filename);
        FileSystemInit init;

        std::vector<FileOperation> ops;
        std::string line;

        auto nextLine = [&]() -> bool {
            while(getline(in, line)) {
                if (!isBlank(line)) {
                    return true;
                }
            }
            return false;
        };

        if(!nextLine()) {
            throw std::runtime_error(filename + ": total de blocos faltando");
        }
        init.totalBlocks = stoi(trim(line));

        if(!nextLine()) {
            throw std::runtime_error(filename + ": numero de arquivos existentes faltando");
        }
        int count = stoi(trim(line));

        for(int i = 0; i < count; i++) {
            if(!nextLine()) {
                throw std::runtime_error(filename + ": aquivo existente faltando");
            }

            std::vector<std::string> fields = splitCsv(line);

            if((int)fields.size() != 3) {
                throw std::runtime_error(filename + ": aquivo existente invalido: " + line);
            }

            FileEntry entry;
            entry.name = fields[0];
            entry.startBlock = stoi(fields[1]);
            entry.numBlocks = stoi(fields[2]);
            init.existingFiles.push_back(entry);
        }

        while(nextLine()) {
            std::vector<std::string> fields = splitCsv(line);
            if((int)fields.size() != 3 && (int)fields.size() != 4) {
                throw std::runtime_error(filename + ": operacao invalida : " + line);
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
    
    std::vector<std::vector<int>> parseStrings(const std::string& filename) {
        std::ifstream in = openOrThrow(filename);
        std::vector<std::vector<int>> all;
        std::string line;

        while(getline(in, line)) {
            if(isBlank(line)) {
                continue;
            }

            std::vector<std::string> fields = splitCsv(line);
            std::vector<int> pages;
            
            for(auto& f : fields) {
                pages.push_back(stoi(f));
            }

            all.push_back(pages);
        }

        return all;

    }

}