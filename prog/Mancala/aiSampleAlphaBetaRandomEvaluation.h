#ifndef _AISAMPLEALPHABETARANDOMEVALUATION_H_
#define _AISAMPLEALPHABETARANDOMEVALUATION_H_
#include <ai_mancala.h>
#include "aiSampleMinimax.h"
namespace ai
{
  namespace Sample
  {
    enum
    {
      TT_SCORE_ME = 1,
      TT_SCORE_OTHER = 2,
      TT_SCORE_DIFF = 3,
      TT_H_1,
      TT_H_2,
      TT_H_3,
      TT_H_4,
      TT_H_5,
      TT_H_6,
      TT_H_COMBO_1,
      TT_H_COMBO_2,
      TT_H_COMBO_3,
    };
    
    class AlphaBetaRandomEvaluation : public ai::Agent::AgentProgram
    {
    public:
      AlphaBetaRandomEvaluation(ai::Agent::Options *opts);
      ~AlphaBetaRandomEvaluation();
      virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
    protected:
      PlyData PickMove(const ai::Mancala::Board &board, int player);
      PlyData Max(const ai::Mancala::Board &board, int player, int depth, double alpha, double beta);
      PlyData Min(const ai::Mancala::Board &board, int player, int depth, double alpha, double beta);

      double Evaluate(const ai::Mancala::Board &board, int player);
    private:
      double EvaluateScoreMe(const ai::Mancala::Board &board, int player);
      double EvaluateScoreOther(const ai::Mancala::Board &board, int player);
      double EvaluateScoreDiff(const ai::Mancala::Board &board, int player);

      double hCloseToWinningMe(const ai::Mancala::Board &board, int player);
      double hCloseToWinningOther(const ai::Mancala::Board &board, int player);
      double hCloseToHome(const ai::Mancala::Board &board, int player);
      double hFarFromHome(const ai::Mancala::Board &board, int player);
      double hMiddleFromHome(const ai::Mancala::Board &board, int player);
      
      int max_depth;
      int evaluation_method;
    };
  }
}
#endif /* _AISAMPLEALPHABETARANDOMEVALUATION_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
