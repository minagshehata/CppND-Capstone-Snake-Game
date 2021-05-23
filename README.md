# CPPND: Capstone Snake Game Project with Special Meal Feature

This is a starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="snake_game.gif"/>

## Added Feature 
* This version of snake game has extra feature that "after three successive trial to get the food ,
There is an offer called "SpecialMeal" which is similar to the normal food but it increases the score by 3 points.
But unfortunately it's limited to just 5 Seconds.
* You can differs the special one as its color blinking and countdown timer appears in the title bar.


## Implementation and Changes Notes
* cmake :
    * 1- Add the needed compiler flag "-pthread" and other flags requried by SDL2 Library

* game : 
    * 1 - create class SpecialMealClass inherit specs from SDL_Point + timeout configuration
    * 2 - create Message Queue class to handle data transferring between SpecialMealInterface class and game class
    * 3 - create class SpecialMealInterface to handle Special Meal proccess in a parallel thread
        has a unique pointer to SpecialMealClass to control the allocation and de allocation of the object in heap.
    * 4 - add future instance as private member , which is used to trigger the concurrent processing.
    * 5 - add grid_height , grid_width as a members in the game object 
    * 6 - add LunchTime flag as new private member in game object
    * 7 - add the needed implementation in update method of the game to trigger 
    * 8 - add shared pointer to message queue class to handle the data transferring 
    * 9 - add OrderSpecialMeal method to trigger the concurrent processing .

* render : 
    * 1 - overload render method to take extra parameter of SpecialMealSpecs
    * 2 - overload UpdateWindowTitle method to take extra parameter of SpecialMealSpecs

## Rubric Points are addressed as following :
  * README Requirements :
    * The README is included with the project and has instructions for building/running the project.
      * Point is Covered : already added based on the original repo.  
    * If any additional libraries are needed to run the project, these are indicated with cross-platform installation instructions.
      * Point is Covered : already added based on the original repo.  
    * You can submit your writeup as markdown or pdf.
      * Point is Covered : Done as modified MD file in the repo.
    * The README describes the project you have built.
      * Point is Covered : sections *Added Feature* and *Implementation and changes notes* are covering the point.
    * The README also indicates the file and class structure, along with the expected behavior or output of the program.
      * Point is Covered : sections *Added Feature* and *Implementation and changes notes* are covering the point.
    * The README indicates which rubric points are addressed. The README also indicates where in the code (i.e. files and line numbers) that the rubric points are addressed.
      * Point is Covered : covered in this section.
  * Compiling and Testing :
      * Point is Covered : The peiject is compiled and tested on both of my local machine and also the remote desktop provided by Udacity.
  * Loops, Functions, I/O :
    * A variety of control structures are used in the project.
      * Point is Covered : In game.cpp . RunSpecialMeal() , PlaceSpecialMeal() are implemented with conditional statements to control the handling of the special meal.
      * In game.cpp Game::update() is updated to trigger spicial meak thread and handle the message Queue also using control structure.
    * The project code is clearly organized into functions.
      * Point is Covered : the new implementation in game.cpp is devided into thre new functions to organize the implementation according to the required functionality.
    * The project reads data from an external file or writes data to a file as part of the necessary operation of the program.
      * Point isn't covered : As i couldn't find suitable context to read data from external files .
    * The project accepts input from a user as part of the necessary operation of the program.
      * Point isn't covered in the new implementation but originally the controllers and SDL part is doing that job.
  * Object Oriented Programming : 
    * The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks.
      * Point is Covered : in game.h you can find the following new classes to serve the intended functionality. 
          * game(just updated)
          * MessageQueue
          * SpecialMealClass
          * SpecialMealInterface
    * All class data members are explicitly specified as public, protected, or private.
      * Point is Covered : in game.h you can find all the new attributes of the following new classes are specified to be private or public
          * game(just updated)
          * MessageQueue
          * SpecialMealClass
          * SpecialMealInterface
    * All class members that are set to argument values are initialized through member initialization lists.
      * Done for *SpecialMealInterface* in game.cpp
    * All class member functions document their effects, either through function names, comments, or formal documentation. Member functions do not change program state in undocumented ways.
      * all the new implementation in game.cpp is well commented to show the intended behavior.
    * Appropriate data and functions are grouped into classes. Member data that is subject to an invariant is hidden from the user. State is accessed via member functions.
      * class *SpecialMealInterface* in game.h is acting like user interface for the SpecialMealClass.
    * Inheritance hierarchies are logical. Composition is used instead of inheritance when appropriate. Abstract classes are composed of pure virtual functions. Override functions are specified.
      * Class SpecialMealClass in game.h inherits from SDL point , for me it's logical as it's the same just add timeout parameter.
    * One function is overloaded with different signatures for the same function name.
      * two functions are overloaded in render.h (Renderer::Render , Renderer::UpdateWindowTitle) 
    * One member function in an inherited class overrides a virtual base class member function.
      * Not targeted.
    * One function is declared with a template that allows it to accept a generic parameter.
      * Done for Message Queue Class in game.h . 
  * Memory Management : 
    * At least two variables are defined as references, or two functions use pass-by-reference in the project code.
      * in game.cpp : RunSpecialMeal() and Send() functions are using refrencing to pass value.
    * At least one class that uses unmanaged dynamically allocated memory, along with any class that otherwise needs to modify state upon the termination of an object, uses a destructor.
    * The project follows the Resource Acquisition Is Initialization pattern where appropriate, by allocating objects at compile-time, initializing objects when they are declared, and utilizing scope to ensure their automatic destruction.
      * in game.cpp , find *SpecialMealInterface*  use smart pointer ex:unique ptr to apply RAII and rule of 5.
    * For all classes, if any one of the copy constructor, copy assignment operator, move constructor, move assignment operator, and destructor are defined, then all of these functions are defined.
      * No need as smart pointer handle all these stuffs.
    * For classes with move constructors, the project returns objects of that class by value, and relies on the move constructor, instead of copying the object.
      * in game.cpp find the using of move semantics whe we use Send() function on message queue class and also for *SpecialMealIf* in function OrderSpecialMeal()
    * The project uses at least one smart pointer: unique_ptr, shared_ptr, or weak_ptr. The project does not use raw pointers.
      * in game.h , *SpecialMealInterface*  
  * Concurrency : 
    * The project uses multiple threads in the execution.
       * OrderSpecialMeal() function in game.ccp , invoke new thread by using std::async .
    * A promise and future is used to pass data from a worker thread to a parent thread in the project code.
      * OrderSpecialMeal() function in game.ccp , future is directky used.
    * A mutex or lock (e.g. std::lock_guard or `std::unique_lock) is used to protect data that is shared across multiple threads in the project code.
      * in game.h, find the implementation of MessageQueue class std::unique_lock<std::mutex> uLock(_mutex);
    * A std::condition_variable is used in the project code to synchronize thread execution.
      * in game.h, _cond.wait and  _cond.notify_one(); are used in receive() and send() function in MessageQueue class.


## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.


## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
