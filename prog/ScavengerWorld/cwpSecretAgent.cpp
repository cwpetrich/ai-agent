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

      // std::ofstream debug_file;
      // debug_file.open("debug.txt", std::ofstream::out | std::ofstream::app);
      // debug_file << std::endl;
      // debug_file << "BEGINNING OF PROGRAM" << std::endl;
      // debug_file << "Charge: " << model->getCharge() << std::endl;

      ai::Scavenger::Action *action = new ai::Scavenger::Action;
      model->gatherData(percept);

      if (model->Move()){
        // debug_file << "if (model->Move())" << std::endl;
        action->SetCode(model->getNextActionToGoal()->getAction());
        return action;
      }
      if (model->Charge()){
        // debug_file << "if (model->Charge())" << std::endl;
        action->SetCode(ai::Scavenger::Action::RECHARGE);
        return action;
      }
      if (model->SearchBase()){
        // debug_file << "if (model->SearchBase())" << std::endl;
        ai::Search::Graph *search = SearchFromXYToGoalOrBase(model->getCurrX(), model->getCurrY(), model->getCharge(), true);
        if (search->Search()){
          std::list<ai::Search::Node *> *solution = search->GetSolution().GetList();
          std::list<ai::Search::Node *>::const_iterator it;
          for(it = solution->begin(); it != solution->end(); it++) {
            if((*it)->GetAction()) {
              model->addActionToGoal(dynamic_cast<cwp::Scavenger::Action *>((*it)->GetAction()));
            }
          }
          action->SetCode(model->getNextActionToGoal()->getAction());
        }
        else {
          action->SetCode(ai::Scavenger::Action::QUIT);
        }
        return action;
      }
      if (model->Look()){
        // debug_file << "if (model->Look())" << std::endl;
        action->SetCode(ai::Scavenger::Action::LOOK);
        ai::Scavenger::Location::Direction direction = model->getNextUndiscoveredDirection(model->getCurrX(), model->getCurrY());
        model->updateLookDirection(direction);
        action->SetDirection(direction);
        return action;
      }
      if (model->Pickup()){
        // debug_file << "if (model->Pickup())" << std::endl;
        action->SetCode(ai::Scavenger::Action::PICKUP);
        action->SetObjectId(model->getObjectToPickup());
        return action;
      }
      if (model->Examine()){
        // debug_file << "if (model->Examine())" << std::endl;
        action->SetCode(ai::Scavenger::Action::EXAMINE);
        action->SetObjectId(model->getObjectToExamine());
        return action;
      }
      if (model->Deposit()){
        // debug_file << "if (model->Deposit())" << std::endl;
        action->SetCode(ai::Scavenger::Action::DEPOSIT);
        action->SetObjectId(model->getObjectToDeposit());
        return action;
      }
      if (model->SearchCell()){
        // debug_file << "if (model->SearchCell())" << std::endl;
        cwp::Scavenger::CellData * closest_unvisited_cell = model->getClosestUnvisitedCell(model->getCurrX(), model->getCurrY());
        if (model->getCurrX() == 0.0 && model->getCurrY() == 0.0 && closest_unvisited_cell->getLocX() == 0.0 && closest_unvisited_cell->getLocY() == 0.0){
          action->SetCode(ai::Scavenger::Action::QUIT);
        }
        else {
          model->updateGoalLocation(closest_unvisited_cell->getLocX(), closest_unvisited_cell->getLocY(), closest_unvisited_cell->getLocZ());
          ai::Search::Graph *search = SearchFromXYToGoalOrBase(model->getCurrX(), model->getCurrY(), model->getCharge() - 5.0, false);
          std::list<ai::Search::Node *> * solution;
          cwp::Scavenger::State * cuc_state;
          if (search->Search()){
            solution = search->GetSolution().GetList();
            if (solution->size() == 1){
              cuc_state = dynamic_cast<cwp::Scavenger::State *>(solution->front()->GetState());
            }
            else {
              cuc_state = dynamic_cast<cwp::Scavenger::State *>(solution->back()->GetState());
            }
            search = SearchFromXYToGoalOrBase(cuc_state->getX(), cuc_state->getY(), cuc_state->getCharge() - 5.0, true);
            if (search->Search()){
              std::list<ai::Search::Node *>::const_iterator it;
              for(it = solution->begin(); it != solution->end(); it++) {
                if((*it)->GetAction()) {
                  model->addActionToGoal(dynamic_cast<cwp::Scavenger::Action *>((*it)->GetAction()));
                }
              }
              action->SetCode(model->getNextActionToGoal()->getAction());
            }
            else {
              search = SearchFromXYToGoalOrBase(model->getCurrX(), model->getCurrY(), model->getCharge(), true);
              if (search->Search()){
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
                search = SearchFromXYToGoalOrBase(model->getCurrX(), model->getCurrY(), model->getCharge(), false);
                action->SetCode(ai::Scavenger::Action::QUIT);
              }
            }
          }
          else {
            search = SearchFromXYToGoalOrBase(model->getCurrX(), model->getCurrY(), model->getCharge(), true);
            if (search->Search()){
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
              action->SetCode(ai::Scavenger::Action::QUIT);
            }
          }
        }
      } else {
        action->SetCode(ai::Scavenger::Action::NOOP);
      }
      // debug_file << "action->GetCode(): " << action->GetCode() << std::endl;
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
