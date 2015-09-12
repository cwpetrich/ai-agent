#include <iostream>
#include "MazeState.h"
#include "MazeAction.h"

namespace ai
{
  namespace Sample
  {
    namespace Maze
    {
      State::State()
      {
        /* empty */
      }
      
      State::State(const State &rhs)
      {
        *this = rhs;
      }
      
      State::~State()
      {
        /* empty */
      }
      
      State &State::operator=(const State &rhs)
      {
        this->x = rhs.x;
        this->y = rhs.y;
        return *this;
      }
      
      bool State::operator==(const State &rhs) const
      {
        return (this->x == rhs.x && this->y == rhs.y);
      }
      
      void State::Display() const
      {
        std::cout << this->x << "," << this->y << std::endl;
      }
      
      bool State::IsEqual(const ai::Search::State * const state_in) const
      {
        const State * const state = dynamic_cast<const State * const>(state_in);
        return (*this) == (*state);
      }
      
      int State::GetX() const
      {
        return this->x;
      }
      int State::GetY() const
      {
        return this->y;
      }

      bool State::SetX(const int &x_in)
      {
        this->x = x_in;
        return true;
      }
      bool State::SetY(const int &y_in)
      {
        this->y = y_in;
        return true;
      }
    }
  }
}

/* Local Variables: */
/* mode:c++         */
/* End:             */
