#include "mutex_Queue.hpp"

using namespace std;
mutex_Queue::mutex_Queue()
{

}

mutex_Queue::~mutex_Queue()
{

}
void mutex_Queue::enqueue(My_string val){
    lock_guard<mutex> lock(queueMutex);
    myQueue.push_back(val);
}
My_string mutex_Queue::dequeue(){
    lock_guard<mutex> lock(queueMutex);
    if (myQueue.empty())
        throw runtime_error("Queue is empty");
    My_string val = myQueue.front();
    myQueue.erase(myQueue.begin());
    return val;
}
bool mutex_Queue::empty(){
    return myQueue.empty();
}

int main(){
    mutex_Queue myQueue;
    My_string myString("racecar");
    myQueue.enqueue(myString);
    myString = myQueue.dequeue();
    cout << myString.original << endl;
    return 0;
}