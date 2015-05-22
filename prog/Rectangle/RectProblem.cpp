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
  bool   RectProblem::FindSuccessors(const ai::Search::State * const state_in,
				     std::vector<ai::Search::ActionStatePair>
				     &results_out) const
  {
    const RectState * const rs1 = (const RectState * const)(state_in);
    unsigned int i;
    int r, f;
    int x, y;
		
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
			RectState  *rs2 = new RectState(*rs1);
			RectAction *ra2 = new RectAction(i, r, f, x, y);
			if(rs2->PlacePiece(i, r, f, x, y))
			  {
			    ai::Search::ActionStatePair asp((ai::Search::State *)rs2,
							    (ai::Search::Action *)ra2);
			    results_out.push_back(asp);
			  }
			else
			  {
			    delete rs2;
			    delete ra2;
			  }
		      }
		  }
	      }
	  }
	break;
      }
    return false;
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
