#ifndef GLOBAL_QUEUE_HPP
#define GLOBAL_QUEUE_HPP

#include <deque>
#include "../process/process.hpp"

class GlobalQueue {
public:
    void push_back(const ProcessData& process);
    ProcessData firstOccurrence(const bool& realTime);
    void pop(ProcessData& process);
    bool empty() const;

private:
    std::deque<ProcessData> queue;
};

#endif // GLOBAL_QUEUE_HPP