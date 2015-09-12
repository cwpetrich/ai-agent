
#include <ai_agent.h>

#include "RandomPick.h"
#include "Dumbo1.h"
#include "Dumbo2.h"
#include "Smarty.h"
#include "Pikachu.h"

namespace ai
{
  namespace Agent
  {
    AgentProgram *AgentProgramSelect(Options *opts)
    {
      AgentProgram *agent_program = 0;
      switch(opts->GetArgChr("agent"))
        {
        case 'r':
          agent_program = new ai::Sample::RandomPick();
          break;
        case 'd':
          agent_program = new ai::Sample::Dumbo1();
          break;
        case 'D':
          agent_program = new ai::Sample::Dumbo2();
          break;
        case 's':
          agent_program = new ai::Sample::Smarty(opts);
          break;
        case 'p':
          agent_program = new cgl::Pikachu();
          break;

        default:
          std::cout << "Unknown agent program " << std::endl
                    << "r : RandomPick" << std::endl
                    << "d : Dumbo1" << std::endl
                    << "D : Dumbo2" << std::endl
                    << "s : Smarty" << std::endl
                    << "p : Pikachu" << std::endl
      
		    << std::endl;
          exit(1);
          break;
        }
      return agent_program;
    }
  }
}
