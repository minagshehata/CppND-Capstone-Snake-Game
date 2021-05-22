#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"

class SpecialMealSpecs
{
  public :
    long remainingTimeInSecs ; 
    std::vector<int> rgb {0xff , 0xFF , 0XFF};
    bool isReached = false ;
    bool timedOut = false ;
    int pos_x = 0 ; 
    int pos_y = 0 ; 
};


class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const snake, SDL_Point const &food);
  void Render(Snake const snake, SDL_Point const &food , SpecialMealSpecs const &SpecialMeal);
  void UpdateWindowTitle(int score, int fps);
  void UpdateWindowTitle(int score, int fps , int remaining_time) ;

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif