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
      debug_file << std::endl;
      debug_file << "BEGINNING OF PROGRAM" << std::endl;
      debug_file << "Charge: " << model->getCharge() << std::endl;

      ai::Scavenger::Action *action = new ai::Scavenger::Action;
      model->gatherData(percept);

      if (model->Move()){
        debug_file << "if (model->Move())" << std::endl;
        action->SetCode(model->getNextActionToGoal()->getAction());
        // return action;
      }
      else if (model->Charge()){
        debug_file << "if (model->Charge())" << std::endl;
        action->SetCode(ai::Scavenger::Action::RECHARGE);
        // return action;
      }
      else if (model->SearchBase()){
        debug_file << "if (model->SearchBase())" << std::endl;
        ai::Search::Graph *search = SearchFromCurrentCellToBase();
        if (search->Search()){
          std::list<ai::Search::Node *> *solution = search->GetSolution().GetList();
          std::list<ai::Search::Node *>::const_iterator it;
          for(it = solution->begin(); it != solution->end(); it++) {
            if((*it)->GetAction()) {
              model->addActionToGoal(dynamic_cast<cwp::Scavenger::Action *>((*it)->GetAction()));
            }
          }
          if(!model->actionQueueEmpty()){
            action->SetCode(model->getNextActionToGoal()->getAction());
          }else{
            action->SetCode(ai::Scavenger::Action::NOOP);
          }
        }
        else {
          action->SetCode(ai::Scavenger::Action::QUIT);
        }
        // return action;
      }
      else if (model->Look()){
        debug_file << "if (model->Look())" << std::endl;
        action->SetCode(ai::Scavenger::Action::LOOK);
        ai::Scavenger::Location::Direction direction = model->getNextUndiscoveredDirection(model->getCurrX(), model->getCurrY());
        model->updateLookDirection(direction);
        action->SetDirection(direction);
        // return action;
      }
      else if (model->Pickup()){
        debug_file << "if (model->Pickup())" << std::endl;
        action->SetCode(ai::Scavenger::Action::PICKUP);
        action->SetObjectId(model->getObjectToPickup());
        // return action;
      }
      else if (model->Examine()){
        debug_file << "if (model->Examine())" << std::endl;
        action->SetCode(ai::Scavenger::Action::EXAMINE);
        action->SetObjectId(model->getObjectToExamine());
        // return action;
      }
      else if (model->Deposit()){
        debug_file << "if (model->Deposit())" << std::endl;
        action->SetCode(ai::Scavenger::Action::DEPOSIT);
        action->SetObjectId(model->getObjectToDeposit());
        // return action;
      }
      else if (model->SearchCell()){
        debug_file << "if (model->SearchCell())" << std::endl;
        cwp::Scavenger::CellData * cu_cell = model->getClosestUnvisitedCell(model->getCurrX(), model->getCurrY());
        model->updateGoalLocation(cu_cell->getLocX(), cu_cell->getLocY(), cu_cell->getLocZ());
        ai::Search::Graph *search;
        std::list<ai::Search::Node *> *solution;
        std::list<ai::Search::Node *>::const_iterator it;

        if (cu_cell->isBase() && (model->getCurrX() == 0.0 && model->getCurrY() == 0.0)){
          debug_file << "INSIDE OF ELSE BS && BL" << std::endl;
          action->SetCode(ai::Scavenger::Action::QUIT);
          // return action;
        }
        else if (cu_cell->isBase() && !(model->getCurrX() == 0.0 && model->getCurrY() == 0.0)){
          debug_file << "INSIDE OF ELSE BS && !BL" << std::endl;
          search = SearchFromCurrentCellToBase();
          if(search->Search()){
            solution = search->GetSolution().GetList();
            for(it = solution->begin(); it != solution->end(); it++){
              if((*it)->GetAction()){
                model->addActionToGoal(dynamic_cast<cwp::Scavenger::Action *>((*it)->GetAction()));
              }
            }
            if(!model->actionQueueEmpty()){
              action->SetCode(model->getNextActionToGoal()->getAction());
            } else {
              action->SetCode(ai::Scavenger::Action::NOOP);
            }
          } else {
            action->SetCode(ai::Scavenger::Action::QUIT);
          }
          // return action;
        }
        else if (!cu_cell->isBase() && (model->getCurrX() == 0.0 && model->getCurrY() == 0.0)){
          debug_file << "INSIDE OF ELSE !BS && BL" << std::endl;

          search = SearchFromCurrentCellToUnvisitedCell();
          if(search->Search()){
            solution = search->GetSolution().GetList();
            cwp::Scavenger::State *cu_cell_state;
            if (solution->size() == 1){
              cu_cell_state = dynamic_cast<cwp::Scavenger::State*>(solution->front()->GetState());
            } else {
              cu_cell_state = dynamic_cast<cwp::Scavenger::State*>(solution->back()->GetState());
            }
            ai::Search::Graph * search2 = SearchFromAnotherCellToBase(cu_cell_state->getX(), cu_cell_state->getY(), cu_cell_state->getCharge());
            if(search2->Search()){
              for(it = solution->begin(); it != solution->end(); it++){
                if((*it)->GetAction()){
                  model->addActionToGoal(dynamic_cast<cwp::Scavenger::Action *>((*it)->GetAction()));
                }
              }
              if(!model->actionQueueEmpty()){
                action->SetCode(model->getNextActionToGoal()->getAction());
              } else {
                action->SetCode(ai::Scavenger::Action::NOOP);
              }
            } else {
              action->SetCode(ai::Scavenger::Action::QUIT);
            }
          } else {
            action->SetCode(ai::Scavenger::Action::QUIT);
          }
          // return action;
        }
        else if (!cu_cell->isBase() && !(model->getCurrX() == 0.0 && model->getCurrY() == 0.0)){
          debug_file << "INSIDE OF ELSE !BS && !BL" << std::endl;
          search = SearchFromCurrentCellToUnvisitedCell();
          if(search->Search()){
            solution = search->GetSolution().GetList();
            cwp::Scavenger::State *cu_cell_state;
            if (solution->size() == 1){
              cu_cell_state = dynamic_cast<cwp::Scavenger::State*>(solution->front()->GetState());
            } else {
              cu_cell_state = dynamic_cast<cwp::Scavenger::State*>(solution->back()->GetState());
            }
            ai::Search::Graph * search2 = SearchFromAnotherCellToBase(cu_cell_state->getX(), cu_cell_state->getY(), cu_cell_state->getCharge() - 5.0);
            if(search2->Search()){
              for(it = solution->begin(); it != solution->end(); it++){
                if((*it)->GetAction()){
                  model->addActionToGoal(dynamic_cast<cwp::Scavenger::Action *>((*it)->GetAction()));
                }
              }
              if(!model->actionQueueEmpty()){
                action->SetCode(model->getNextActionToGoal()->getAction());
              } else {
                action->SetCode(ai::Scavenger::Action::NOOP);
              }
            } else {
              search = SearchFromCurrentCellToBase();
              if (search->Search()){
                solution = search->GetSolution().GetList();
                for(it = solution->begin(); it != solution->end(); it++){
                  if((*it)->GetAction()){
                    model->addActionToGoal(dynamic_cast<cwp::Scavenger::Action *>((*it)->GetAction()));
                  }
                }
                if(!model->actionQueueEmpty()){
                  action->SetCode(model->getNextActionToGoal()->getAction());
                } else {
                  action->SetCode(ai::Scavenger::Action::NOOP);
                }
              }
            }
          } else {
            search = SearchFromCurrentCellToBase();
            if(search->Search()){
              solution = search->GetSolution().GetList();
              for(it = solution->begin(); it != solution->end(); it++){
                if((*it)->GetAction()){
                  model->addActionToGoal(dynamic_cast<cwp::Scavenger::Action *>((*it)->GetAction()));
                }
              }
              if(!model->actionQueueEmpty()){
                action->SetCode(model->getNextActionToGoal()->getAction());
              } else {
                action->SetCode(ai::Scavenger::Action::NOOP);
              }
            } else {
              action->SetCode(ai::Scavenger::Action::QUIT);
            }
          }
          // return action;
        }
        else {
          debug_file << "INSIDE OF ELSE" << std::endl;
          action->SetCode(ai::Scavenger::Action::NOOP);
          // return action;
        }
      } else {
        debug_file << "ELSE IN CHANE OF POTENTIAL ACTIONS" << std::endl;
        action->SetCode(ai::Scavenger::Action::QUIT);
      }
      debug_file << "action->GetCode(): " << action->GetCode() << std::endl;
      return action;
    }

    ai::Search::Graph * SecretAgent::SearchFromCurrentCellToBase(){
      cwp::Scavenger::State * initial_state = new cwp::Scavenger::State(model->getCurrX(), model->getCurrY(), model->getCharge());
      cwp::Scavenger::Problem * problem = new cwp::Scavenger::Problem(dynamic_cast<ai::Search::State *>(initial_state), model, true);
      ai::Search::Frontier * fringe = new ai::Search::AStarFrontier;
      ai::Search::Graph * search = new ai::Search::Graph(problem, fringe);
      return search;
    }

    ai::Search::Graph * SecretAgent::SearchFromCurrentCellToUnvisitedCell(){
      cwp::Scavenger::State * initial_state = new cwp::Scavenger::State(model->getCurrX(), model->getCurrY(), model->getCharge());
      cwp::Scavenger::Problem * problem = new cwp::Scavenger::Problem(dynamic_cast<ai::Search::State *>(initial_state), model, false);
      ai::Search::Frontier * fringe = new ai::Search::UCFrontier;
      ai::Search::Graph * search = new ai::Search::Graph(problem, fringe);
      return search;
    }

    ai::Search::Graph * SecretAgent::SearchFromAnotherCellToBase(double x, double y, double charge){
      cwp::Scavenger::State * initial_state = new cwp::Scavenger::State(x, y, charge - 5.0);
      cwp::Scavenger::Problem * problem = new cwp::Scavenger::Problem(dynamic_cast<ai::Search::State *>(initial_state), model, true);
      ai::Search::Frontier * fringe = new ai::Search::AStarFrontier;
      ai::Search::Graph * search = new ai::Search::Graph(problem, fringe);
      return search;
    }
  }
}
