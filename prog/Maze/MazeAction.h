#ifndef _MAZEACTION_H_
#define _MAZEACTION_H_

#include <ai_search.h>
namespace ai
{
  namespace Sample
  {
    namespace Maze
    {
      class Action : public ai::Search::Action
      {
      public:
        enum
          {
            A_MIN = 0,
            A_LEFT,
            A_RIGHT,
            A_UP,
            A_DOWN,
            A_MAX
          };
        
        Action();
        Action(const Action &rhs);
        virtual ~Action();
        Action &operator=(const Action &rhs);
        bool operator==(const Action &rhs) const;
        virtual void Display() const;

        bool SetType(int type_in);

        int GetType() const;
        
      protected:
        int type; /* A_* */

      private:
      };
    }
  }
}
#endif /* _MAZEACTION_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
