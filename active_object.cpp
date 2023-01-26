//write in cpp active object for nutex_Queue.cpp

// Path: active_object.cpp

#include "mutex_Queue.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>

class active_object
{
public:
    My_string Q;
    void doTask();
private:
    std::mutex queueMutex;
    std::condition_variable queueCond;
    std::queue<My_string> myQueue;
    bool isRunning;
}; 
