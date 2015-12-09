#include "CWPPlayer.h"
#include <iostream>
#include <fstream>

namespace cwp
{
  namespace Checkers
  {
    static int gGlobalCounter = 0;

    static void Pause()
    {
      //This is to just to slow down so we can do time tests
      for(int pause=0; pause<500; pause++)
      {
        for(int pause1=0; pause1<500000; pause1++)
        {
          gGlobalCounter++;

          if(gGlobalCounter%2==3)
          {// don't let the pause get optimized away
            std::cout << "hi" << std::endl;
          }
        }
      }
    }

    CWPPlayer::CWPPlayer(){}


    CWPPlayer::CWPPlayer(ai::Agent::Options *opts)
    {
      SetName("CWPPlayer");
      std::cout << "My first  option is " << opts->GetArg("user1")    << "." << std::endl;
      std::cout << "My second option is " << opts->GetArg("user2")    << "." << std::endl;
      std::cout << "My third  option is " << opts->GetArg("user3")    << "." << std::endl;
      std::cout << "My fourth option is " << opts->GetArgChr("user4") << "." << std::endl;
      std::cout << "My fifth  option is " << opts->GetArgFlt("user5") << "." << std::endl;
      std::cout << "My sixth  option is " << opts->GetArgInt("user6") << "." << std::endl;
      if(0) { Pause(); }
    }

    CWPPlayer::~CWPPlayer()
    {
    }

    ai::Agent::Action * CWPPlayer::Program(const ai::Agent::Percept * percept)
    {
      ai::CCheckers::Action *action = new ai::CCheckers::Action;
      ai::CCheckers::MoveData move;

      std::string board_str = percept->GetAtom("BASIC_BOARD").GetValue();
      ai::CCheckers::BasicBoard board(board_str);

      int player = atoi(percept->GetAtom("PLAYER_NUMBER").GetValue().c_str());

      MinMaxData move_and_value = Max(player, &board, 1);
      move = move_and_value.move;

      action->SetMove(move);
      action->SetCode(ai::CCheckers::Action::MOVE);

      return action;
    }

    MinMaxData CWPPlayer::Max(int player, ai::CCheckers::BasicBoard * board, int depth){
      std::ofstream debug_file;
      debug_file.open("checkers.txt", std::ofstream::app | std::ofstream::out);
      MinMaxData move_and_value;
      move_and_value.value = -999;
      if(board->HaveWinner() || depth > 1)
      {
        move_and_value.value = board->Forwardness(player);
        return move_and_value;
      }

      ai::CCheckers::BasicBoard my_board(*board);
      const std::vector<ai::CCheckers::MoveData> & moves = my_board.DetermineLegalMoves(player);
      int i;
      for(i = 0; i < (int)moves.size(); i++)
      {
        ai::CCheckers::BasicBoard tmp_board(my_board);
        tmp_board.Move(player, moves[i]);
        // if (tmp_board.Forwardness(player) > move_and_value.value){
        //   move_and_value.move = moves[i];
        //   move_and_value.value = tmp_board.Forwardness(player);
        // }
        MinMaxData tmp_best_move = this->Min(player, &tmp_board, depth + 1);
        if(tmp_best_move.value > move_and_value.value)
        {
          move_and_value.value = tmp_best_move.value;
          move_and_value.move = moves[i];
        }
      }
      return move_and_value;
    }

    MinMaxData CWPPlayer::Min(int player, ai::CCheckers::BasicBoard * board, int depth){
      std::ofstream debug_file;
      debug_file.open("checkers.txt", std::ofstream::app | std::ofstream::out);
      MinMaxData move_and_value;
      move_and_value.value = 999;
      if(board->HaveWinner() || depth > 1)
      {
        move_and_value.value = board->Forwardness(player);
        return move_and_value;
      }

      ai::CCheckers::BasicBoard my_board(*board);
      const std::vector<ai::CCheckers::MoveData> & moves = my_board.DetermineLegalMoves(player);
      int i;
      for(i = 0; i < (int)moves.size(); i++)
      {
        ai::CCheckers::BasicBoard tmp_board(my_board);
        tmp_board.Move(player, moves[i]);
        if (tmp_board.Forwardness(player) < move_and_value.value){
          move_and_value.move = moves[i];
          move_and_value.value = tmp_board.Forwardness(player);
        }
        // MinMaxData tmp_best_move = this->Max(player, tmp_board, depth + 1);
        // if(tmp_best_move.value < move_and_value.value)
        // {
        //   move_and_value.value = tmp_best_move.value;
        //   move_and_value.move = moves[i];
        // }
      }

      return move_and_value;
    }
  }
}
