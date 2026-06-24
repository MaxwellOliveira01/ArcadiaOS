#include "disk.hpp"

namespace {
    string EMPTY = "0";
};

Disk::Disk(int totalBlocks) {
    blocks = vector<string>(totalBlocks, EMPTY);
}

int Disk::allocate(string name, int numBlocks) {
    return 0; // TODO
}

void Disk::freeBlocks(int startBlock, int numBlocks) {
    for(int i = 0; i < numBlocks && i + numBlocks < (int)blocks.size(); i++) {
        blocks[i + startBlock] = EMPTY;
    }
}

bool Disk::placeExisting(string name, int startBlock, int numBlocks) {
    return false; // TODO
}

void Disk::printMap() {
    // TODO
}