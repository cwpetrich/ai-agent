
#include <ai_agent.h>

#include "aiSampleManual.h"
#include "aiSampleRandom.h"
#include "aiSampleMaximum.h"
#include "aiSampleMinimax.h"

namespace ai
{
  namespace Agent
  {
    AgentProgram *AgentProgramSelect(Options *opts)
    {
      AgentProgram *agent_program = 0;
      switch(opts->GetArgChr("agent"))
        {
        case 'M':
          agent_program = new ai::Sample::Manual(opts);
          break;
        case 'r':
          agent_program = new ai::Sample::Random(opts);
          break;
        case 'm':
          agent_program = new ai::Sample::Maximum(opts);
          break;
        case 'x':
          agent_program = new ai::Sample::Minimax(opts);
          break;

        default:
          std::cout << "Unknown agent program " << std::endl
                    << "M : Manual" << std::endl
                    << "r : Random" << std::endl
                    << "m : Maximum" << std::endl
                    << "x : Minimax" << std::endl
      
		    << std::endl;
          exit(1);
          break;
        }
      return agent_program;
    }
  }
}
