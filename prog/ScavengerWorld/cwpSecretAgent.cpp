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

      ai::Scavenger::Action *action = new ai::Scavenger::Action;
      model->gatherData(percept);

      cwp::Scavenger::State * initial_state = new cwp::Scavenger::State(model->getCurrX(), model->getCurrY(), model->getCharge());
      cwp::Scavenger::Problem * problem = new cwp::Scavenger::Problem(dynamic_cast<ai::Search::State *>(initial_state), model);
      
      ai::Search::Frontier *fringe  = new ai::Search::DFFrontier;
      ai::Search::Tree *search1 = new ai::Search::Tree(problem, fringe);

      if(search1->Search()) {
        std::list<ai::Search::Node *> *solution = search1->GetSolution().GetList();
        std::list<ai::Search::Node *>::const_iterator it;
        std::ofstream debug_file;
        debug_file.open("debug.txt", std::ofstream::out | std::ofstream::app);

        for(it = solution->begin(); it != solution->end(); it++) {
          if((*it)->GetAction()) {
            (*it)->GetAction()->Display();
            model->addActionToGoal(dynamic_cast<cwp::Scavenger::Action *>((*it)->GetAction()));
          }
        }
        debug_file << "Actions: " << "\n";
        while(cwp::Scavenger::Action * next_action = model->getNextActionToGoal()){
          switch (next_action->getAction()){
            case ai::Scavenger::Action::GO_NORTH: debug_file << "north -> " << next_action->getAction() << std::endl; break;
            case ai::Scavenger::Action::GO_EAST: debug_file << "east -> " << next_action->getAction() << std::endl; break;
            case ai::Scavenger::Action::GO_WEST: debug_file << "west -> " << next_action->getAction() << std::endl; break;
            case ai::Scavenger::Action::GO_SOUTH: debug_file << "south -> " << next_action->getAction() << std::endl; break;
            case ai::Scavenger::Action::QUIT: debug_file << "quit -> " << next_action->getAction() << std::endl; break;
          }
        }
        debug_file << "Charge Left: " << model->getCharge() - solution->back()->GetPathCost() << std::endl;

        debug_file << "Path Cost: " << solution->back()->GetPathCost() << std::endl;

        debug_file << "Nodes generated: " << search1->GetNumberNodesGenerated() << std::endl;
        debug_file << "Nodes stored:    " << search1->GetMaxNodesStored() << std::endl;
        debug_file.close();
      }
      delete search1;
      search1 = 0;
      exit(0);
      // if (model->actions_to_goal.empty()){

      // }

      // ai::Search::State * new_state = problem->Result(initial_state, initial_action);
      // cwp::Scavenger::State * newest_state = dynamic_cast<cwp::Scavenger::State *>(new_state);
      // std::cout << newest_state->getX() << std::endl;

      // double current_x, current_y;
      // current_x = model->getCurrX();
      // current_y = model->getCurrY();
      // cwp::Scavenger::CellData* current_cell = model->getCell(current_x, current_y);

      // cwp::Scavenger::Action * new_action = new cwp::Scavenger::Action;

      // if (current_cell->getCellNorth() == "plain" || current_cell->getCellNorth() == "mud"){
      //   action->SetCode(ai::Scavenger::Action::GO_NORTH);
      //   new_action->SetCode(ai::Scavenger::Action::GO_NORTH);
      // }else if (current_cell->getCellEast() == "plain" || current_cell->getCellEast() == "mud"){
      //   action->SetCode(ai::Scavenger::Action::GO_EAST);
      //   new_action->SetCode(ai::Scavenger::Action::GO_EAST);
      // }else if (current_cell->getCellWest() == "plain" || current_cell->getCellWest() == "mud"){
      //   action->SetCode(ai::Scavenger::Action::GO_WEST);
      //   new_action->SetCode(ai::Scavenger::Action::GO_WEST);
      // }else if (current_cell->getCellSouth() == "plain" || current_cell->getCellSouth() == "mud"){
      //   action->SetCode(ai::Scavenger::Action::GO_SOUTH);
      //   new_action->SetCode(ai::Scavenger::Action::GO_SOUTH);
      // }else{
      //   action->SetCode(ai::Scavenger::Action::QUIT);
      //   new_action->SetCode(ai::Scavenger::Action::QUIT);
      // }
      return action;
    }
  }
}
