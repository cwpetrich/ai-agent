#include "Dumbo2.h"

namespace ai
{
  namespace Sample
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

              if(gGlobalCounter%2==3) // don't let the pause get optimized away
                std::cout << "hi" << std::endl;
            }
        }
    }


    Dumbo2::Dumbo2()
    {
      SetName("Dumbo2");
      if(0) { Pause(); }
    }

    Dumbo2::~Dumbo2()
    {
    }

    ai::Agent::Action * Dumbo2::Program(const ai::Agent::Percept * percept)
    {
      ai::CCheckers::Action *action = new ai::CCheckers::Action;
      ai::CCheckers::MoveData move;

      std::string board_str = percept->GetAtom("BASIC_BOARD").GetValue();
      ai::CCheckers::BasicBoard board(board_str);

      int player = atoi(percept->GetAtom("PLAYER_NUMBER").GetValue().c_str());

      const std::vector<ai::CCheckers::MoveData> & moves = board.DetermineLegalMoves(player);

      //Pause();

      double       best_heuristic = -1e6;
      unsigned int best_move = 0;

      for(unsigned int i=0; i<moves.size(); i++)
        {
          ai::CCheckers::BasicBoard b(board);
          b.Move(player, moves[i], 0);
          double h = Heuristic(player, b);  //Note this is the agents function
          if(h>best_heuristic)
            {
              best_heuristic = h;
              best_move = i;
            }
          else if(h==best_heuristic && rand()%2==0)
            {
              best_heuristic = h;
              best_move = i;
            }
        } // for each possible move

      if(moves.size() > 0)
        {
          move = moves[best_move];
        }

      action->SetMove(move);
      action->SetCode(ai::CCheckers::Action::MOVE);

      return action;
    }

    double Dumbo2::Heuristic(int player, const ai::CCheckers::BasicBoard &board)
    {
      const ai::CCheckers::BoardData & b = board.GetBoard();

      double h = 0;
      int x,y;

      for(x=0; x<17; x++)
        {
          for(y=0; y<17; y++)
            {
              if(b.hole[x][y] == player)
                {
                  // grant one point for overall "forwardness"
                  if(player == 1)
                    h += y;
                  else
                    h += 16-y;

                }
            }
        }

      return h;
    }
  }
}
