#ifndef INPUT_PARSER_HPP
#define INPUT_PARSER_HPP

#include <vector>
#include <utility>
#include <string>

#include "../process/process.hpp"
#include "../filesystem/file_system.hpp"

namespace InputParser {
    
    vector<ProcessData> parseProcesses(const string& filename);
    
    pair<FileSystemInit, vector<FileOperation>> parseFiles(const string& filename);
    
    vector<vector<int>> parseStrings(const string& filename);
}

#endif