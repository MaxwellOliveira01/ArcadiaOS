#ifndef FILE_SYSTEM_HPP
#define FILE_SYSTEM_HPP

#include <vector>
#include <utility>
#include <string>

using namespace std;

struct FileEntry {
    string name;
    int startBlock;
    int numBlocks;
};

struct FileOperation {
    int pid;
    int opCode; // 0 to create, 1 to delete
    string fileName;
    int numBlocks;
};

struct FileSystemInit {
    int totalBlocks;
    vector<FileEntry> existingFiles;
};

#endif