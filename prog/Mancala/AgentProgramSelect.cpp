
#include <ai_agent.h>

#include "aiSampleManual.h"
#include "aiSampleRandom.h"
#include "aiSampleMaximum.h"
#include "aiSampleMinimax.h"
#include "aiSampleMinimaxRandom.h"
#include "aiSampleMinimaxEvaluation.h"
#include "aiSampleAlphaBeta.h"
#include "aiSampleAlphaBetaRandom.h"
#include "aiSampleAlphaBetaRandomEvaluation.h"

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
        case 'X':
          agent_program = new ai::Sample::MinimaxRandom(opts);
          break;
        case 'e':
          agent_program = new ai::Sample::MinimaxEvaluation(opts);
          break;
        case 'a':
          agent_program = new ai::Sample::AlphaBeta(opts);
          break;
        case 'A':
          agent_program = new ai::Sample::AlphaBetaRandom(opts);
          break;
        case 'E':
          agent_program = new ai::Sample::AlphaBetaRandomEvaluation(opts);
          break;

        default:
          std::cout << "Unknown agent program " << std::endl
                    << "M : Manual" << std::endl
                    << "r : Random" << std::endl
                    << "m : Maximum" << std::endl
                    << "x : Minimax" << std::endl
                    << "X : MinimaxRandom" << std::endl
                    << "e : MinimaxEvaluation" << std::endl
                    << "a : AlphaBeta" << std::endl
                    << "A : AlphaBetaRandom" << std::endl
                    << "E : AlphaBetaRandomEvaluation" << std::endl
      
		    << std::endl;
          exit(1);
          break;
        }
      return agent_program;
    }
  }
}
