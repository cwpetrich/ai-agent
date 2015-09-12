#ifndef _RECTPROBLEM_H_
#define _RECTPROBLEM_H_
#include "ai_search.h"

namespace cgl
{
  class RectProblem : public ai::Search::Problem
  {
  public:
    RectProblem(ai::Search::State *initial_state_in);
    virtual ~RectProblem();

    virtual bool   GoalTest(const ai::Search::State * const state_in) const;
    virtual std::vector<ai::Search::Action *> Actions(const ai::Search::State * const state_in);
    virtual ai::Search::State *Result(const ai::Search::State * const state_in, const ai::Search::Action * const action_in);

    virtual double StepCost(const ai::Search::State  * const state1_in,
			    const ai::Search::Action * const action_in,
			    const ai::Search::State  * const state2_in) const;
    virtual double Heuristic(const ai::Search::State  * const state_in) const;

    void SetFindAll();
    
  protected:
    int board_width;
    int board_height;
    bool find_all;
  private:
  };
}
#endif /* _RECTPROBLEM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
