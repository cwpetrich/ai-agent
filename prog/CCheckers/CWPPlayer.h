#ifndef _CWPPLAYER_H_
#define _CWPPLAYER_H_

#include <ai_ccheckers.h>

namespace cwp
{
  namespace Checkers
  {
    struct MinMaxData {
      ai::CCheckers::MoveData move;
      int value;
    };


    class CWPPlayer : public ai::Agent::AgentProgram
    {
    public:
      CWPPlayer();
      CWPPlayer(ai::Agent::Options *opts);
      ~CWPPlayer();
      virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
      MinMaxData Max(int player, ai::CCheckers::BasicBoard * board, int depth);
      MinMaxData Min(int player, ai::CCheckers::BasicBoard * board, int depth);
    protected:
    private:
    };
  }
}

#endif /* _CWPPLAYER_H_ */
