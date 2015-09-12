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
      ai::Mancala::Action *action = new ai::Mancala::Action;
      
      std::string board_str = percept->GetAtom("BOARD").GetValue();
      ai::Mancala::Board board(board_str);
      
      int player = atoi(percept->GetAtom("PLAYER_NUMBER").GetValue().c_str());

      const std::vector<ai::Mancala::MoveData> & moves = board.DetermineLegalMoves(player);

      if(moves.size() > 0)
        {
          std::cout << "Player: " << player << std::endl;
          std::cout << "Board: " << board_str << std::endl;
          
          bool done = false;
          while(!done)
            {
              int i;
              for(i = 0; i < (int)moves.size(); i++)
                {
                  std::cout << i << " " << moves[i].ToString() << std::endl;
                }
              
              std::cout << "Move? i ";
              
              int j;
              char buf[128];
              std::cin.getline(buf, 128);
              
              if(std::sscanf(buf, "%d", &j) == 1)
                {
                  if(j >= 0 && j < (int)moves.size())
                    {
                      action->SetMove(moves[j]);
                      action->SetCode(ai::Mancala::Action::MOVE);
                      done = true;
                    }
                }
            }
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
