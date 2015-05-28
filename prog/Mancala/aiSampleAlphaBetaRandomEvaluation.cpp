#include "aiSampleAlphaBetaRandomEvaluation.h"
#include <cstdio>

namespace ai
{
  namespace Sample
  {
    const double MY_INFINITY = 1.e10;

    AlphaBetaRandomEvaluation::AlphaBetaRandomEvaluation(ai::Agent::Options *opts)
      : max_depth(3),
        evaluation_method(TT_SCORE_DIFF)
    {
      if(opts->GetArgInt("user1") > 0)
        {
          this->max_depth = opts->GetArgInt("user1");
        }
      if(opts->GetArgInt("user2") > 0)
        {
          this->evaluation_method = opts->GetArgInt("user2");
        }
      {
        char buf[128];
        std::sprintf(buf, "AlphaBetaRandomEvaluation-%d-%d",
                     this->max_depth,
                     this->evaluation_method);
        SetName(buf);
      }

      std::cout << "Options:" << std::endl;
      std::cout << "-U num : maximum search depth (in ply)." << std::endl;
      std::cout << "-V num : evaluation method." << std::endl;
      std::cout << "       : my score    : " << TT_SCORE_ME << std::endl;
      std::cout << "       : other score : " << TT_SCORE_OTHER << std::endl;
      std::cout << "       : diff score  : " << TT_SCORE_DIFF << std::endl;
      std::cout << "       : h1   : " << TT_H_1 << std::endl;
      std::cout << "       : h2   : " << TT_H_2 << std::endl;
      std::cout << "       : h3   : " << TT_H_3 << std::endl;
      std::cout << "       : h4   : " << TT_H_4 << std::endl;
      std::cout << "       : h5   : " << TT_H_5 << std::endl;
      std::cout << "       : h6   : " << TT_H_6 << std::endl;
      std::cout << "       : hcombo1 : " << TT_H_COMBO_1 << std::endl;
      std::cout << "       : hcombo2 : " << TT_H_COMBO_2 << std::endl;
      std::cout << "       : hcombo3 : " << TT_H_COMBO_3 << std::endl;
    }

    AlphaBetaRandomEvaluation::~AlphaBetaRandomEvaluation()
    {
    }

    ai::Agent::Action * AlphaBetaRandomEvaluation::Program(const ai::Agent::Percept * percept)
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

    PlyData AlphaBetaRandomEvaluation::PickMove(const ai::Mancala::Board &board, int player)
    {
      PlyData ply_data = this->Max(board, player, 1, -MY_INFINITY, MY_INFINITY);
      return ply_data;
    }

    PlyData AlphaBetaRandomEvaluation::Max(const ai::Mancala::Board &board, int player, int depth, double alpha, double beta)
    {
      PlyData ply_data(-MY_INFINITY);
      if(board.HaveWinner() || depth > this->max_depth)
        {
          ply_data.utility = this->Evaluate(board, player);
          return ply_data;
        }

      ai::Mancala::Board my_board(board);
      const std::vector<ai::Mancala::MoveData> & moves = my_board.DetermineLegalMoves(player);
      std::vector<PlyData> best_plys;
      int i;
      for(i = 0; i < (int)moves.size(); i++)
        {
          ai::Mancala::Board tmp_board(board);
          tmp_board.Move(player, moves[i]);
          PlyData tmp_ply_data = this->Min(tmp_board, player, depth + 1, alpha, beta);
          if(tmp_ply_data.utility > ply_data.utility)
            {
              ply_data.utility = tmp_ply_data.utility;
              ply_data.move    = moves[i];
              ply_data.move_ok = true;

              best_plys.clear();
              best_plys.push_back(ply_data);

              if(ply_data.utility > beta)
                { /* parent node has a better child, so this node will not be chosen */
                  break;
                }
              if(ply_data.utility > alpha)
                {
                  alpha = ply_data.utility;
                }
            }
          else if(tmp_ply_data.utility == ply_data.utility)
            {
              PlyData t(-MY_INFINITY);
              t.utility = tmp_ply_data.utility;
              t.move    = moves[i];
              t.move_ok = true;
              best_plys.push_back(t);
            }
        }

      return best_plys[rand () % best_plys.size()];
    }

