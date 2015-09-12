#include "aiSampleMaximum.h"

namespace ai
{
  namespace Sample
  {
    Maximum::Maximum(ai::Agent::Options *opts)
    {
      SetName("Maximum");
    }
    
    Maximum::~Maximum()
    {
    }
    
    ai::Agent::Action * Maximum::Program(const ai::Agent::Percept * percept)
    {
      ai::Mancala::Action *action = new ai::Mancala::Action;
      
      std::string board_str = percept->GetAtom("BOARD").GetValue();
      ai::Mancala::Board board(board_str);
      
      int player = atoi(percept->GetAtom("PLAYER_NUMBER").GetValue().c_str());

      const std::vector<ai::Mancala::MoveData> & moves = board.DetermineLegalMoves(player);

      ai::Mancala::MoveData best_move;
      double best_evaluation = -1e6;
      int    best_i = -1;
      double evaluation;
      
      for(int i = 0; i < (int)moves.size(); i++)
        {
          ai::Mancala::Board tmp_board(board);
          tmp_board.Move(player, moves[i]);
          evaluation = tmp_board.Score(player);
          if(evaluation > best_evaluation)
            {
              best_evaluation = evaluation;
              best_i    = i;
              best_move = moves[i];
            }
        } // for each possible move

      if(best_i >= 0)
        {
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
