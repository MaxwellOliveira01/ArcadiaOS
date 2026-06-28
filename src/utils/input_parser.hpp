#ifndef INPUT_PARSER_HPP
#define INPUT_PARSER_HPP

#include <vector>
#include <utility>
#include <string>

#include "../process/process.hpp"
#include "../filesystem/file_system.hpp"

namespace InputParser {
    
    std::vector<ProcessData> parseProcesses(const std::string& filename);
    
    std::pair<FileSystemInit, std::vector<FileOperation>> parseFiles(const std::string& filename);
    
    std::vector<std::vector<int>> parseStrings(const std::string& filename);
}

#endif