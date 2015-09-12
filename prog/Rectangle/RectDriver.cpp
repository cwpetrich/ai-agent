#include <iostream>
#include "RectProblem.h"
#include "RectState.h"

int main()
{
  int width  = 6;
  int height = 10;
  bool find_all = false;
  cgl::RectState   *state   = new cgl::RectState(width, height);
  cgl::RectProblem *problem = new cgl::RectProblem((ai::Search::State *)state);
  if(find_all)
    {
      problem->SetFindAll();
    }

  ai::Search::Frontier *fringe  = new ai::Search::DFFrontier;
  ai::Search::Tree *search = new ai::Search::Tree(problem, fringe);
  if(search->Search())
    {
      std::list<ai::Search::Node *> *solution = search->GetSolution().GetList();
      std::list<ai::Search::Node *>::const_iterator it;

      for(it = solution->begin(); it != solution->end(); it++)
        {
          if((*it)->GetAction())
            {
              (*it)->GetAction()->Display();
            }
          if((*it)->GetState())
            {
              (*it)->GetState()->Display();
            }
        }
      std::cout << "Path Cost: " << solution->back()->GetPathCost() << std::endl;

      std::cout << "Nodes generated: " << search->GetNumberNodesGenerated() << std::endl;
      std::cout << "Nodes stored:    " << search->GetMaxNodesStored() << std::endl;
      
      /*
        it = solution->begin();
        cgl::RectState *state_ptr = (cgl::RectState *)((*it)->GetState());
        state_ptr->Display();
        std::cout << std::endl;
      */
    }
  delete search;
  search = 0;
  return 0;
}
