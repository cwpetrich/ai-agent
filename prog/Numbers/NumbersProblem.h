#ifndef _NUMBERSPROBLEM_H_
#define _NUMBERSPROBLEM_H_

#include <ai_search.h>
namespace ai
{
  namespace Sample
  {
    namespace Numbers
    {
      class Problem : public ai::Search::Problem
      {
      public:
        Problem(ai::Search::State *initial_state_in);
        virtual ~Problem();

        virtual bool   GoalTest(const ai::Search::State * const state_in) const;
        virtual bool   FindSuccessors(const ai::Search::State * const state_in,
                                      std::vector<ai::Search::ActionStatePair>
                                      &results_out) const;
        virtual double StepCost(const ai::Search::State  * const state1_in,
                                const ai::Search::Action * const action_in,
                                const ai::Search::State  * const state2_in) const;
        virtual double Heuristic(const ai::Search::State  * const state_in) const;

        bool SetGoal(double goal_value_in);
      protected:
        double goal_value;
      private:
      };
    }
  }
}

#endif /* _NUMBERSPROBLEM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
