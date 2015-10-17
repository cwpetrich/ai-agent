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

      ai::Scavenger::Action *action = new ai::Scavenger::Action;
      model->gatherData(percept);
      debug_file << "Charge: " << model->getCharge() << std::endl;
      debug_file << "isUndiscoveredDirections: " << model->isUndiscoveredDirections(model->getCurrX(), model->getCurrY()) << std::endl;
      if (model->isUndiscoveredDirections(model->getCurrX(), model->getCurrY())) {
        debug_file << "inside IF statement" << std::endl;
        action->SetCode(ai::Scavenger::Action::LOOK);
        ai::Scavenger::Location::Direction direction = model->getNextUndiscoveredDirection(model->getCurrX(), model->getCurrY());
        model->updateLookDirection(direction);
        action->SetDirection(direction);
      }else{
        debug_file << "inside ELSE statement" << std::endl;
        // model->updateLookDirection(-1);
        // cwp::Scavenger::CellData * current_cell = model->getCell(model->getCurrX(), model->getCurrY());
        // if (current_cell->getCellNorth() == "plain" || current_cell->getCellNorth() == "mud"){
        //   action->SetCode(ai::Scavenger::Action::GO_NORTH);
        // } else if (current_cell->getCellEast() == "plain" || current_cell->getCellEast() == "mud") {
        //   action->SetCode(ai::Scavenger::Action::GO_EAST);
        // } else if (current_cell->getCellWest() == "plain" || current_cell->getCellWest() == "mud") {
        //   action->SetCode(ai::Scavenger::Action::GO_WEST);
        // } else if (current_cell->getCellSouth() == "plain" || current_cell->getCellSouth() == "mud") {
        //   action->SetCode(ai::Scavenger::Action::GO_SOUTH);
        // } else {
        //   action->SetCode(ai::Scavenger::Action::QUIT);
        // }
        // debug_file << "Known Cells Size: " << model->getKnownCells().size() << std::endl;
        // debug_file << "Is Action Queue Empty: " << model->actionQueueEmpty() << std::endl;
        // debug_file << "Closest Unvisited Cell: " << model->getClosestUnvisitedCell(model->getCurrX(), model->getCurrY()) << std::endl;
        // debug_file << "Next Undiscovered Direction: " << model->getNextUndiscoveredDirection(model->getCurrX(), model->getCurrY()) << std::endl;
        if (model->actionQueueEmpty()) {
          cwp::Scavenger::CellData * closest_unvisited_cell = model->getClosestUnvisitedCell(model->getCurrX(), model->getCurrY());
          debug_file << "x: " << model->getGoalX() << " y: " << model->getGoalY() << " z: " << model->getGoalZ() << std::endl;
          model->updateGoalLocation(closest_unvisited_cell->getLocX(), closest_unvisited_cell->getLocY(), closest_unvisited_cell->getLocZ());

          cwp::Scavenger::State * initial_state = new cwp::Scavenger::State(model->getCurrX(), model->getCurrY(), model->getCharge());
          cwp::Scavenger::Problem * problem = new cwp::Scavenger::Problem(dynamic_cast<ai::Search::State *>(initial_state), model, false);
          
          ai::Search::Frontier *fringe  = new ai::Search::BFFrontier;
          ai::Search::Graph *search = new ai::Search::Graph(problem, fringe);

          if (search->Search()) {
            std::list<ai::Search::Node *> *solution = search->GetSolution().GetList();
            std::list<ai::Search::Node *>::const_iterator it;
            for (it = solution->begin(); it != solution->end(); it++) {
              if ((*it)->GetAction()) {
                (*it)->GetAction()->Display();
                cwp::Scavenger::Action* next_action = dynamic_cast<cwp::Scavenger::Action *>((*it)->GetAction());
                debug_file << "next_action->getAction(): " << next_action->getAction() << std::endl;
                debug_file << "1 actionQueueEmpty: " << model->actionQueueEmpty() << std::endl;
                model->addActionToGoal(next_action);
                debug_file << "2 actionQueueEmpty: " << model->actionQueueEmpty() << std::endl;
              }
            }
            action->SetCode(model->getNextActionToGoal()->getAction());
          }
        } else {
          action->SetCode(model->getNextActionToGoal()->getAction());
        }
      }
      return action;

      // cwp::Scavenger::State * initial_state = new cwp::Scavenger::State(model->getCurrX(), model->getCurrY(), model->getCharge());
      // cwp::Scavenger::Problem * problem = new cwp::Scavenger::Problem(dynamic_cast<ai::Search::State *>(initial_state), model);
      
      // ai::Search::Frontier *fringe  = new ai::Search::UCFrontier;
      // ai::Search::Graph *search = new ai::Search::Graph(problem, fringe);

      // if(model->searched == false){

      //   if(search->Search()) {
      //     std::list<ai::Search::Node *> *solution = search->GetSolution().GetList();
      //     std::list<ai::Search::Node *>::const_iterator it;

      //     // debug_file << "Actions: " << std::endl;
      //     for(it = solution->begin(); it != solution->end(); it++) {
      //       if((*it)->GetAction()) {
      //         // (*it)->GetAction()->Display();
      //         cwp::Scavenger::Action* next_action = dynamic_cast<cwp::Scavenger::Action *>((*it)->GetAction());
      //         // debug_file << next_action->getAction() << std::endl;
      //         model->addActionToGoal(next_action);
      //       }
      //     }
      //     // debug_file << "Path Cost: " << solution->back()->GetPathCost() << std::endl;
      //     // debug_file << "Nodes generated: " << search->GetNumberNodesGenerated() << std::endl;
      //     // debug_file << "Nodes stored:    " << search->GetMaxNodesStored() << std::endl;
      //     // debug_file << std::endl;
      //   }
      //   model->searched = true;
      // }
      // cwp::Scavenger::Action * next_action = model->getNextActionToGoal();
      // if (next_action == NULL){
      //   action->SetCode(ai::Scavenger::Action::QUIT);
      // }else{
      //   action->SetCode(next_action->getAction());
      // }
      // return action;
    }
  }
}
