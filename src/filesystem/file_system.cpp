#include "file_system.hpp"
#include "../process/process.hpp"

FileSystem::FileSystem(FileSystemInit& init, std::vector<ProcessData>& processes): 
    diskUnit(init.totalBlocks), processes(processes) {

    for(auto &f : init.existingFiles) {
        if(diskUnit.placeExisting(f.name, f.startBlock, f.numBlocks)) {
            filesByName[f.name] = f;
        } else {
            // nao consegui colocar um cara do inicio, erro? -> input invalido
        }
    }
};

OperationResult FileSystem::execute(FileOperation &op) {
    if(op.pid < 0 || op.pid >= (int)processes.size()) {
        return OperationResult{false, "PID " + std::to_string(op.pid) + " inexistente"};
    }

    if(op.opCode == 0) {
        return handleCreate(op);
    }

    if(op.opCode == 1) {
        return handleDelete(op);
    }

    return {false, "opcode invalido"};
}


OperationResult FileSystem::handleCreate(FileOperation& op) {

    if(filesByName.count(op.fileName) > 0) {
        return {false, "arquivo " + op.fileName + " ja existe"};
    }

    int start = diskUnit.tryAllocate(op.fileName, op.numBlocks);
    if(start < 0) {
        return {false, "espaço insuficiente para alocar o arquivo " + op.fileName};
    }

    FileEntry entry;
    entry.name = op.fileName;
    entry.startBlock = start;
    entry.numBlocks = op.numBlocks;
    
    filesByName[op.fileName] = entry;
    fileOwner[op.fileName] = op.pid;

    return {true, ""};

}

OperationResult FileSystem::handleDelete(FileOperation& op) {
    auto it = filesByName.find(op.fileName);

    if(it == filesByName.end()) {
        return {false, "arquivo " + op.fileName + " nao encontrado"};
    }

    auto processOwner = processes[op.pid];

    // pra alterar tem que ter permissao:
    //  - se ele for realmente ele pode
    //  - se ele nao for realtime, só pode se o arquivo for dele
    if(!processOwner.realTime) {
        auto ownerIt = fileOwner.find(op.fileName);
        if(ownerIt == fileOwner.end() || fileOwner[op.fileName] != op.pid) {
            return { false, "PID " + std::to_string(op.pid) + " nao tem permissao para deletar o arquivo: " + op.fileName};
        }
    }

    diskUnit.freeBlocks(it->second.startBlock, it->second.numBlocks);
    filesByName.erase(it);
    fileOwner.erase(op.fileName);

    return {true, ""};
}
