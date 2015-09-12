#include "NumbersProblem.h"
#include "NumbersState.h"
#include "NumbersAction.h"
#include <ai_search.h>
#include <cmath>
namespace ai
{
  namespace Sample
  {
    namespace Numbers
    {
      Problem::Problem(ai::Search::State *initial_state_in)
        : ai::Search::Problem(initial_state_in),
          goal_value(0)
      {
        /* empty */
      }
      Problem::~Problem()
      {
        /* empty */
      }
      
      bool   Problem::GoalTest(const ai::Search::State * const state_in) const
      {
        const State * const state = dynamic_cast<const State * const>(state_in);
        if((state->GetUnused().size() == 0) &&
           (state->GetStack().size() == 1) &&
           (std::fabs(this->goal_value - state->GetStack()[0]) < 0.01))
          {
            return true;
          }
        return false;
      }
      
      bool   Problem::FindSuccessors(const ai::Search::State * const state_in,
                                     std::vector<ai::Search::ActionStatePair>
                                     &results_out) const
      {
        const State * const state = dynamic_cast<const State * const>(state_in);
        
        /* push each remaining number */
        std::set<double>::const_iterator sit;
        
        for(sit = state->GetUnused().begin();
            sit != state->GetUnused().end();
            sit++)
          {
            State  *new_state  = new State(*state);
            Action *new_action = new Action();
            
            new_state->PushStack(*sit);
            
            new_action->SetType(Action::A_PUSH_NUMBER);
            new_action->SetNumber(*sit);

            ai::Search::ActionStatePair asp(new_state, new_action);
            results_out.push_back(asp);
          }
        
        
        /* push each operator */
        if(state->GetStack().size() >= 2)
          {
            int op;
            for(op = (int)Action::O_MIN+1; op < (int)Action::O_MAX; op++)
              {
                State  *new_state  = new State(*state);
                Action *new_action = new Action();
                
                new_state->PushOperator(op);
                
                new_action->SetType(Action::A_PUSH_OPERATOR);
                new_action->SetOperator(op);
                
                ai::Search::ActionStatePair asp(new_state, new_action);
                results_out.push_back(asp);
              }
          }
        return (results_out.size() > 0);
      }
      
      double Problem::StepCost(const ai::Search::State  * const state1_in,
                               const ai::Search::Action * const action_in,
                               const ai::Search::State  * const state2_in) const
      {
        const State  * const state1 = dynamic_cast<const State * const>(state1_in);
        const Action * const action = dynamic_cast<const Action * const>(action_in);
        const State  * const state2 = dynamic_cast<const State * const>(state2_in);

        /* temporarily fool compiler to think we are using these variables */
        state1->GetUnused();
        action->GetOperator();
        state2->GetUnused();
        /* all actions cost 1 */
        return 1.0;
      }
      
      double Problem::Heuristic(const ai::Search::State  * const state_in) const
      { /* No hints */
        return 0.0;
      }
      
      bool Problem::SetGoal(double goal_value_in)
      {
        this->goal_value = goal_value_in;
        return true;
      }
    }
  }
}
