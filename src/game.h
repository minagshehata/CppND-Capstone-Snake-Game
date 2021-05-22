#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include <thread>
#include <future>
#include <mutex>
#include <queue>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

template <class T>
class MessageQueue
{
public:
    bool isEmpty(){ return _messages.empty(); };
    T receive()
    {
        // perform queue modification under the lock
        std::unique_lock<std::mutex> uLock(_mutex);
        _cond.wait(uLock, [this] { return !_messages.empty(); }); // pass unique lock to condition variable

        // remove last vector element from queue
        T msg = std::move(_messages.back());
        _messages.pop_back();

        return msg; // will not be copied due to return value optimization (RVO) in C++
    }

    void send(T &&msg)
    {
        // simulate some work
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // perform vector modification under the lock
        std::lock_guard<std::mutex> uLock(_mutex);

        // add vector to queue
        _messages.push_back(std::move(msg));
        _cond.notify_one(); // notify client after pushing new Vehicle into vector
    }

private:
    std::mutex _mutex;
    std::condition_variable _cond;
    std::deque<T> _messages;
};

class SpecialMealClass : public SDL_Point
{
  public : 
     SpecialMealClass() {};
    int TimeOutInSecs = 5 ; 
};


class SpecialMealInterface
{
  private : 
    std::unique_ptr<SpecialMealClass> SpecialMealInstanse = std::make_unique<SpecialMealClass>(); 
    

  public :
    SpecialMealInterface(std::size_t grid_width, std::size_t grid_height);
    void PlaceSpecialMeal(int food_x, int food_y , Snake snake) ;
    void RunSpecialMeal( int food_x, int food_y ,Snake *snake, std::shared_ptr<MessageQueue<SpecialMealSpecs>> queue ) ;
    std::random_device dev;
    std::mt19937 engine;
    std::uniform_int_distribution<int> random_w;
    std::uniform_int_distribution<int> random_h;

};

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  Snake snake;
  SDL_Point food;
  bool LunchTime = false; 
  SpecialMealSpecs _SpecialMealSpecsInstance ; 
  std::future<void> SpecialMealThread;
  std::shared_ptr<MessageQueue<SpecialMealSpecs>> queue = std::make_shared<MessageQueue<SpecialMealSpecs>>();

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  std::size_t grid_width; 
  std::size_t grid_height;

  int score{0};

  void PlaceFood();
  void OrderSpecialMeal();
  void Update();
};

#endif