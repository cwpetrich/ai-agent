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
      std::stringstream ss1;

      int agent_base;
      double agent_x, agent_y, agent_z, agent_hp, agent_charge, agent_goal_x, agent_goal_y, agent_goal_z;

      ss1.str(percept->GetAtom("BASE").GetValue()); ss1.clear();
      ss1 >> agent_base;
      model->updateBaseNum(agent_base);

      ss1.str(percept->GetAtom("X_LOC").GetValue()); ss1.clear();
      ss1 >> agent_x;
      ss1.str(percept->GetAtom("Y_LOC").GetValue()); ss1.clear();
      ss1 >> agent_y;
      ss1.str(percept->GetAtom("Z_LOC").GetValue()); ss1.clear();
      ss1 >> agent_z;
      model->updateCurrLocation(agent_x, agent_y, agent_z);

      ss1.str(percept->GetAtom("CHARGE").GetValue()); ss1.clear();
      ss1 >> agent_charge;
      model->updateCharge(agent_charge);

      ss1.str(percept->GetAtom("HP").GetValue()); ss1.clear();
      ss1 >> agent_hp;
      model->updateHitPoints(agent_hp);

      std::string agent_goal_loc = percept->GetAtom("GOAL_LOC").GetValue();
      ss1.str(agent_goal_loc); ss1.clear();
      ss1 >> agent_goal_x; ss1.ignore();
      ss1 >> agent_goal_y; ss1.ignore();
      ss1 >> agent_goal_z; ss1.ignore();
      model->updateGoalLocation(agent_goal_x, agent_goal_y, agent_goal_z);

      for (uint i = 0; i < percept->NumAtom(); i++){
        if (percept->GetAtom(i).GetName().substr(0, 5) == "CELL_"){
		      std::stringstream ss2;
        	double cell_x, cell_y, cell_z;
      		std::string cell_north, cell_south, cell_east, cell_west;

        	std::string value = percept->GetAtom(i).GetValue();
	        ss2.str(value); ss2.clear();
	        ss2 >> cell_x; ss2.ignore();
	        ss2 >> cell_y; ss2.ignore();
	        ss2 >> cell_z; ss2.ignore();
	        std::getline(ss2, cell_north, ',');
	        std::getline(ss2, cell_south, ',');
	        std::getline(ss2, cell_east, ',');
	        std::getline(ss2, cell_west, ',');

	        model->updateCell(percept->GetAtom(i).GetName().substr(5), cell_x, cell_y, cell_z, cell_north, cell_south, cell_east, cell_west);
        }
      }

      cwp::Scavenger::State * initial_state = new cwp::Scavenger::State(0.0, 0.0, 100.0);
      cwp::Scavenger::Problem * problem = new cwp::Scavenger::Problem(dynamic_cast<ai::Search::State *>(initial_state), model);
      
      ai::Search::Frontier *fringe  = new ai::Search::DFFrontier;
      ai::Search::Tree *search1 = new ai::Search::Tree(problem, fringe);

      if(search1->Search()) {
        std::list<ai::Search::Node *> *solution = search1->GetSolution().GetList();
        std::list<ai::Search::Node *>::const_iterator it;
        std::vector<cwp::Scavenger::Action *> solution_actions;
        std::ofstream debug_file;
        debug_file.open("debug.txt", std::ofstream::out | std::ofstream::app);

        for(it = solution->begin(); it != solution->end(); it++) {
          if((*it)->GetAction()) {
            (*it)->GetAction()->Display();
            solution_actions.push_back(dynamic_cast<cwp::Scavenger::Action *>((*it)->GetAction()));
          }
        }
        debug_file << "Actions: " << "\n";
        for (uint i = 0; i < solution_actions.size(); i++){
          switch (solution_actions[i]->getAction()){
            case ai::Scavenger::Action::GO_NORTH: debug_file << "north -> " << solution_actions[i]->getAction() << std::endl; break;
            case ai::Scavenger::Action::GO_EAST: debug_file << "east -> " << solution_actions[i]->getAction() << std::endl; break;
            case ai::Scavenger::Action::GO_WEST: debug_file << "west -> " << solution_actions[i]->getAction() << std::endl; break;
            case ai::Scavenger::Action::GO_SOUTH: debug_file << "south -> " << solution_actions[i]->getAction() << std::endl; break;
            case ai::Scavenger::Action::QUIT: debug_file << "quit -> " << solution_actions[i]->getAction() << std::endl; break;
          }
        }
        debug_file << "Charge Left: " << model->getCharge() - solution->back()->GetPathCost() << std::endl;
        // debug_file << "\nStates: " << "\n";
        // for (int i = 0; i < solution_states.size(); i++){
        //   debug_file << solution_states[i]->get
        // }

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

      // const cwp::Scavenger::State * const state = new cwp::Scavenger::State(current_cell->getLocX(), current_cell->getLocY(), model->getCharge());
      // double state_x, state_y, state_c;
      // state_x = state->getX();
      // state_y = state->getY();
      // state_c = state->getCharge();
      // std::ofstream myfile;
      // myfile.open("debugging.txt", std::ofstream::out | std::ofstream::app);
      // myfile << "StateX: " << state_x << "\n";
      // myfile << "StateY: " << state_y << "\n";
      // myfile << "StateC: " << state_c << "\n\n";
      // myfile << "Action Taken: " << new_action->getAction() << "\n\n";
      // myfile.close();
      return action;
    }
  }
}
