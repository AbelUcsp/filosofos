// Copyright [year] <Copyright Owner
#include<bits/stdc++.h>
#include <mutex>
#include <random>
#include <thread>
#include <condition_variable>

#include <queue>

#include <thread>
#include <vector>


const unsigned int kNumberPhilosophers = 5;
const unsigned int kNumberActions = 1000;

int get_random(int low, int high) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distribution(low, high);
  return distribution(gen);
}


class DiningPhilosophers;

struct Philosopher {
  Philosopher(unsigned int id, std::string name, unsigned int number_actions,
              DiningPhilosophers* table)
      : id_(id), name_(name), number_actions_(number_actions), table_(table) {}

  void operator()() {
    for (unsigned int i = 0; i < number_actions_; ++i) {
      Eat();
    }
  }

  void Think();
  void Eat();

  unsigned int id_;
  std::string name_;
  unsigned int number_actions_;
  DiningPhilosophers* table_;
};

class DiningPhilosophers {
 public:
  DiningPhilosophers() {
    std::thread* philosophers[kNumberPhilosophers];
    std::string philosopher_names[] = {
        "Socrates", "Plato", "Aristotle", "Descartes", "Tomas",
    };

    forks.reserve(kNumberPhilosophers);
    for(int i=0; i<kNumberPhilosophers; ++i){
      forks.push_back( std::make_pair( true, false ) );
    }
    forks[4].second = true; //actual max



    for (unsigned int i = 0; i < kNumberPhilosophers; ++i) {
      philosophers[i] = new std::thread(
          Philosopher(i, philosopher_names[i], kNumberActions, this));
    }
    for (unsigned int i = 0; i < kNumberPhilosophers; ++i) {
      philosophers[i]->join();
    }
  }

  void get_max_(){
    int J;
    int i=0;
    for(int i=0; i<kNumberPhilosophers-1; ++i){
      if( (forks[(i+1) % kNumberPhilosophers].first == forks[(i+1) % kNumberPhilosophers].second  ) == true ){
        J = i+1;
      }
    }
    maximo = J% kNumberPhilosophers;
  }


  void PickFork(unsigned int id) {
    forks_.lock();
    get_max_();
    forks[id].first = false;      //bloquea palillo
    forks[maximo].first = false;  
  }

  void LeaveFork(unsigned int id) {    
    forks[id].first = true;     //libera palillo
    forks[maximo].first = true;
    
    forks[id].second = true;    //marca como max 
    forks[maximo].second = true;//marca como max 
    forks_.unlock();
  }

 int maximo;
 std::vector< std::pair <bool, bool> > forks; // first desbloqueado(true) bloqueado(false)
                                              // second max(true)         min(false)
 private:
  std::mutex forks_;
};

void Philosopher::Eat() {
  table_->PickFork(id_);
  
  std::cout << name_ + " is Eating." << std::endl;
  table_->LeaveFork(id_);
}


int main() {
  DiningPhilosophers table;

  return 0;
}
