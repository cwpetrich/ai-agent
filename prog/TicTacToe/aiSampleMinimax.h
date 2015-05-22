#ifndef _AISAMPLEMINIMAX_H_
#define _AISAMPLEMINIMAX_H_
#include <ai_tictactoe.h>
namespace ai
{
  namespace Sample
  {
    class PlyData
    {
    public:
      PlyData(double utility_in);
      bool move_ok;
      ai::TicTacToe::MoveData move;
      double utility;
    };
    
    class Minimax : public ai::Agent::AgentProgram
    {
    public:
      Minimax(ai::Agent::Options *opts);
      ~Minimax();
      virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
    protected:
      PlyData PickMove(const ai::TicTacToe::Board &board, int player);
      PlyData Max(const ai::TicTacToe::Board &board, int player);
      PlyData Min(const ai::TicTacToe::Board &board, int player);

      double Evaluate(const ai::TicTacToe::Board &board, int player);
    private:
    };
  }
}
#endif /* _AISAMPLEMINIMAX_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
