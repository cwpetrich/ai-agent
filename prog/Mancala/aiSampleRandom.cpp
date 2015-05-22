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
      ai::Mancala::Action *action = new ai::Mancala::Action;
      
      std::string board_str = percept->GetAtom("BOARD").GetValue();
      ai::Mancala::Board board(board_str);
      
      int player = atoi(percept->GetAtom("PLAYER_NUMBER").GetValue().c_str());

      const std::vector<ai::Mancala::MoveData> & moves = board.DetermineLegalMoves(player);

      if(moves.size() > 0)
        {
          int best_i = rand() % moves.size();
          ai::Mancala::MoveData best_move = moves[best_i];
          action->SetMove(best_move);
          action->SetCode(ai::Mancala::Action::MOVE);
        }
      else
        {
          /* no legal move */
          action->SetCode(ai::Mancala::Action::QUIT);
        }
      return action;
    }
  }
}
