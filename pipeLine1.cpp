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
    next_->SendMessage(std::move(data));
  }

};

class ActiveObject2 : public ActiveObject {
public:
  ActiveObject2(std::shared_ptr<ActiveObject> next) : next_(next) {}
  void SendMessage(std::unique_ptr<My_string> data) override {
    std::cout << "Perform operation 2 on data" << std::endl;
    data.get()->original = "world";
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
            int i = 0;
            int j = str.length() - 1;
            while (i < j){
                if (str[i] != str[j])
                    data.get()->isPalindrome = 0;
                i++;
                j--;
            }
            data.get()->isPalindrome = 1;
        next_->SendMessage(std::move(data));
  }
private:
  std::shared_ptr<ActiveObject> next_;
};

class ActiveObject4 : public ActiveObject {
public:
  ActiveObject4() {}
  void SendMessage(std::unique_ptr<My_string> data) override {
    std::cout << "Perform operation 4 on data" << std::endl;
    std::cout << "Data is: " << data.get()->original << std::endl;
  }
};

int main() {
  auto active_object4 = std::make_shared<ActiveObject4>();
  auto active_object3 = std::make_shared<ActiveObject3>(active_object4);
  auto active_object2 = std::make_shared<ActiveObject2>(active_object3);
  auto active_object1 = std::make_shared<ActiveObject1>(active_object2);

  std::unique_ptr<My_string> data = std::make_unique<My_string>("hello");
  active_object1->SendMessage(std::move(data));
}