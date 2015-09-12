#include "Dumbo1.h"

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


    Dumbo1::Dumbo1()
    {
      SetName("Dumbo1");
      if(0) { Pause(); }
    }

    Dumbo1::~Dumbo1()
    {
    }

    ai::Agent::Action * Dumbo1::Program(const ai::Agent::Percept * percept)
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
      int          best_from = (player==1) ? 17 : -1;

      for(unsigned int i=0; i<moves.size(); i++)
        {
          int progress = moves[i].to_y - moves[i].from_y;
          if(player==2)
            progress = - progress;

          bool chooseThisMove = false;
          // pick the move resulting in the most overall forwardness
          if(progress > best_heuristic)
            chooseThisMove = true;
          // On a forwardness tie, pick the marble the farthest back.
          else if(progress == best_heuristic && player==1 && moves[i].from_y < best_from)
            chooseThisMove = true;
          // On a farwardness tie, and a farthest back tie, randomly pick.
          else if(progress == best_heuristic && player==1  && moves[i].from_y <= best_from && rand()%2==0)
            chooseThisMove = true;
          // Repeat test from player 2's perspective
          else if(progress == best_heuristic && player==2 && moves[i].from_y > best_from)
            chooseThisMove = true;
          // Repeat test from player 2's perspective
          else if(progress == best_heuristic && player==2 && moves[i].from_y >= best_from && rand()%2==0)
            chooseThisMove = true;

          if(chooseThisMove)
            {
              best_heuristic = progress;
              best_move = i;
              best_from = moves[i].from_y;
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
  }
}
