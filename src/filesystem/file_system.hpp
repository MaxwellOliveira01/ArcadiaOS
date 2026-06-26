#ifndef FILE_SYSTEM_HPP
#define FILE_SYSTEM_HPP

#include <vector>
#include <utility>
#include <string>

#include <map>
#include "disk.hpp"

struct ProcessData; // declaracao pra fazer compilar, o codigo em si ta no outro arquivo

struct FileEntry {
    std::string name;
    int startBlock;
    int numBlocks;
};

struct FileOperation {
    int pid;
    int opCode; // 0 to create, 1 to delete
    std::string fileName;
    int numBlocks;
};

struct FileSystemInit {
    int totalBlocks;
    std::vector<FileEntry> existingFiles;
};

struct OperationResult {
    bool success;
    std::string message;
};

class FileSystem {
public:

    FileSystem(FileSystemInit& init, std::vector<ProcessData>& processes);

    OperationResult execute(FileOperation &op);

private:

    OperationResult handleCreate(FileOperation &op);

    OperationResult handleDelete(FileOperation& op);

    Disk diskUnit;

    std::map<std::string, FileEntry> filesByName;

    std::map<std::string, int> fileOwner;

    std::vector<ProcessData> processes;

};


#endif