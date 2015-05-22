#ifndef _NUMBERSACTION_H_
#define _NUMBERSACTION_H_

#include <ai_search.h>
namespace ai
{
  namespace Sample
  {
    namespace Numbers
    {
      class Action : public ai::Search::Action
      {
      public:
        enum
          {
            A_MIN = 0,
            A_PUSH_NUMBER,
            A_PUSH_OPERATOR,
            A_MAX
          };
        enum
          {
            O_MIN = 0,
            O_PLUS,
            O_MINUS,
            O_TIMES,
            O_DIVIDE,
            O_MAX
          };
        
        Action();
        Action(const Action &rhs);
        virtual ~Action();
        Action &operator=(const Action &rhs);
        bool operator==(const Action &rhs) const;
        virtual void Display() const;

        bool SetType(int type_in);
        bool SetOperator(int op_in);
        bool SetNumber(double number_in);

        int GetType() const;
        int GetOperator() const;
        double GetNumber() const;
        
      protected:
        int type; /* A_* */
        int op;   /* O_* */
        double number;

      private:
      };
    }
  }
}
#endif /* _NUMBERSACTION_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
