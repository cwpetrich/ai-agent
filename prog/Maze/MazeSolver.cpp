#include <ai_search.h>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <unistd.h>

#include "MazeAction.h"
#include "MazeProblem.h"
#include "MazeState.h"


#define TREE 0
#define GRAPH 1
#define ALL_ALGORITHMS 99

#define BFS 0
#define UCS 1
#define DFS 2
#define DLS 3
#define IDS 4
#define ALL_FRONTIERS 99

bool DoSearch(ai::Search::Algorithm *the_algorithm, std::string &prefix, bool show_solution,
              ai::Sample::Maze::Model *the_model)
{
  bool rval;
  
  if(the_algorithm->Search())
    {
      std::list<ai::Search::Node *> *solution = the_algorithm->GetSolution().GetList();
      std::list<ai::Search::Node *>::const_iterator it;

      double cost = 0;
      int depth   = 0;
      for(it = solution->begin(); it != solution->end(); it++)
        {
          cost = (*it)->GetPathCost();
          depth = (*it)->GetDepth();
          
          if((*it)->GetState())
            {
              const ai::Sample::Maze::State * const state =
                dynamic_cast<const ai::Sample::Maze::State * const>((*it)->GetState());
              the_model->SetPathCell(state->GetX(), state->GetY());
            }
        }

      if(show_solution)
        {
          the_algorithm->GetSolution().Display();
          the_model->Display();
        }
      size_t nodes_generated = the_algorithm->GetNumberNodesGenerated();
      size_t nodes_stored    = the_algorithm->GetMaxNodesStored();
      std::cout << prefix << "Results: T " << cost << " " << depth << " " << nodes_generated << " " << nodes_stored << std::endl;
      
      rval = true;
    }
  else
    {
      size_t nodes_generated = the_algorithm->GetNumberNodesGenerated();
      size_t nodes_stored    = the_algorithm->GetMaxNodesStored();
      double cost = -1;
      int depth = -1;
      std::cout << prefix << "Results: F " << cost << " " << depth << " " << nodes_generated << " " << nodes_stored << std::endl;
      rval = false;
    }
  return rval;
}

#define R_ERROR 1
#define R_DONE 2
#define R_BREAK 3
int BuildSearchAux(ai::Search::Problem *the_problem,
                   int the_frontier_type, int the_search_type, int max_depth,
                   int generation_limit, int store_limit,
                   bool show_solution,
                   ai::Sample::Maze::Model *the_model)
{
  ai::Search::Frontier *the_frontier = 0;
  ai::Search::Algorithm *the_algorithm = 0;
  
  std::string id = "";
  char dstring[128];
  std::sprintf(dstring, "%d", max_depth);
		
  switch(the_frontier_type)
    {
    case BFS:  the_frontier = new ai::Search::BFFrontier(); id += "BFS"; break;
    case UCS:  the_frontier = new ai::Search::UCFrontier(); id += "UCS"; break;
    case DFS:  the_frontier = new ai::Search::DFFrontier(); id += "DFS"; break;
    case DLS:  the_frontier = new ai::Search::DLFrontier(max_depth); id += "DLS:"; id += dstring; break;
    case IDS:  the_frontier = new ai::Search::DLFrontier(max_depth); id += "IDS:"; id += dstring;  break;
    default:
      std::cerr << "Invalid frontier type: " << the_frontier_type << std::endl;
      return R_ERROR;
      break;
    }
		
  switch(the_search_type)
    {
    case TREE:   the_algorithm = new ai::Search::Tree(the_problem, the_frontier);  id += "-TREE";  break;
    case GRAPH:  the_algorithm = new ai::Search::Graph(the_problem, the_frontier); id += "-GRAPH"; break;
    default:
      std::cerr << "Invalid search type: " << the_search_type << std::endl;
      return R_ERROR;
      break;
    }
  the_algorithm->SetGenerationLimit(generation_limit);
  the_algorithm->SetStoreLimit(store_limit);
  id += ": ";
  bool done = DoSearch(the_algorithm, id, show_solution, the_model);
  delete the_algorithm;  // destructor will cause the_problem, the_fringe, and tmp_initial_state to be deleted.
  if(done) { return R_BREAK; }
  return R_DONE;
}

