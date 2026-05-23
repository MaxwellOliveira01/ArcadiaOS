#ifndef PROCESS_HPP
#define PROCESS_HPP

struct ProcessData {
    int pid;
    int initTime;
    int priority;
    int cpuTime;
    int workingSetSize;
    bool requiresPrinter;
    bool requiresScanner;
    bool requiresModem;
    bool requiresSata;
};

#endif