#ifndef _SECRETAGENT_H_
#define _SECRETAGENT_H_

#include <ai_scavenger.h>
#include "cwpSecretAgentModel.h"
#include "cwpSecretAgentAction.h"
#include "cwpSecretAgentState.h"
#include "cwpSecretAgentProblem.h"

namespace cwp
{
  namespace Scavanger
  {
    class SecretAgent : public ai::Agent::AgentProgram
    {
    public:
      SecretAgent(ai::Agent::Options *opts);
      ~SecretAgent();
      virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
      ai::Search::Graph * SearchFromCurrentCellToBase();
      ai::Search::Graph * SearchFromCurrentCellToUnvisitedCell();
      ai::Search::Graph * SearchFromAnotherCellToBase(double x, double y, double charge);
    protected:
    private:
      cwp::Scavenger::SecretAgentModel * model;
    };
  }
}

#endif /* _SECRETAGENT_H_ */
