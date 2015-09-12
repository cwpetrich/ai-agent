#include "aiSampleMinimax.h"
#include <cstdio>

namespace ai
{
  namespace Sample
  {
    PlyData::PlyData(double utility_in)
      : move_ok(false),
        utility(utility_in)
    {
    }
    
    Minimax::Minimax(ai::Agent::Options *opts)
      : max_depth(3)
    {
      if(opts->GetArgInt("user1") > 0)
        {
          this->max_depth = opts->GetArgInt("user1");
        }
      {
        char buf[128];
        std::sprintf(buf, "Minimax-%d", this->max_depth);
        SetName(buf);
      }
      
      std::cout << "Options:" << std::endl;
      std::cout << "-U num : maximum search depth (in ply)." << std::endl;
    }
    
    Minimax::~Minimax()
    {
    }
    
    ai::Agent::Action * Minimax::Program(const ai::Agent::Percept * percept)
    {
      ai::Mancala::Action *action = new ai::Mancala::Action;
      
      std::string board_str = percept->GetAtom("BOARD").GetValue();
      ai::Mancala::Board board(board_str);
      
      int player = atoi(percept->GetAtom("PLAYER_NUMBER").GetValue().c_str());

      PlyData ply_data = this->PickMove(board, player);

      if(ply_data.move_ok)
        {
          action->SetCode(ai::Mancala::Action::MOVE);
          action->SetMove(ply_data.move);
        }
      else
        {
          /* no legal move, or bad choice */
          action->SetCode(ai::Mancala::Action::QUIT);
        }
      return action;
    }

    PlyData Minimax::PickMove(const ai::Mancala::Board &board, int player)
    {
      PlyData ply_data = this->Max(board, player, 1);
      return ply_data;
    }

    PlyData Minimax::Max(const ai::Mancala::Board &board, int player, int depth)
    {
      PlyData ply_data(-1.e6);
      if(board.HaveWinner() || depth > this->max_depth)
        {
          ply_data.utility = this->Evaluate(board, player);
          return ply_data;
        }

      ai::Mancala::Board my_board(board);
      const std::vector<ai::Mancala::MoveData> & moves = my_board.DetermineLegalMoves(player);
      int i;
      for(i = 0; i < (int)moves.size(); i++)
        {
          ai::Mancala::Board tmp_board(board);
          tmp_board.Move(player, moves[i]);
          PlyData tmp_ply_data = this->Min(tmp_board, player, depth + 1);
          if(tmp_ply_data.utility > ply_data.utility)
            {
              ply_data.utility = tmp_ply_data.utility;
              ply_data.move    = moves[i];
              ply_data.move_ok = true;
            }
        }
      
      return ply_data;
    }
    
    PlyData Minimax::Min(const ai::Mancala::Board &board, int player, int depth)
    {
      PlyData ply_data(1.e6);
      if(board.HaveWinner() || depth > this->max_depth)
        {
          ply_data.utility = this->Evaluate(board, player);
          return ply_data;
        }
      
      int other_player = (player == 1) ? 2 : 1;
      ai::Mancala::Board my_board(board);
      const std::vector<ai::Mancala::MoveData> & moves = my_board.DetermineLegalMoves(other_player);
      int i;
      for(i = 0; i < (int)moves.size(); i++)
        {
          ai::Mancala::Board tmp_board(board);
          tmp_board.Move(other_player, moves[i]);
          PlyData tmp_ply_data = this->Max(tmp_board, player, depth + 1);
          if(tmp_ply_data.utility < ply_data.utility)
            {
              ply_data.utility = tmp_ply_data.utility;
              ply_data.move    = moves[i];
              ply_data.move_ok = true;
            }
        }
      
      return ply_data;
    }

    /*
     * Simple Evaluation, my score - opponent's score
     */
    double Minimax::Evaluate(const ai::Mancala::Board &board, int player)
    {
      int other_player = (player == 1) ? 2 : 1;
      return board.Score(player) - board.Score(other_player);
    }
  }
}
