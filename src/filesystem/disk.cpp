#include "disk.hpp"

namespace {
    string EMPTY = "0";
};

Disk::Disk(int totalBlocks) {
    blocks = vector<string>(totalBlocks, EMPTY);
}

int Disk::tryAllocate(string name, int numBlocks) {
    int start = -1;

    for(int i = 0; i + numBlocks < (int)blocks.size(); i++) {
        if(isRangeAvailable(i, numBlocks)) {
            start = i;
            break;
        }
    }

    if(start == -1) {
        return -1;
    }

    allocate(name, start, numBlocks);
    return start;
}

void Disk::freeBlocks(int startBlock, int numBlocks) {
    for(int i = 0; i < numBlocks && i + numBlocks < (int)blocks.size(); i++) {
        blocks[i + startBlock] = EMPTY;
    }
}

bool Disk::placeExisting(string name, int startBlock, int numBlocks) {
    if(!isRangeAvailable(startBlock, numBlocks)) {
        return false;
    }
    allocate(name, startBlock, numBlocks);
    return true;
}

string Disk::getMemoryMap() {
    string result = "";
    for(int i = 0; i < (int)blocks.size(); i++) {
        result += blocks[i];
        if(i + 1 < (int)blocks.size()) result += " ";
    }
    return result;
}

bool Disk::isRangeAvailable(int start, int numBlocks) {
    if(start + numBlocks >= (int)blocks.size()) {
        return false;
    }

    for(int i = start; i < start + numBlocks; i++) {
        if(blocks[i] != EMPTY) {
            return false;
        }
    }
    
    return true;
}

void Disk::allocate(string name, int start, int numBlocks) {
    for(int i = start; i < start + numBlocks; i++) {
        blocks[i] = name;
    }
}