#ifndef _AISAMPLEALPHABETARANDOM_H_
#define _AISAMPLEALPHABETARANDOM_H_
#include <ai_mancala.h>
#include "aiSampleMinimax.h"
namespace ai
{
  namespace Sample
  {
    class AlphaBetaRandom : public ai::Agent::AgentProgram
    {
    public:
      AlphaBetaRandom(ai::Agent::Options *opts);
      ~AlphaBetaRandom();
      virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
    protected:
      PlyData PickMove(const ai::Mancala::Board &board, int player);
      PlyData Max(const ai::Mancala::Board &board, int player, int depth, double alpha, double beta);
      PlyData Min(const ai::Mancala::Board &board, int player, int depth, double alpha, double beta);

      double Evaluate(const ai::Mancala::Board &board, int player);
    private:
      int max_depth;
    };
  }
}
#endif /* _AISAMPLEALPHABETARANDOM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
