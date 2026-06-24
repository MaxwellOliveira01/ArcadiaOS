#include "resource_manager.hpp"

ResourceManager::ResourceManager() {
    printers = TOTAL_PRINTERS;
    scanners = TOTAL_SCANNERS;
    modems = TOTAL_MODEMS;
    sata = TOTAL_SATA;
};

bool ResourceManager::tryAllocate(ProcessData& process) {
    
    if(process.requiresPrinter && printers == 0) return false;
    if(process.requiresModem && modems == 0) return false;
    if(process.requiresSata && sata == 0) return false;
    if(process.requiresScanner && scanners == 0) return false;

    printers -= process.requiresPrinter;
    modems -= process.requiresModem;
    sata -= process.requiresSata;
    scanners -= process.requiresScanner;
    
    return true;
}

void ResourceManager::release(ProcessData& process) {
    if(process.requiresPrinter) printers++; 
    if(process.requiresModem) modems++; 
    if(process.requiresSata) sata++; 
    if(process.requiresScanner) scanners++; 
}