template <class StateType, class ProblemType, class ModelType>
bool BuildSearch(int search_type, int frontier_type, int max_depth, int ids_max,
                 StateType *initial_state, int generation_limit, int store_limit,
                 bool show_solution,
                 StateType *goal_state, ModelType *model)
{
  int start_search = TREE;
  int end_search   = GRAPH;
  if(search_type != ALL_ALGORITHMS)
    {
      start_search = search_type;
      end_search = search_type;
    }
  int the_search_type;
  for(the_search_type = start_search; the_search_type <= end_search; the_search_type ++)
    {
      int start_frontier = BFS;
      int stop_frontier = IDS;
      if(frontier_type != ALL_FRONTIERS)
        {
          start_frontier = frontier_type;
          stop_frontier = frontier_type;
        }
      int the_frontier_type;
      for(the_frontier_type = start_frontier;
          the_frontier_type <= stop_frontier;
          the_frontier_type ++)
        {
  
          int i;
          int max_iterations = 1;
	  
          if(the_frontier_type == IDS) { max_iterations = ids_max; }
          for(i = 0; i < max_iterations; i++)
            {
              int the_max_depth = max_depth;
              if(the_frontier_type == IDS) { the_max_depth = i+1; }
	      
              StateType *tmp_initial_state = new StateType(*initial_state);
              StateType *tmp_goal_state    = new StateType(*goal_state);
              ModelType *tmp_model         = new ModelType(*model);
              ProblemType *the_problem = new ProblemType(tmp_initial_state, tmp_model);
              the_problem->SetGoal(tmp_goal_state);
              
              int done = BuildSearchAux(the_problem,
                                        the_frontier_type, the_search_type, the_max_depth,
                                        generation_limit, store_limit,
                                        show_solution, tmp_model);

              if(done == R_ERROR)
                {
                  return false;
                }
              else if(done == R_BREAK)
                {
                  break;
                }
            }
        }
    }
  return true;
}

int main(int argc, char **argv)
{
  int c;
  int show_help     = 0;
  int search_type   = TREE;
  int frontier_type = BFS;
  int max_depth     = 9; // number of actions
  int ids_max       = 9; // number of actions
  int generation_limit = 0;
  int store_limit = 0;
  bool show_solution = false;
  const char *maze_file = "maze1.txt";
  
  while((c = getopt(argc, argv,"f:g:l:L:m:St:F:h")) != EOF)
    {
      switch(c)
        {
        case 'f': frontier_type    = atoi(optarg); break;
        case 'g': generation_limit = atoi(optarg); break;
        case 'l': max_depth        = atoi(optarg); break;
        case 'L': ids_max          = atoi(optarg); break;
        case 'm': store_limit      = atoi(optarg); break;

        case 'F': maze_file        = optarg;       break;
        case 'S': show_solution    = true;         break;
        case 't': search_type      = atoi(optarg); break;
          
        default:
          printf("Unknown option: %c\n", c);
        case 'h': show_help   = 1; break;
        }
    }

  if(search_type != TREE && search_type != GRAPH && search_type != ALL_ALGORITHMS)
    {
      std::cout << "Invalid search type" << search_type << std::endl;
      show_help ++;
    }
  if((frontier_type < BFS || frontier_type > IDS) && (frontier_type != ALL_FRONTIERS))
    {
      std::cout << "Invalid frontier type" << frontier_type << std::endl;
      show_help ++;
    }
  
  if(show_help)
    {
      std::cout << "-f num   : frontier_type" << std::endl;
      std::cout << "         : 0 - BFS, 1 - UCS, 2 - DFS, 3 - DLS, 4 - IDS, 99 - ALL" << std::endl;
      std::cout << "-g num   : generation_limit" << std::endl;
      std::cout << "-m num   : store_limit" << std::endl;
      std::cout << "-l num   : max depth for depth limited search" << std::endl;
      std::cout << "-L num   : max depth for iterative deepening search" << std::endl;
      std::cout << "-S       : show solution" << std::endl;
      std::cout << "-t num   : search type "<<TREE<<" == tree, "<<GRAPH<<" == graph search" << std::endl;
      std::cout << "         : 0 - Tree, 1 - Graph, 99 ALL" << std::endl;
      std::cout << "-F file  : maze file" << std::endl;
      std::cout << "-h       : show this message" << std::endl;
      return 0;
    }
  
  ai::Sample::Maze::Model *the_model = new ai::Sample::Maze::Model(maze_file);
  ai::Sample::Maze::State *the_initial_state = new ai::Sample::Maze::State();
  the_initial_state->SetX(the_model->GetStartX());
  the_initial_state->SetY(the_model->GetStartY());
  
  ai::Sample::Maze::State *the_goal_state = new ai::Sample::Maze::State();
  the_goal_state->SetX(the_model->GetGoalX());
  the_goal_state->SetY(the_model->GetGoalY());
  
  BuildSearch<ai::Sample::Maze::State,
              ai::Sample::Maze::Problem,
              ai::Sample::Maze::Model>(search_type, frontier_type, max_depth, ids_max,
                                       the_initial_state, generation_limit, store_limit,
                                       show_solution,
                                       the_goal_state, the_model);
  
  delete the_initial_state;
  delete the_goal_state;
  delete the_model;

  return 0;
}
