#include "cwpSecretAgent.h"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>

namespace cwp
{
  namespace Scavanger
  {
    /* For more on options look at the files:
     * ai-lib/include/Agent/Options.h
     * ai-lib/src/Agent/Options.cpp
     * al-lib/src/Agent/AgentDriver.cpp
     */
    /*
     * Run with:
     * ./RunProg ./SA_Test -a s -U 1
     */
     SecretAgent::SecretAgent(ai::Agent::Options *opts)
     {
      SetName("SecretAgent");
      std::cout << "The value of the -U option is: " << opts->GetArgInt("user1") << std::endl;
      this->model = new cwp::Scavenger::SecretAgentModel;
    }

    SecretAgent::~SecretAgent()
    {
      delete(model);
    }

    ai::Agent::Action * SecretAgent::Program(const ai::Agent::Percept * percept)
    {

      std::ofstream debug_file;
      debug_file.open("debug.txt", std::ofstream::out | std::ofstream::app);
      debug_file << "BEGINNING OF PROGRAM" << std::endl;

      ai::Scavenger::Action *action = new ai::Scavenger::Action;
      model->gatherData(percept);
      debug_file << "Charge: " << model->getCharge() << std::endl;
      debug_file << "isUndiscoveredDirections: " << model->isUndiscoveredDirections(model->getCurrX(), model->getCurrY()) << std::endl;
      if (model->isUndiscoveredDirections(model->getCurrX(), model->getCurrY()))
      {
        action->SetCode(ai::Scavenger::Action::LOOK);
        ai::Scavenger::Location::Direction direction = model->getNextUndiscoveredDirection(model->getCurrX(), model->getCurrY());
        model->updateLookDirection(direction);
        action->SetDirection(direction);
      }
      else
      {
        if (model->actionQueueEmpty())
        {
          cwp::Scavenger::CellData * closest_unvisited_cell = model->getClosestUnvisitedCell(model->getCurrX(), model->getCurrY());
          model->updateGoalLocation(closest_unvisited_cell->getLocX(), closest_unvisited_cell->getLocY(), closest_unvisited_cell->getLocZ());
          ai::Search::Graph *search = SearchFromXYToGoalOrBase(model->getCurrX(), model->getCurrY(), model->getCharge(), false);
          std::list<ai::Search::Node *> * solution;
          cwp::Scavenger::State * state_of_closest_unvisited_cell;
          if (search->Search()) 
          {
            solution = search->GetSolution().GetList();
            state_of_closest_unvisited_cell = dynamic_cast<cwp::Scavenger::State *>(solution->back()->GetState());

            search = SearchFromXYToGoalOrBase(state_of_closest_unvisited_cell->getX(), state_of_closest_unvisited_cell->getY(), state_of_closest_unvisited_cell->getCharge(), true);
            if (search->Search())
            {
              std::list<ai::Search::Node *>::const_iterator it;
              for(it = solution->begin(); it != solution->end(); it++)
              {
                if((*it)->GetAction())
                {
                  model->addActionToGoal(dynamic_cast<cwp::Scavenger::Action *>((*it)->GetAction()));
                }
              }
              action->SetCode(model->getNextActionToGoal()->getAction());
            }
            else
            {
              search = SearchFromXYToGoalOrBase(model->getCurrX(), model->getCurrY(), model->getCharge(), true);
              if (search->Search())
              {
                solution = search->GetSolution().GetList();
                std::list<ai::Search::Node *>::const_iterator it;
                for(it = solution->begin(); it != solution->end(); it++)
                {
                  if((*it)->GetAction())
                  {
                    model->addActionToGoal(dynamic_cast<cwp::Scavenger::Action *>((*it)->GetAction()));
                  }
                }
                action->SetCode(model->getNextActionToGoal()->getAction());
              }
              else
              {
                action->SetCode(ai::Scavenger::Action::QUIT);
              }
            }
          }
          else
          {
            action->SetCode(ai::Scavenger::Action::QUIT);
          }
        }
        else
        {
          action->SetCode(model->getNextActionToGoal()->getAction());
        }
      }
      debug_file << "action->GetCode(): " << action->GetCode() << std::endl;
      debug_file << "action->GetDirection(): " << action->GetDirection() << std::endl;
      return action;
    }

    ai::Search::Graph * SecretAgent::SearchFromXYToGoalOrBase(double x, double y,double charge, bool to_base) {
      cwp::Scavenger::State * initial_state = new cwp::Scavenger::State(x, y, charge);
      cwp::Scavenger::Problem * problem = new cwp::Scavenger::Problem(dynamic_cast<ai::Search::State *>(initial_state), model, to_base);
      ai::Search::Frontier *fringe  = new ai::Search::AStarFrontier;
      ai::Search::Graph *search = new ai::Search::Graph(problem, fringe);
      return search;
    }
  }
}
