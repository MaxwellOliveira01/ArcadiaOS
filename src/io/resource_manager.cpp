#include "resource_manager.hpp"

ResourceManager::ResourceManager() {
    printers = TOTAL_PRINTERS;
    scanners = TOTAL_SCANNERS;
    modems = TOTAL_MODEMS;
    sata = TOTAL_SATA;
};

IOAllocation ResourceManager::tryAllocate(ProcessData& process) {
    IOAllocation iosAlloc = {
        printers - process.requiresPrinter < 0 ? printers : process.requiresPrinter,
        scanners - process.requiresScanner < 0 ? scanners : process.requiresScanner,
        modems - process.requiresModem < 0 ? modems : process.requiresModem,
        sata - process.requiresSata < 0 ? sata : process.requiresSata
    };

    // Primeiro diminui a quantidade de recursos disponíveis    
    printers -= iosAlloc.printersUsed;
    modems -= iosAlloc.modemsUsed;
    sata -= iosAlloc.sataUsed;
    scanners -= iosAlloc.scannersUsed;

    // Depois diminui a quantidade de recursos que o processo ainda precisa ter posse.
    process.requiresPrinter -= iosAlloc.printersUsed;
    process.requiresModem -= iosAlloc.modemsUsed;
    process.requiresSata -= iosAlloc.sataUsed;
    process.requiresScanner -= iosAlloc.scannersUsed;
    
    return iosAlloc;
}

void ResourceManager::release(const IOAllocation& ioAlloc) {
    printers += ioAlloc.printersUsed;
    modems += ioAlloc.modemsUsed;
    sata += ioAlloc.sataUsed;
    scanners += ioAlloc.scannersUsed;
}