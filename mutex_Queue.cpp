#include "mutex_Queue.hpp"

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
bool mutex_Queue::empty(){
    std::lock_guard<std::mutex> lock(queueMutex);
    return myQueue.empty();
}
My_string mutex_Queue::front(){
    std::lock_guard<std::mutex> lock(queueMutex);
    if (myQueue.empty())
        throw std::runtime_error("Queue is empty");
    return myQueue.front();
}
void mutex_Queue::pop(){
    std::lock_guard<std::mutex> lock(queueMutex);
    if (myQueue.empty())
        throw std::runtime_error("Queue is empty");
    myQueue.erase(myQueue.begin());
}

// int main(){
//     mutex_Queue myQueue;
//     My_string myString("racecar");
//     myQueue.enqueue(myString);
//     myString = myQueue.dequeue();
//     std::cout << myString.original << std::endl;
//     return 0;
// }