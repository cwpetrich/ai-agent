#ifndef _AISAMPLEMINIMAXEVALUATION_H_
#define _AISAMPLEMINIMAXEVALUATION_H_
#include <ai_mancala.h>
#include "aiSampleMinimax.h"
namespace ai
{
  namespace Sample
  {
    enum
    {
      T_SCORE_ME = 1,
      T_SCORE_OTHER = 2,
      T_SCORE_DIFF = 3,
      T_H_1,
      T_H_2,
      T_H_3,
      T_H_4,
      T_H_5,
      T_H_6,
      T_H_COMBO_1,
      T_H_COMBO_2,
      T_H_COMBO_3,
    };
    
    class MinimaxEvaluation : public ai::Agent::AgentProgram
    {
    public:
      MinimaxEvaluation(ai::Agent::Options *opts);
      ~MinimaxEvaluation();
      virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
    protected:
      PlyData PickMove(const ai::Mancala::Board &board, int player);
      PlyData Max(const ai::Mancala::Board &board, int player, int depth);
      PlyData Min(const ai::Mancala::Board &board, int player, int depth);

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
#endif /* _AISAMPLEMINIMAXEVALUATION_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
