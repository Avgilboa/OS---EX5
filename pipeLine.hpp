// g++ pipeLine.cpp mutex_Queue.cpp -lpthread


#include <iostream>
#include <memory>
#include <string>
#include "mutex_Queue.hpp"
#include <chrono>
#include <mutex>
#include <thread>
#include <pthread.h>
#include<stdlib.h>
#include<stdio.h>


std::mutex queueMutex;
std::thread t2;
class ActiveObject {
public:
  virtual void SendMessage(My_string & data) = 0;
  
};

class ActiveObject1 : public ActiveObject {
private:
  std::shared_ptr<ActiveObject> next_;
public:
  ActiveObject1(std::shared_ptr<ActiveObject> next) : next_(next) {}
  void SendMessage(My_string & data) override {
    // Perform operation 1 on data
    std::string curr = "";
    for(int i=0; i<data.current.length(); i++){
      if(data.current[i] >= 32 && data.current[i] <= 126)
        curr += data.current[i];
    }
    data.current = curr;
    next_->SendMessage(data);
  }

};

class ActiveObject2 : public ActiveObject {
public:
  ActiveObject2(std::shared_ptr<ActiveObject> next) : next_(next) {}
  void SendMessage(My_string & data) override {
    std::string curr = "";
    for(int i=0; i<data.current.length(); i++){
      if(data.current[i] >= 'a' && data.current[i] <= 'z'){
        curr += data.current[i] - 32;
      }
      else if(data.current[i] >= 'A' && data.current[i] <= 'Z'){
        curr += data.current[i];
      }
      else{
        curr += ' ';
      }
    }
    data.current = curr;
    next_->SendMessage(data);
  }
private:
  std::shared_ptr<ActiveObject> next_;
};

class ActiveObject3 : public ActiveObject {
public:
  ActiveObject3(std::shared_ptr<ActiveObject> next) : next_(next) {}
  void SendMessage(My_string & data) override {
            std::string str = data.current;
            data.isPalindrome = -1;
            int i = 0;
            int j = str.length() - 1;
            while (i < j){
                if (str[i] != str[j]){
                    data.isPalindrome = 0;
                    break;
                }
                i++;
                j--;
            }
            if(data.isPalindrome == -1){
              data.isPalindrome = 1;
            }
        next_->SendMessage(data);
  }
private:
  std::shared_ptr<ActiveObject> next_;
};

class ActiveObject4 : public ActiveObject {
public:
  ActiveObject4() {}
  void SendMessage(My_string & data) override {
    std::string str = "NO";
    if(data.isPalindrome == 1)
      str = "YES";
    std::cout << "Perform operation 4 on data" << std::endl;
    std::cout << "original:" << data.original << std::endl;
    std::cout << "polyndrom:" << str << std::endl;
    
  }
};
void run(mutex_Queue *q2,ActiveObject *act){
  while (true) {
          if(q2->empty())
            continue;
          std::lock_guard<std::mutex> lock(queueMutex);
          My_string data = q2->dequeue();
          act->SendMessage(data);
        }
}
class pipeLine{
  public:
    
    pipeLine(){  
      std::cout<<"hi!"<<std::endl;
      activeObject4 = std::make_shared<ActiveObject4>();
      activeObject3 = std::make_shared<ActiveObject3>(activeObject4);
      activeObject2 = std::make_shared<ActiveObject2>(activeObject3);
      activeObject1 = std::make_shared<ActiveObject1>(activeObject2);
      std::cout<<"hi!"<<std::endl;

      // std::thread t1([&] {
      // });
      // std::thread t1(run,std::ref(myQueue), &activeObject1);
      // t1.join();
      // pthread_t thread1;
      // int t1;
      // t1 = pthread_create(&thread1,NULL, run,(void*) &myQueue,&activeObject1);
      // //t2 = std::thread(run,myQueue, &activeObject1);
    }
  
    mutex_Queue myQueue;
    std::shared_ptr<ActiveObject> activeObject1;
    std::shared_ptr<ActiveObject> activeObject2;
    std::shared_ptr<ActiveObject> activeObject3;
    std::shared_ptr<ActiveObject> activeObject4;
    void run(){
        while (true) {
          if(myQueue.empty())
            continue;
          std::lock_guard<std::mutex> lock(queueMutex);
          My_string data = myQueue.dequeue();
          activeObject1->SendMessage(data);
        }
    }
    // void get(string str){
    //   myQueue.enqueue(My_string(str));
    // }
};

// int main(){
//   pipeLine pl;
//   std::thread t1([&pl]{
//     pl.run();
//   });
//   pl.myQueue.enqueue(My_string("helloooww"));
//   pl.myQueue.enqueue(My_string("heeh"));
//   std::thread t2([&pl]{
//     pl.myQueue.enqueue(My_string("helloooww"));
//     pl.myQueue.enqueue(My_string("heeh"));
//   });
//   t2.join();
//   t1.join();
  
  
// }
// int main() {
//   mutex_Queue myQueue;
//   activeObject4 = std::make_shared<ActiveObject4>();
//   activeObject3 = std::make_shared<ActiveObject3>(activeObject4);
//   activeObject2 = std::make_shared<ActiveObject2>(activeObject3);
//   activeObject1 = std::make_shared<ActiveObject1>(activeObject2);
//   std::thread t1([&myQueue, &activeObject1] {
//     while (true) {
//       if(myQueue.empty())
//         continue;
//       std::lock_guard<std::mutex> lock(queueMutex);
//       My_string data = myQueue.dequeue();
//       activeObject1->SendMessage(data);
//     }
//   });

//   std::thread t2([&myQueue] {
//     while (true) {
//       std::string str;
//       std::cin >> str;
//       if (str == "exit") {
//         exit(1);
//       }
//       My_string data = str;
//       std::lock_guard<std::mutex> lock(queueMutex);
//       myQueue.enqueue(data);
//     }
//   });
//   t1.join();
//   t2.join();
  
//   return 0;
// }