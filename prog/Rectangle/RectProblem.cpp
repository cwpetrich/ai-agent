#include "RectProblem.h"
#include "RectState.h"
#include "RectAction.h"
#include <iostream>

namespace cgl
{
  RectProblem::RectProblem(ai::Search::State *initial_state_in)
    : ai::Search::Problem(initial_state_in),
      find_all(false)
  {
    board_width  = ((const RectState * const)initial_state_in)->GetWidth();
    board_height = ((const RectState * const)initial_state_in)->GetHeight();
  }

  RectProblem::~RectProblem()
  {
  }

  bool   RectProblem::GoalTest(const ai::Search::State * const state_in) const
  {
    const RectState * const rs1 = (const RectState * const)(state_in);
    unsigned int i;
    for(i = 0; i < rs1->pieces.size(); i++)
      {
        if(!rs1->pieces[i].placed)
          {
            return false;
          }
      }
    std::cout << "SOLUTION" << std::endl;
    rs1->Display();
    return !find_all; // return false to find ALL solutions
  }
  
  std::vector<ai::Search::Action *> RectProblem::Actions(const ai::Search::State * const state_in)
  {
    const RectState * const rs1 = dynamic_cast<const RectState * const>(state_in);
    std::vector<ai::Search::Action *> actions;
    unsigned int i;
    int r, f;
    int x, y;

    /* Consider all placements of the first unplaced piece */
    for(i = 0; i < rs1->pieces.size(); i++)
      {
        if(rs1->pieces[i].placed)
          {
            continue;
          }
        for(r = 0; r < rs1->pieces[i].max_rotation; r++)
          {
            for(f = 0; f < rs1->pieces[i].max_flip; f++)
              {
                for(x = 0; x < board_width; x++)
                  {
                    for(y = 0; y < board_height; y++)
                      {
                        // only use action if place will actually place
                        RectState  rstmp(*rs1);
                        if(rstmp.PlacePiece(i, r, f, x, y))
                          {
                            actions.push_back(new RectAction(i, r, f, x, y));
                          }
                      }
                  }
              }
          }
        break;
      }
    return actions;
  }
  
  ai::Search::State *RectProblem::Result(const ai::Search::State * const state_in, const ai::Search::Action * const action_in)
  {
    const RectState * const rs1 = dynamic_cast<const RectState * const>(state_in);
    const RectAction * const ra1 = dynamic_cast<const RectAction * const>(action_in);
    RectState  *rs2 = new RectState(*rs1);
    rs2->PlacePiece(ra1->GetPiece(), ra1->GetRotation(), ra1->GetFlip(), ra1->GetX(), ra1->GetY());
    return rs2;
  }
  

  double RectProblem::StepCost(const ai::Search::State  * const state1_in,
                               const ai::Search::Action * const action_in,
                               const ai::Search::State  * const state2_in) const
  {
    return 1;
  }
  
  double RectProblem::Heuristic(const ai::Search::State  * const state_in) const
  {
    return 0;
  }
  
  void RectProblem::SetFindAll()
  {
    find_all = true;
  }
  
}
