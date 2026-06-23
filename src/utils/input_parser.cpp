#include "input_parser.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

namespace {
    
    string trimEnd(string s) {
        while((int)s.size() && isspace(s.back())) {
            s.pop_back();
        }
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
            throw runtime_error("Error when opening file " + filename);
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
                throw runtime_error("Invalid line " + line + ", expected exactly 8 fields");
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
            p.requiresScanner = stoi(fields[7]) != 0;

            processes.push_back(p);
        }

        return processes;
    }
    
    pair<FileSystemInit, vector<FileOperation>> parseFiles(const string& filename) {
        // TODO
    }
    
    vector<vector<int>> parseStrings(const string& filename) {
        // TODO
    }

}