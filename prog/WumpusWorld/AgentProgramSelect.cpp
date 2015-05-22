
#include <ai_agent.h>

#include "Simpleton.h"
#include "Aristotle.h"

namespace ai
{
  namespace Agent
  {
    AgentProgram *AgentProgramSelect(Options *opts)
    {
      AgentProgram *agent_program = 0;
      switch(opts->GetArgChr("agent"))
        {
        case 'S':
          agent_program = new ai::Sample::Simpleton();
          break;
        case 'A':
          agent_program = new ai::Sample::Aristotle();
          break;

        default:
          std::cout << "Unknown agent program " << std::endl
                    << "S : Simpleton" << std::endl
                    << "A : Aristotle" << std::endl
      
		    << std::endl;
          exit(1);
          break;
        }
      return agent_program;
    }
  }
}
