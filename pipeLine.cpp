#include <iostream>
#include <memory>
#include <string>
#include "mutex_Queue.hpp"
#include <chrono>
#include <mutex>



class ActiveObject {
public:
  virtual void SendMessage(std::unique_ptr<My_string> data) = 0;
  
};

class ActiveObject1 : public ActiveObject {
private:
  std::shared_ptr<ActiveObject> next_;
public:
  ActiveObject1(std::shared_ptr<ActiveObject> next) : next_(next) {}
  void SendMessage(std::unique_ptr<My_string> data) override {
    // Perform operation 1 on data
    std::string curr = "";
    for(int i=0; i<data.get()->current.length(); i++){
      if(data.get()->current[i] >= 32 && data.get()->current[i] <= 126)
        curr += data.get()->current[i];
    }
    data.get()->current = curr;
    next_->SendMessage(std::move(data));
  }

};

class ActiveObject2 : public ActiveObject {
public:
  ActiveObject2(std::shared_ptr<ActiveObject> next) : next_(next) {}
  void SendMessage(std::unique_ptr<My_string> data) override {
    std::string curr = "";
    for(int i=0; i<data.get()->current.length(); i++){
      if(data.get()->current[i] >= 'a' && data.get()->current[i] <= 'z'){
        curr += data.get()->current[i] - 32;
      }
      else if(data.get()->current[i] >= 'A' && data.get()->current[i] <= 'Z'){
        curr += data.get()->current[i];
      }
      else{
        curr += ' ';
      }
    }
    data.get()->current = curr;
    next_->SendMessage(std::move(data));
  }
private:
  std::shared_ptr<ActiveObject> next_;
};

class ActiveObject3 : public ActiveObject {
public:
  ActiveObject3(std::shared_ptr<ActiveObject> next) : next_(next) {}
  void SendMessage(std::unique_ptr<My_string> data) override {
            std::string str = data.get()->current;
            data.get()->isPalindrome = -1;
            int i = 0;
            int j = str.length() - 1;
            while (i < j){
                if (str[i] != str[j]){
                    data.get()->isPalindrome = 0;
                    break;
                }
                i++;
                j--;
            }
            if(data.get()->isPalindrome == -1){
              data.get()->isPalindrome = 1;
            }
        next_->SendMessage(std::move(data));
  }
private:
  std::shared_ptr<ActiveObject> next_;
};

class ActiveObject4 : public ActiveObject {
public:
  ActiveObject4() {}
  void SendMessage(std::unique_ptr<My_string> data) override {
    std::string str = "NO";
    if(data.get()->isPalindrome == 1)
      str = "YES";
    std::cout << "Perform operation 4 on data" << std::endl;
    std::cout << "original:" << data.get()->original << std::endl;
    std::cout << "polyndrom:" << str << std::endl;
    
  }
};

int main() {
  auto active_object4 = std::make_shared<ActiveObject4>();
  auto active_object3 = std::make_shared<ActiveObject3>(active_object4);
  auto active_object2 = std::make_shared<ActiveObject2>(active_object3);
  auto active_object1 = std::make_shared<ActiveObject1>(active_object2);

  std::unique_ptr<My_string> data = std::make_unique<My_string>("hello 1123_olleh");
  active_object1->SendMessage(std::move(data));
}