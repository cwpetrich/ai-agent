#ifndef _DUMBO2_H_
#define _DUMBO2_H_

#include <ai_ccheckers.h>

namespace ai
{
  namespace Sample
  {
    class Dumbo2 : public ai::Agent::AgentProgram
    {
    public:
      Dumbo2();
      ~Dumbo2();
      virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
      
      double Heuristic(int player, const ai::CCheckers::BasicBoard &board);
    protected:
    private:
    };
  }
}

#endif /* _DUMBO2_H_ */
