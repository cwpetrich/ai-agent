#include "aiSampleRandom.h"
#include <ctime>
#include <cstdlib>

namespace ai
{
  namespace Sample
  {
    Random::Random(ai::Agent::Options *opts)
    {
      SetName("Random");
    }
    
    Random::~Random()
    {
    }
    
    ai::Agent::Action * Random::Program(const ai::Agent::Percept * percept)
    {
      ai::TicTacToe::Action *action = new ai::TicTacToe::Action;
      
      std::string board_str = percept->GetAtom("BOARD").GetValue();
      ai::TicTacToe::Board board(board_str);
      
      int player = atoi(percept->GetAtom("PLAYER_NUMBER").GetValue().c_str());

      const std::vector<ai::TicTacToe::MoveData> & moves = board.DetermineLegalMoves(player);

      if(moves.size() > 0)
        {
          int best_i = rand() % moves.size();
          ai::TicTacToe::MoveData best_move = moves[best_i];
          action->SetMove(best_move);
          action->SetCode(ai::TicTacToe::Action::MOVE);
        }
      else
        {
          /* no legal move */
          action->SetCode(ai::TicTacToe::Action::QUIT);
        }
      return action;
    }
  }
}
