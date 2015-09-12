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
      ai::TicTacToe::Action *action = new ai::TicTacToe::Action;
      
      std::string board_str = percept->GetAtom("BOARD").GetValue();
      ai::TicTacToe::Board board(board_str);
      
      int player = atoi(percept->GetAtom("PLAYER_NUMBER").GetValue().c_str());

      const std::vector<ai::TicTacToe::MoveData> & moves = board.DetermineLegalMoves(player);

      ai::TicTacToe::MoveData best_move;
      double best_evaluation = -1e6;
      int    best_i = -1;
      double evaluation;
      
      for(int i = 0; i < (int)moves.size(); i++)
        {
          /* prefer moves in center of board, in corners, in middle of edges */
          if(moves[i].pos_x == 1 && moves[i].pos_y == 1)
            {
              evaluation = 3;
            }
          else if((moves[i].pos_x == 0 || moves[i].pos_x == 2) &&
                  (moves[i].pos_y == 0 || moves[i].pos_y == 2))
            {
              evaluation = 2;
            }
          else
            {
              evaluation = 1;
            }
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
