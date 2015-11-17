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
      debug_file << "Charge: " << model->getCharge() << std::endl;
      debug_file << "isUndiscoveredDirections: " << model->isUndiscoveredDirections(model->getCurrX(), model->getCurrY()) << std::endl;

      ai::Scavenger::Action *action = new ai::Scavenger::Action;
      model->gatherData(percept);
      
      if (model->isUndiscoveredDirections(model->getCurrX(), model->getCurrY())) {
        debug_file << "else if (isUndiscoveredDirections(CurrX, CurrY))" << std::endl;
        action->SetCode(ai::Scavenger::Action::LOOK);
        ai::Scavenger::Location::Direction direction = model->getNextUndiscoveredDirection(model->getCurrX(), model->getCurrY());
        model->updateLookDirection(direction);
        action->SetDirection(direction);
        return action;
      }
      else if (model->chargeAgent()) {
        debug_file << "else if (chargeAgent)" << std::endl;
        action->SetCode(ai::Scavenger::Action::RECHARGE);
        return action;
      }
      if (model->actionQueueEmpty()) {
        debug_file << "else if (actionQueueEmpty)" << std::endl;
        cwp::Scavenger::CellData * closest_unvisited_cell = model->getClosestUnvisitedCell(model->getCurrX(), model->getCurrY());
        if (model->getCurrX() == 0.0 && model->getCurrY() == 0.0 && closest_unvisited_cell->getLocX() == 0.0 && closest_unvisited_cell->getLocY() == 0.0){
          debug_file << "If at base and closest unvisted cell is base" << std::endl;
          action->SetCode(ai::Scavenger::Action::QUIT);
        }
        else {
          debug_file << "not at base or closest cell isn't base" << std::endl;
          model->updateGoalLocation(closest_unvisited_cell->getLocX(), closest_unvisited_cell->getLocY(), closest_unvisited_cell->getLocZ());
          ai::Search::Graph *search = SearchFromXYToGoalOrBase(model->getCurrX(), model->getCurrY(), model->getCharge() - 5.0, false);
          std::list<ai::Search::Node *> * solution;
          cwp::Scavenger::State * cuc_state;
          if (search->Search()){
            debug_file << "Successful search to unvisited cell" << std::endl;
            solution = search->GetSolution().GetList();
            if (solution->size() == 1){
              cuc_state = dynamic_cast<cwp::Scavenger::State *>(solution->front()->GetState());
            }
            else {
              cuc_state = dynamic_cast<cwp::Scavenger::State *>(solution->back()->GetState());
            }
            search = SearchFromXYToGoalOrBase(cuc_state->getX(), cuc_state->getY(), cuc_state->getCharge() - 5.0, true);
            if (search->Search()){
              debug_file << "Successful search to base from unvisited cell" << std::endl;
              std::list<ai::Search::Node *>::const_iterator it;
              for(it = solution->begin(); it != solution->end(); it++) {
                if((*it)->GetAction()) {
                  model->addActionToGoal(dynamic_cast<cwp::Scavenger::Action *>((*it)->GetAction()));
                }
              }
              action->SetCode(model->getNextActionToGoal()->getAction());
            }
            else {
              debug_file << "Unsuccessful search to base from unvisited cell" << std::endl;
              search = SearchFromXYToGoalOrBase(model->getCurrX(), model->getCurrY(), model->getCharge(), true);
              if (search->Search()){
                debug_file << "Successful search to base form current cell" << std::endl;
                solution = search->GetSolution().GetList();
                std::list<ai::Search::Node *>::const_iterator it;
                for(it = solution->begin(); it != solution->end(); it++) {
                  if((*it)->GetAction()) {
                    model->addActionToGoal(dynamic_cast<cwp::Scavenger::Action *>((*it)->GetAction()));
                  }
                }
                action->SetCode(model->getNextActionToGoal()->getAction());
              }
              else {
                debug_file << "Unsuccessful search to base from current cell" << std::endl;
                search = SearchFromXYToGoalOrBase(model->getCurrX(), model->getCurrY(), model->getCharge(), false);
                action->SetCode(ai::Scavenger::Action::QUIT);
              }
            }
          }
          else {
            debug_file << "Unsuccessful search from current cell to unvisted cell" << std::endl;
            search = SearchFromXYToGoalOrBase(model->getCurrX(), model->getCurrY(), model->getCharge(), true);
            if (search->Search()){
              debug_file << "Successful search from current cell to base" << std::endl;
              solution = search->GetSolution().GetList();
              std::list<ai::Search::Node *>::const_iterator it;
              for(it = solution->begin(); it != solution->end(); it++) {
                if((*it)->GetAction()) {
                  model->addActionToGoal(dynamic_cast<cwp::Scavenger::Action *>((*it)->GetAction()));
                }
              }
              action->SetCode(model->getNextActionToGoal()->getAction());
            }
            else {
              debug_file << "Unsuccessful search from current cell to base" << std::endl;
              action->SetCode(ai::Scavenger::Action::QUIT);
            }
          }
        }
      }
      else {
        debug_file << "if (!actionQueueEmpty)" << std::endl;
        action->SetCode(model->getNextActionToGoal()->getAction());
      }
      debug_file << "action->GetCode(): " << action->GetCode() << std::endl;
      debug_file << "action->GetDirection(): " << action->GetDirection() << std::endl;
      return action;
    }

    ai::Search::Graph * SecretAgent::SearchFromXYToGoalOrBase(double x, double y, double charge, bool to_base) {
      cwp::Scavenger::State * initial_state = new cwp::Scavenger::State(x, y, charge);
      cwp::Scavenger::Problem * problem = new cwp::Scavenger::Problem(dynamic_cast<ai::Search::State *>(initial_state), model, to_base);
      ai::Search::Frontier * fringe;
      if (to_base){
        fringe = new ai::Search::AStarFrontier;
      } else {
        fringe = new ai::Search::UCFrontier;
      }
      ai::Search::Graph *search = new ai::Search::Graph(problem, fringe);
      return search;
    }
  }
}
