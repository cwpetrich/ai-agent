#include <ai_search.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "MazeAction.h"
#include "MazeProblem.h"
#include "MazeState.h"
#include "MazeModel.h"

int main(int argc, char **argv)
{
  const char *maze_file = "maze1.txt";
  if(argc > 1)
    {
      maze_file = argv[1];
    }
  
  ai::Sample::Maze::Model *the_model = new ai::Sample::Maze::Model(maze_file);
  the_model->Display();
  ai::Sample::Maze::State *the_initial_state = new ai::Sample::Maze::State();
  the_initial_state->SetX(the_model->GetStartX());
  the_initial_state->SetY(the_model->GetStartY());
  
  ai::Sample::Maze::State *the_goal_state = new ai::Sample::Maze::State();
  the_goal_state->SetX(the_model->GetGoalX());
  the_goal_state->SetY(the_model->GetGoalY());
  
  ai::Sample::Maze::Problem *the_problem = new ai::Sample::Maze::Problem(the_initial_state, the_model);
  the_problem->SetGoal(the_goal_state);

  //ai::Search::Frontier  *the_frontier  = new ai::Search::DFFrontier();
  ai::Search::Frontier  *the_frontier  = new ai::Search::BFFrontier();
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

              const ai::Sample::Maze::State * const state = dynamic_cast<const ai::Sample::Maze::State * const>((*it)->GetState());
              the_model->SetPathCell(state->GetX(), state->GetY());
            }
          cost = (*it)->GetPathCost();
          depth = (*it)->GetDepth();
          std::cout << std::endl;
          
        }

      size_t nodes_generated = the_algorithm->GetNumberNodesGenerated();
      size_t nodes_stored    = the_algorithm->GetMaxNodesStored();
      std::cout << "Results: T " << cost << " " << depth << " "
                << nodes_generated << " " << nodes_stored << std::endl;
      
      the_model->Display();
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
