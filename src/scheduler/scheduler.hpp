#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <deque>
#include "../process/process.hpp"
#include "real_time_queue.hpp"
#include "user_queue.hpp"

class Scheduler {
    private:
        std::deque<ProcessData> processes;
    public:
        void addProcess(const ProcessData& process);

        ProcessData getProcess();

        bool isEmpty() const;

        void Scheduler::scale(const ProcessData& process, RealTimeQueue& realTimeQueue, UserQueue& userQueue, bool& executed);
};

#endif