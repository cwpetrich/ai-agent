#ifndef _MAZESTATE_H_
#define _MAZESTATE_H_
#include <ai_search.h>
#include <vector>
#include <set>
namespace ai
{
  namespace Sample
  {
    namespace Maze
    {
      class State : public ai::Search::State
      {
      public:
        State();
        State(const State &rhs);
        virtual ~State();
        State &operator=(const State &rhs);
        bool operator==(const State &rhs) const;
        virtual void Display() const;
        virtual bool IsEqual(const ai::Search::State * const state_in) const;

        int GetX() const;
        int GetY() const;

        bool SetX(const int &x_in);
        bool SetY(const int &y_in);

      protected:
        int x;
        int y;
      private:
      };
    }
  }
}
#endif /* _MAZESTATE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */

