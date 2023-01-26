#include "mutex_Queue.hpp"

int main(){
    mutex_Queue myQueue;
    My_string myString("racecar");
    myQueue.enqueue(myString);
    myString = myQueue.dequeue();
    std::cout << myString.original << std::endl;
    return 0;
}