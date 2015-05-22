#ifndef _AISAMPLEMINIMAX_H_
#define _AISAMPLEMINIMAX_H_
#include <ai_mancala.h>
namespace ai
{
  namespace Sample
  {
    class PlyData
    {
    public:
      PlyData(double utility_in);
      bool move_ok;
      ai::Mancala::MoveData move;
      double utility;
    };
    
    class Minimax : public ai::Agent::AgentProgram
    {
    public:
      Minimax(ai::Agent::Options *opts);
      ~Minimax();
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
#endif /* _AISAMPLEMINIMAX_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
