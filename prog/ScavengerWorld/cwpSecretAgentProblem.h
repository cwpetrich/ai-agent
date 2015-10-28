#pragma once

#include "cwpSecretAgentState.h"
#include "cwpSecretAgentAction.h"
#include "cwpSecretAgentModel.h"
#include <ai_search.h>
#include <ai_scavenger.h>

namespace cwp {
	namespace Scavenger {

		class Problem : public ai::Search::Problem {
		public:
			Problem(ai::Search::State * initial_state, cwp::Scavenger::SecretAgentModel * const model, bool to_base);
			virtual ~Problem();
      virtual std::vector<ai::Search::Action *> Actions(const ai::Search::State * const state_in);
      virtual bool GoalTest(const ai::Search::State * const state_in) const;
      virtual ai::Search::State *Result(const ai::Search::State * const state_in,
      													const ai::Search::Action * const action_in);
      virtual double StepCost(const ai::Search::State * const state1_in,
      								const ai::Search::Action * const action_in,
      								const ai::Search::State * const state2_in) const;
      virtual double Heuristic(const ai::Search::State  * const state_in) const;
		protected:
			bool to_base;
			cwp::Scavenger::SecretAgentModel * model;
      cwp::Scavenger::State * initial_state;
    private:
		};
	}
}