#include "aiSampleManual.h"
#include <cstdio>

namespace ai
{
  namespace Sample
  {
    Manual::Manual(ai::Agent::Options *opts)
    {
      SetName("Manual");
    }
    
    Manual::~Manual()
    {
    }
    
    ai::Agent::Action * Manual::Program(const ai::Agent::Percept * percept)
    {
      ai::TicTacToe::Action *action = new ai::TicTacToe::Action;
      
      std::string board_str = percept->GetAtom("BOARD").GetValue();
      ai::TicTacToe::Board board(board_str);
      
      int player = atoi(percept->GetAtom("PLAYER_NUMBER").GetValue().c_str());

      const std::vector<ai::TicTacToe::MoveData> & moves = board.DetermineLegalMoves(player);

      if(moves.size() > 0)
        {
          std::cout << "Player: " << player << std::endl;
          std::cout << "Board: " << board_str << std::endl;
          
          bool done = false;
          while(!done)
            {
              std::cout << "Move? x,y ";
              
              int x, y, i;
              char buf[128];
              std::cin.getline(buf, 128);
              
              if(std::sscanf(buf, "%d,%d", &x, &y) == 2)
                {
                  for(i = 0; i < (int)moves.size(); i++)
                    {
                      if(moves[i].pos_x == x && moves[i].pos_y == y)
                        {
                          action->SetMove(moves[i]);
                          action->SetCode(ai::TicTacToe::Action::MOVE);
                          done = true;
                          break;
                        }
                    }
                }
            }
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
