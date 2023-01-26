#include <iostream>
#include <memory>
#include <string>
#include "mutex_Queue.hpp"
#include <chrono>
#include <mutex>
#include<thread>
#include <condition_variable>
#include <queue>
void pipline(My_string str);

class ActiveObject {
public:
    void start() {
        thread_ = std::thread(&ActiveObject::run, this);
    }
    void stop() {
        running_ = false;
        thread_.join();
    }
    void doSomething() {
        std::cout<<"123"<<std::endl;
        std::unique_lock<std::mutex> lock(mutex_);
        tasks_.push(123);
        this->run();
        condition_.notify_one();
    }
private:
    void run() {
                            std::cout<<"1234567"<<std::endl;

        while (running_) {
            std::unique_lock<std::mutex> lock(mutex_);
            while (tasks_.empty()) {
                      std::cout<<"1234"<<std::endl;

                condition_.wait(lock);
            }
                    std::cout<<"12345"<<std::endl;

            auto task = tasks_.front();
            tasks_.pop();
            lock.unlock();
            this->stop();
            pipline(My_string("hih"));
        }
    }
    std::thread thread_;
    std::mutex mutex_;
    std::condition_variable condition_;
    std::queue<int> tasks_;
    bool running_ = true;
};

class Stage {
public:    
  virtual void SendMessage(std::unique_ptr<My_string> data) = 0;
  
};

class Stage1 : public Stage {
private:
  std::shared_ptr<Stage> next_;
public:
  Stage1(std::shared_ptr<Stage> next) : next_(next) {}
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

class Stage2 : public Stage {
public:
  Stage2(std::shared_ptr<Stage> next) : next_(next) {}
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
  std::shared_ptr<Stage> next_;
};

class Stage3 : public Stage {
public:
  Stage3(std::shared_ptr<Stage> next) : next_(next) {}
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
  std::shared_ptr<Stage> next_;
};

class Stage4 : public Stage {
public:
  Stage4() {}
  void SendMessage(std::unique_ptr<My_string> data) override {
    std::string str = "NO";
    if(data.get()->isPalindrome == 1)
      str = "YES";
    std::cout << "Perform operation 4 on data" << std::endl;
    std::cout << "original:" << data.get()->original << std::endl;
    std::cout << "polyndrom:" << str << std::endl;
    
  }
};

void pipline(My_string str){
  auto stage4 = std::make_shared<Stage4>();
  auto stage3 = std::make_shared<Stage3>(stage4);
  auto stage2 = std::make_shared<Stage2>(stage3);
  auto stage1 = std::make_shared<Stage1>(stage2);

  std::unique_ptr<My_string> data = std::make_unique<My_string>(str);
  stage1->SendMessage(std::move(data));
}


int main() {
  ActiveObject a;
  a.doSomething();
}