    PlyData AlphaBetaRandomEvaluation::Min(const ai::Mancala::Board &board, int player, int depth, double alpha, double beta)
    {
      PlyData ply_data(MY_INFINITY);
      if(board.HaveWinner() || depth > this->max_depth)
        {
          ply_data.utility = this->Evaluate(board, player);
          return ply_data;
        }

      int other_player = (player == 1) ? 2 : 1;
      ai::Mancala::Board my_board(board);
      const std::vector<ai::Mancala::MoveData> & moves = my_board.DetermineLegalMoves(other_player);
      std::vector<PlyData> best_plys;
      int i;
      for(i = 0; i < (int)moves.size(); i++)
        {
          ai::Mancala::Board tmp_board(board);
          tmp_board.Move(other_player, moves[i]);
          PlyData tmp_ply_data = this->Max(tmp_board, player, depth + 1, alpha, beta);
          if(tmp_ply_data.utility < ply_data.utility)
            {
              ply_data.utility = tmp_ply_data.utility;
              ply_data.move    = moves[i];
              ply_data.move_ok = true;

              best_plys.clear();
              best_plys.push_back(ply_data);

              if(ply_data.utility < alpha)
                { /* parent node has a better child, so this node will not be chosen */
                  break;
                }
              if(ply_data.utility < beta)
                {
                  beta = ply_data.utility;
                }
            }
          else if(tmp_ply_data.utility == ply_data.utility)
            {
              PlyData t(MY_INFINITY);
              t.utility = tmp_ply_data.utility;
              t.move    = moves[i];
              t.move_ok = true;
              best_plys.push_back(t);
            }

        }

      return best_plys[rand () % best_plys.size()];
    }

    /*
     * Choose evaluation method
     */
    double AlphaBetaRandomEvaluation::Evaluate(const ai::Mancala::Board &board, int player)
    {
      double v = 0.0;
      switch(this->evaluation_method)
        {
        case TT_SCORE_ME:
          v = this->EvaluateScoreMe(board, player);
          break;
        case TT_SCORE_OTHER:
          v = this->EvaluateScoreOther(board, player);
          break;
        case TT_SCORE_DIFF:
          /* fall through */
        default:
          v = this->EvaluateScoreDiff(board, player);
          break;
        case TT_H_1:
          v = this->EvaluateScoreDiff(board, player);
          break;
        case TT_H_2:
          v = this->hCloseToWinningMe(board, player);
          break;
        case TT_H_3:
          v = this->hCloseToWinningOther(board, player);
          break;
        case TT_H_4:
          v = this->hCloseToHome(board, player);
          break;
        case TT_H_5:
          v = this->hFarFromHome(board, player);
          break;
        case TT_H_6:
          v = this->hMiddleFromHome(board, player);
          break;
        case TT_H_COMBO_1:
          v = -1*this->hCloseToWinningMe(board, player) +
            this->hCloseToHome(board, player) +
            3*this->hFarFromHome(board, player) +
            2*this->hMiddleFromHome(board, player);
          break;
        case TT_H_COMBO_2:
          v = -2*this->hCloseToWinningMe(board, player) +
            1*this->hCloseToWinningOther(board, player) +
            1*this->hFarFromHome(board, player);
          break;
        case TT_H_COMBO_3:
          v = 1*this->EvaluateScoreDiff(board, player) +
            -1*this->hCloseToWinningMe(board, player) +
            1*this->hCloseToWinningOther(board, player);
          break;
        }
      return v;
    }
    double AlphaBetaRandomEvaluation::EvaluateScoreMe(const ai::Mancala::Board &board, int player)
    {
      return board.Score(player);
    }

    double AlphaBetaRandomEvaluation::EvaluateScoreOther(const ai::Mancala::Board &board, int player)
    {
      int other_player = (player == 1) ? 2 : 1;
      return board.Score(other_player);
    }

    double AlphaBetaRandomEvaluation::EvaluateScoreDiff(const ai::Mancala::Board &board, int player)
    {
      int other_player = (player == 1) ? 2 : 1;
      return board.Score(player) - board.Score(other_player);
    }

    double AlphaBetaRandomEvaluation::hCloseToWinningMe(const ai::Mancala::Board &board, int player)
    {
      double d = 25 - board.Score(player);
      if(d < 0) { d = 0; }
      return d;
    }
    double AlphaBetaRandomEvaluation::hCloseToWinningOther(const ai::Mancala::Board &board, int player)
    {
      int other_player = (player == 1) ? 2 : 1;
      double d = 25 - board.Score(other_player);
      if(d < 0) { d = 0; }
      return d;
    }
    double AlphaBetaRandomEvaluation::hCloseToHome(const ai::Mancala::Board &board, int player)
    {
      double v = 0;
      if(player == 1)
        {
          v = board.GetBoard().board[4] + board.GetBoard().board[5];
        }
      else
        {
          v = board.GetBoard().board[11] + board.GetBoard().board[12];
        }
      return v;
    }

    double AlphaBetaRandomEvaluation::hFarFromHome(const ai::Mancala::Board &board, int player)
    {
      double v = 0;
      if(player == 1)
        {
          v = board.GetBoard().board[0] + board.GetBoard().board[1];
        }
      else
        {
          v = board.GetBoard().board[7] + board.GetBoard().board[8];
        }
      return v;
    }
    double AlphaBetaRandomEvaluation::hMiddleFromHome(const ai::Mancala::Board &board, int player)
    {
      double v = 0;
      if(player == 1)
        {
          v = board.GetBoard().board[2] + board.GetBoard().board[3];
        }
      else
        {
          v = board.GetBoard().board[9] + board.GetBoard().board[10];
        }
      return v;
    }

  }
}
