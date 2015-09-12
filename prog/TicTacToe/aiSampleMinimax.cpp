#include "aiSampleMinimax.h"

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
    {
      SetName("Minimax");
    }
    
    Minimax::~Minimax()
    {
    }
    
    ai::Agent::Action * Minimax::Program(const ai::Agent::Percept * percept)
    {
      ai::TicTacToe::Action *action = new ai::TicTacToe::Action;
      
      std::string board_str = percept->GetAtom("BOARD").GetValue();
      ai::TicTacToe::Board board(board_str);
      
      int player = atoi(percept->GetAtom("PLAYER_NUMBER").GetValue().c_str());

      PlyData ply_data = this->PickMove(board, player);

      if(ply_data.move_ok)
        {
          action->SetCode(ai::TicTacToe::Action::MOVE);
          action->SetMove(ply_data.move);
        }
      else
        {
          /* no legal move, or bad choice */
          action->SetCode(ai::TicTacToe::Action::QUIT);
        }
      return action;
    }

    PlyData Minimax::PickMove(const ai::TicTacToe::Board &board, int player)
    {
      PlyData ply_data = this->Max(board, player);
      return ply_data;
    }

    PlyData Minimax::Max(const ai::TicTacToe::Board &board, int player)
    {
      PlyData ply_data(-1.e6);
      if(board.HaveWinner())
        {
          ply_data.utility = this->Evaluate(board, player);
          return ply_data;
        }

      ai::TicTacToe::Board my_board(board);
      const std::vector<ai::TicTacToe::MoveData> & moves = my_board.DetermineLegalMoves(player);
      int i;
      for(i = 0; i < (int)moves.size(); i++)
        {
          ai::TicTacToe::Board tmp_board(board);
          tmp_board.Move(player, moves[i]);
          PlyData tmp_ply_data = this->Min(tmp_board, player);
          if(tmp_ply_data.utility > ply_data.utility)
            {
              ply_data.utility = tmp_ply_data.utility;
              ply_data.move    = moves[i];
              ply_data.move_ok = true;
            }
        }
      
      return ply_data;
    }
    
    PlyData Minimax::Min(const ai::TicTacToe::Board &board, int player)
    {
      PlyData ply_data(1.e6);
      if(board.HaveWinner())
        {
          ply_data.utility = this->Evaluate(board, player);
          return ply_data;
        }
      
      int other_player = (player == 1) ? 2 : 1;
      ai::TicTacToe::Board my_board(board);
      const std::vector<ai::TicTacToe::MoveData> & moves = my_board.DetermineLegalMoves(other_player);
      int i;
      for(i = 0; i < (int)moves.size(); i++)
        {
          ai::TicTacToe::Board tmp_board(board);
          tmp_board.Move(other_player, moves[i]);
          PlyData tmp_ply_data = this->Max(tmp_board, player);
          if(tmp_ply_data.utility < ply_data.utility)
            {
              ply_data.utility = tmp_ply_data.utility;
              ply_data.move    = moves[i];
              ply_data.move_ok = true;
            }
        }
      
      return ply_data;
    }
    
    double Minimax::Evaluate(const ai::TicTacToe::Board &board, int player)
    {
      int winner = board.HaveWinner();
      int other_player = (player == 1) ? 2 : 1;
      if(winner == 3)
        { /* tie games are worth 0 */
          return 0.0;
        }
      else if(winner == player)
        { /* wins are worth 1 */
          return 1.0;
        }
      else if(winner == other_player)
        { /* wins are worth -1 */
          return -1.0;
        }
      else
        { /* Game not over, estimate the board's value.
           *
           * There are 8 possible wins.  Count how many
           * are still available for each player.
           */
          const ai::TicTacToe::BoardData & bd = board.GetBoard();
          int count[2] = { 0, 0 };
          int x, y;
          
          /* look at row wins */
          for(y = 0; y < 3; y++)
            {
              bool contains[2] = { false, false };
              for(x = 0; x < 3; x++)
                {
                  if(bd.grid[y][x] != 0)
                    {
                      contains[bd.grid[y][x]-1] = true;
                    }
                }
              if(!contains[1])
                {
                  count[0]++;
                }
              if(!contains[0])
                {
                  count[1]++;
                }
            }

          /* look at column wins */
          for(x = 0; x < 3; x++)
            {
              bool contains[2] = { false, false };
              for(y = 0; y < 3; y++)
                {
                  if(bd.grid[y][x] != 0)
                    {
                      contains[bd.grid[y][x]-1] = true;
                    }
                }
              if(!contains[1])
                {
                  count[0]++;
                }
              if(!contains[0])
                {
                  count[1]++;
                }
            }
          
          /* look at diagonal wins */
          {
            bool contains[2] = { false, false };
            for(x = 0; x < 3; x++)
              {
                if(bd.grid[x][x] != 0)
                  {
                    contains[bd.grid[x][x]-1] = true;
                  }
              }
            if(!contains[1])
              {
                count[0]++;
              }
            if(!contains[0])
              {
                count[1]++;
              }
          }
          
          /* look at diagonal wins */
          {
            bool contains[2] = { false, false };
            for(x = 0; x < 3; x++)
              {
                if(bd.grid[x][2-x] != 0)
                  {
                    contains[bd.grid[x][2-x]-1] = true;
                  }
              }
            if(!contains[1])
              {
                count[0]++;
              }
            if(!contains[0])
              {
                count[1]++;
              }
          }
          
          return count[player-1] / 8.0 - count[other_player-1] / 8.0;
        }
    }
    
  }
}
