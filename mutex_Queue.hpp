#include <iostream>
#include <mutex>
#include <vector>
#include <stdexcept>
#include <string>

typedef struct poli_str{
    std::string original;
    std::string current;
    int isPalindrome;
    poli_str(std::string str);
} My_string;

class mutex_Queue
{
public:
    mutex_Queue();
    ~mutex_Queue();
    void enqueue(My_string val);
    My_string dequeue();
private:
    std::vector<My_string> myQueue;
    std::mutex queueMutex;
};  // class mutex_Queue
