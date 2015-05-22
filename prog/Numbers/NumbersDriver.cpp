#include <ai_search.h>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cstdio>

#include "NumbersAction.h"
#include "NumbersProblem.h"
#include "NumbersState.h"


int main(int argc, char **argv)
{
  std::srand(static_cast<unsigned int>(std::time(0)));
  
  ai::Sample::Numbers::State *the_initial_state = new ai::Sample::Numbers::State();
  the_initial_state->AddNumber(11);
  the_initial_state->AddNumber(7);
  the_initial_state->AddNumber(2);
  the_initial_state->AddNumber(4);
  the_initial_state->AddNumber(5);

  ai::Sample::Numbers::Problem *the_problem = new ai::Sample::Numbers::Problem(the_initial_state);
  the_problem->SetGoal(17);

  ai::Search::Frontier    *the_frontier  = new ai::Search::DFFrontier();
  //ai::Search::Frontier    *the_frontier  = new ai::Search::BFFrontier();
  ai::Search::Algorithm *the_algorithm = new ai::Search::Tree(the_problem, the_frontier);

  if(the_algorithm->Search())
    {
      std::list<ai::Search::Node *> *solution = the_algorithm->GetSolution().GetList();
      std::list<ai::Search::Node *>::const_iterator it;

      double cost = 0;
      int depth   = 0;
      for(it = solution->begin(); it != solution->end(); it++)
        {
          if((*it)->GetAction())
            {
              std::cout << "Action: ";
              (*it)->GetAction()->Display();
            }
          if((*it)->GetState())
            {
              std::cout << "State: ";
              (*it)->GetState()->Display();
            }
          cost = (*it)->GetPathCost();
          depth = (*it)->GetDepth();
          std::cout << std::endl;
        }

      size_t nodes_generated = the_algorithm->GetNumberNodesGenerated();
      size_t nodes_stored    = the_algorithm->GetMaxNodesStored();
      std::cout << "Results: T " << cost << " " << depth << " "
                << nodes_generated << " " << nodes_stored << std::endl;
    }
  else
    {
      size_t nodes_generated = the_algorithm->GetNumberNodesGenerated();
      size_t nodes_stored    = the_algorithm->GetMaxNodesStored();
      double cost = -1;
      int depth = -1;
      std::cout << "Results: F " << cost << " " << depth << " "
                << nodes_generated << " " << nodes_stored << std::endl;
    }
  
  
  // destructor will cause the_problem, the_fringe, and the_initial_state to be deleted.
  delete the_algorithm;  
  
  return 0;
}
