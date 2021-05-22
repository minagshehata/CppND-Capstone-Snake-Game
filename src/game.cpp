#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      grid_height(grid_height),
      grid_width(grid_width) {
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    if (!LunchTime)
    {
      renderer.Render(snake, food);
    }
    else 
    {
      renderer.Render(snake, food , _SpecialMealSpecsInstance) ;
    }

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      if (!LunchTime)
      {
        renderer.UpdateWindowTitle(score, frame_count);
      }
      else
      {
        renderer.UpdateWindowTitle(score, frame_count, _SpecialMealSpecsInstance.remainingTimeInSecs +1);
      }

      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  static int SpecialMealCounter = 0 ;
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    SpecialMealCounter ++ ; 
    PlaceFood();
    if( (SpecialMealCounter >= 3 ) && (!LunchTime))
    {
      SpecialMealCounter = 0 ; 
       
       OrderSpecialMeal(); 
    }
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
  if (!queue->isEmpty())
  {
       _SpecialMealSpecsInstance =  queue->receive();
       LunchTime = true ;
      if(_SpecialMealSpecsInstance.isReached)
      {
          score += 3 ;
          LunchTime = false ;
          // Grow snake and increase speed.
          snake.GrowBody();
          snake.speed += 0.02;
      }
      else if (_SpecialMealSpecsInstance.timedOut)
      {
         LunchTime = false ;
      }
  }
}

void Game::OrderSpecialMeal()
{
  std::unique_ptr<SpecialMealInterface> SpecialMealIf = std::make_unique<SpecialMealInterface>(this->grid_width ,this->grid_height);
  
  SpecialMealThreads = (std::async(std::launch::async,&SpecialMealInterface::RunSpecialMeal, std::move(SpecialMealIf) ,
                        this->food.x, this->food.y, &this->snake , this->queue )) ;
}
int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

SpecialMealInterface::SpecialMealInterface(std::size_t grid_width, std::size_t grid_height)
    : engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
}

void SpecialMealInterface::RunSpecialMeal( int food_x, int food_y ,Snake *snake, std::shared_ptr<MessageQueue<SpecialMealSpecs>> queue )
{
  SpecialMealSpecs SpecialMealSpecsInstance ;
  long currentTime = SDL_GetTicks(); 
  long initialTime = SDL_GetTicks(); 
  long blinkTime = SDL_GetTicks(); 
  long timeDifference = 0;
  bool blinkFlag = false ;
  // receive msg q from game to have 
  PlaceSpecialMeal(food_x , food_y , *snake);
  SpecialMealSpecsInstance.pos_x = SpecialMealInstanse->x;
  SpecialMealSpecsInstance.pos_y = SpecialMealInstanse->y;
  while (true)
  {
      currentTime = SDL_GetTicks(); 
      timeDifference = currentTime - initialTime ;
      int new_x = static_cast<int>(snake->head_x);
      int new_y = static_cast<int>(snake->head_y);
      // check snake positioning 
      
      if( snake->SnakeCell(SpecialMealInstanse->x, SpecialMealInstanse->y) )
      {
        SpecialMealSpecsInstance.isReached = true ;
        SpecialMealSpecsInstance.remainingTimeInSecs = 0 ; 
        SpecialMealSpecsInstance.timedOut = false ; 
        //update status 
        break ; 
      }
      else if (timeDifference < SpecialMealInstanse->TimeOutInSecs*1000)
      {
        
       // blink color ; 
        SpecialMealSpecsInstance.isReached = false ;
        SpecialMealSpecsInstance.remainingTimeInSecs = ((SpecialMealInstanse->TimeOutInSecs*1000 ) - timeDifference) /1000 ; ; 
        SpecialMealSpecsInstance.timedOut = false ; 
        if ((currentTime - blinkTime) > 500)
        {
          blinkTime = currentTime ;
          blinkFlag = !blinkFlag;
        }
        if (blinkFlag)
          SpecialMealSpecsInstance.rgb = {0, 191, 255} ; 
        else
          SpecialMealSpecsInstance.rgb = {240, 15, 255} ; 
      }
      else
      {
        SpecialMealSpecsInstance.isReached = false ;
        SpecialMealSpecsInstance.remainingTimeInSecs = 0 ; ; 
        SpecialMealSpecsInstance.timedOut = true ; 
        break ; 
      }
      queue->send(std::move(SpecialMealSpecsInstance));
      
  }
  queue->send(std::move(SpecialMealSpecsInstance));
}


void SpecialMealInterface::PlaceSpecialMeal(int food_x, int food_y , Snake snake){
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      if ((food_x != x) && (food_y != y) )
      {
        SpecialMealInstanse->x = x;
        SpecialMealInstanse->y = y;
        return;
      }
    }
  }
}
