#include <memory>
#include "mutex_Queue.hpp"
#include <chrono>



class ActiveObject {
public:
  virtual void SendMessage(std::unique_ptr<My_string> data) = 0;
  
};

class ActiveObject1 : public ActiveObject {
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
    // Perform operation 2 on data
    next_->SendMessage(std::move(data));
  }
private:
  std::shared_ptr<ActiveObject> next_;
};

class ActiveObject3 : public ActiveObject {
public:
  ActiveObject3() {}
  void SendMessage(std::unique_ptr<My_string> data) override {
    std::cout << "Perform operation 3 on data" << std::endl;
  }
};

int main() {
  auto active_object3 = std::make_shared<ActiveObject3>();
  auto active_object2 = std::make_shared<ActiveObject2>(active_object3);
  auto active_object1 = std::make_shared<ActiveObject1>(active_object2);

  std::unique_ptr<My_string> data = std::make_unique<My_string>();
  active_object1->SendMessage(std::move(NULL));
}