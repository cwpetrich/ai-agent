#include <iostream>
#include "MazeAction.h"

namespace ai
{
  namespace Sample
  {
    namespace Maze
    {
      Action::Action()
        : type(A_MIN)
      {
        /* empty */
      }
      
      Action::Action(const Action &rhs)
      {
        *this = rhs;
      }
      
      Action::~Action()
      {
        /* empty */
      }
      
      Action &Action::operator=(const Action &rhs)
      {
        this->type   = rhs.type;
        return *this;
      }
      
      bool Action::operator==(const Action &rhs) const
      {
        return (this->type == rhs.type);
      }
      
      void Action::Display() const
      {
        switch(this->type)
          {
          case A_LEFT:
            std::cout << "Left: " << std::endl;
            break;
          case A_RIGHT:
            std::cout << "Right: " << std::endl;
            break;
          case A_UP:
            std::cout << "Up: " << std::endl;
            break;
          case A_DOWN:
            std::cout << "Down: " << std::endl;
            break;
          default:
            std::cout << "Unknown action" << std::endl;
            break;
          }
      }

      bool Action::SetType(int type_in)
      {
        if(type_in > A_MIN && type_in < A_MAX)
          {
            this->type = type_in;
            return true;
          }
        return false;
      }
      
      int Action::GetType() const
      {
        return this->type;
      }
    }
  }
}
