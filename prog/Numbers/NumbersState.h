#ifndef _NUMBERSSTATE_H_
#define _NUMBERSSTATE_H_
#include <ai_search.h>
#include <vector>
#include <set>
namespace ai
{
  namespace Sample
  {
    namespace Numbers
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

        std::vector<double> &GetStack();
        const std::vector<double> &GetStack() const;
        std::set<double> &GetUnused();
        const std::set<double> &GetUnused() const;

        /* adds new_number to the top of the stack,
         * and removes it from the set */
        bool PushStack(const double &new_number);

        /* adds new_number to the set */
        bool AddNumber(const double &new_number);
        
        /* applies new_operator to top values of stack,
         * removing them, and replacing with the result */
        bool PushOperator(const int &new_operator);

      protected:
        std::vector<double> number_stack;
        std::set<double> unused_numbers;
      private:
      };
    }
  }
}
#endif /* _NUMBERSSTATE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
