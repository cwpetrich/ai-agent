
#include <ai_agent.h>

#include "Augustus.h"

namespace ai
{
  namespace Agent
  {
    AgentProgram *AgentProgramSelect(Options *opts)
    {
      AgentProgram *agent_program = 0;
      switch(opts->GetArgChr("agent"))
        {
        case 'A':
          agent_program = new ai::Sample::Augustus(opts);
          break;

        default:
          std::cout << "Unknown agent program " << std::endl
                    << "A : Augustus" << std::endl
      
		    << std::endl;
          exit(1);
          break;
        }
      return agent_program;
    }
  }
}
