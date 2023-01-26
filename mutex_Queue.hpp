#include <iostream>
#include <mutex>
#include <vector>
#include <stdexcept>
#include <string>

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
    mutex_Queue();
    ~mutex_Queue();
    void enqueue(My_string val);
    bool empty();
    My_string dequeue();
private:
    std::vector<My_string> myQueue;
    std::mutex queueMutex;
};  // class mutex_Queue
