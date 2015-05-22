#ifndef _AISAMPLEMINIMAXRANDOM_H_
#define _AISAMPLEMINIMAXRANDOM_H_
#include <ai_mancala.h>
#include "aiSampleMinimax.h"
namespace ai
{
  namespace Sample
  {
    class MinimaxRandom : public ai::Agent::AgentProgram
    {
    public:
      MinimaxRandom(ai::Agent::Options *opts);
      ~MinimaxRandom();
      virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
    protected:
      PlyData PickMove(const ai::Mancala::Board &board, int player);
      PlyData Max(const ai::Mancala::Board &board, int player, int depth);
      PlyData Min(const ai::Mancala::Board &board, int player, int depth);

      double Evaluate(const ai::Mancala::Board &board, int player);
    private:
      int max_depth;
    };
  }
}
#endif /* _AISAMPLEMINIMAXRANDOM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
