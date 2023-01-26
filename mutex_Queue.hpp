#include <iostream>
#include <mutex>
#include <vector>
#include <stdexcept>
#include <string>
#include <thread>

class My_string{
    public:
        My_string(std::string str){
            original = str;
            current = str;
            
        }
        My_string(){
            original = "";
            current = "";
        }
        bool isPalindrome = 0;
        std::string original;
        std::string current;
};

class mutex_Queue
{
public:
    mutex_Queue(){}
    ~mutex_Queue(){}
    void enqueue(My_string val){
        std::lock_guard<std::mutex> lock(queueMutex);
        myQueue.push_back(val);
    }
    bool empty(){
        std::lock_guard<std::mutex> lock(queueMutex);
        return myQueue.empty();
    }
    My_string dequeue(){
        std::lock_guard<std::mutex> lock(queueMutex);
        if (myQueue.empty())
            throw std::runtime_error("Queue is empty");
        My_string val = myQueue.front();
        myQueue.erase(myQueue.begin());
        return val;
    }
    My_string front(){
        std::lock_guard<std::mutex> lock(queueMutex);
        if (myQueue.empty())
            throw std::runtime_error("Queue is empty");
        return myQueue.front();
    }
    void pop(){
        std::lock_guard<std::mutex> lock(queueMutex);
        if (myQueue.empty())
            throw std::runtime_error("Queue is empty");
        myQueue.erase(myQueue.begin());
    }

    
private:
    std::vector<My_string> myQueue;
    std::mutex queueMutex;
};  // class mutex_Queue
