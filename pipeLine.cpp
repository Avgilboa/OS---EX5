// g++ pipeLine.cpp mutex_Queue.cpp -lpthread


#include <iostream>
#include <memory>
#include <string>
#include "mutex_Queue.hpp"
#include <chrono>
#include <mutex>
#include <thread>

std::mutex queueMutex;
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


int main() {
  mutex_Queue myQueue;
  std::shared_ptr<ActiveObject> activeObject4 = std::make_shared<ActiveObject4>();
  std::shared_ptr<ActiveObject> activeObject3 = std::make_shared<ActiveObject3>(activeObject4);
  std::shared_ptr<ActiveObject> activeObject2 = std::make_shared<ActiveObject2>(activeObject3);
  std::shared_ptr<ActiveObject> activeObject1 = std::make_shared<ActiveObject1>(activeObject2);
  std::thread t1([&myQueue, &activeObject1] {
    while (true) {
      if(myQueue.empty())
        continue;
      std::lock_guard<std::mutex> lock(queueMutex);
      My_string data = myQueue.dequeue();
      activeObject1->SendMessage(data);
    }
  });
  std::thread t2([&myQueue] {
    while (true) {
      std::string str;
      std::cin >> str;
      if (str == "exit") {
        break;
      }
      My_string data = str;
      std::lock_guard<std::mutex> lock(queueMutex);
      myQueue.enqueue(data);
    }
  });
  t1.join();
  t2.join();
  return 0;
}