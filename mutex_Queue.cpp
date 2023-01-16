#include "mutex_Queue.hpp"


poli_str::poli_str(std::string str){
    original = str;
    current = "";
    isPalindrome = 0;  
}
mutex_Queue::mutex_Queue()
{

}

mutex_Queue::~mutex_Queue()
{

}
void mutex_Queue::enqueue(My_string val){
    std::lock_guard<std::mutex> lock(queueMutex);
    myQueue.push_back(val);
}
My_string mutex_Queue::dequeue(){
    std::lock_guard<std::mutex> lock(queueMutex);
    if (myQueue.empty())
        throw std::runtime_error("Queue is empty");
    My_string val = myQueue.front();
    myQueue.erase(myQueue.begin());
    return val;
}

int main(){
    mutex_Queue myQueue;
    My_string myString("racecar");
    myQueue.enqueue(myString);
    myString = myQueue.dequeue();
    std::cout << myString.original << std::endl;
    return 0;
}