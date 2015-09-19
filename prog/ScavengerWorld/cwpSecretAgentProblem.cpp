#include "cwpSecretAgentProblem.h"
#include <iostream>
#include <fstream>
#include <cmath>

namespace cwp {
	namespace Scavenger {

		Problem::Problem(ai::Search::State * initial_state, cwp::Scavenger::SecretAgentModel * const model)
		:ai::Search::Problem(initial_state)
		{

			this->initial_state = dynamic_cast<cwp::Scavenger::State * >(initial_state);
			this->model = model;
		}

		Problem::~Problem(){
		}

		std::vector<ai::Search::Action *> Problem::Actions(const ai::Search::State * const state_in){
    	const cwp::Scavenger::State * const state = dynamic_cast<const cwp::Scavenger::State * const>(state_in);
    	double x = state->getX();
    	double y = state->getY();
      cwp::Scavenger::CellData* current_cell = model->getCell(x, y);
			std::vector<ai::Search::Action*> actions;
			if (state->getCharge() <= 0){
				return actions;
			}
      if (current_cell->getCellNorth() == "plain" || current_cell->getCellNorth() == "mud"){
				cwp::Scavenger::Action * action = new cwp::Scavenger::Action(ai::Scavenger::Action::GO_NORTH);				
				actions.push_back(dynamic_cast<ai::Search::Action *>(action));
      }
      if (current_cell->getCellEast() == "plain" || current_cell->getCellEast() == "mud"){
				cwp::Scavenger::Action * action = new cwp::Scavenger::Action(ai::Scavenger::Action::GO_EAST);
				actions.push_back(dynamic_cast<ai::Search::Action *>(action));
      }
      if (current_cell->getCellWest() == "plain" || current_cell->getCellWest() == "mud"){
				cwp::Scavenger::Action * action = new cwp::Scavenger::Action(ai::Scavenger::Action::GO_WEST);
				actions.push_back(dynamic_cast<ai::Search::Action *>(action));
      }
      if (current_cell->getCellSouth() == "plain" || current_cell->getCellSouth() == "mud"){
				cwp::Scavenger::Action * action = new cwp::Scavenger::Action(ai::Scavenger::Action::GO_SOUTH);
				actions.push_back(dynamic_cast<ai::Search::Action *>(action));
      }
      return actions;
		}
    
    ai::Search::State * Problem::Result(const ai::Search::State * const state_in, const ai::Search::Action * const action_in){
    	const cwp::Scavenger::State * const state = dynamic_cast<const cwp::Scavenger::State * const>(state_in);
    	const cwp::Scavenger::Action * const action = dynamic_cast<const cwp::Scavenger::Action * const>(action_in);
    	std::ofstream debug_file;
      // debug_file.open("debug.txt", std::ofstream::out | std::ofstream::app);

      // debug_file << "state->getX() -> " << state->getX() << "\n";
      // debug_file << "state->getY() -> " << state->getY() << "\n";
      // debug_file << "state->getCharge() -> " << state->getCharge() << "\n";
      // debug_file << "ai::Savenger::Action::GO_NORTH -> " << ai::Scavenger::Action::GO_NORTH << "\n";
      // debug_file << "action->getAction() -> " << action->getAction() << "\n\n";

    	double x = state->getX();
    	double y = state->getY();

    	// debug_file << "y + 1000 -> " << y + 1000 << "\n\n";

    	cwp::Scavenger::CellData* current_cell = model->getCell(x, y);
    	cwp::Scavenger::CellData* next_cell = new cwp::Scavenger::CellData;
    	cwp::Scavenger::State * next_state;
    	std::string interface;
    	double interface_cost = 0.0;
    	if (action->getAction() == ai::Scavenger::Action::GO_NORTH){
    		// debug_file << "went north" << "\n";
  			next_cell = model->getCell(x, y + 1000);
  			interface = current_cell->getCellNorth();
    	}else if(action->getAction() == ai::Scavenger::Action::GO_EAST){
				// debug_file << "went east" << "\n";
  			next_cell = model->getCell(x + 1000, y);
  			interface = current_cell->getCellEast();
    	}else if(action->getAction() == ai::Scavenger::Action::GO_WEST){
				// debug_file << "went west" << "\n";
  			next_cell = model->getCell(x - 1000, y);
  			interface = current_cell->getCellWest();
    	}else if(action->getAction() == ai::Scavenger::Action::GO_SOUTH){
    		// debug_file << "went south" << "\n";
  			next_cell = model->getCell(x, y - 1000);
  			interface = current_cell->getCellSouth();
    	}else{
    		debug_file << "quit" << "\n";
    	}

    	if (interface == "plain"){
    		interface_cost = 1.0;
    	} else if (interface == "mud"){
    		interface_cost = 2.0;
    	} else if (interface == "rock"){
    		interface_cost = 2.0;
    	} else if (interface == "wall"){
    		interface_cost = 2.0;
    	} else if (interface == "cliff"){
    			interface_cost = 2.0;
    	}
    	next_state = new cwp::Scavenger::State(next_cell->getLocX(), next_cell->getLocY(), state->getCharge() - (interface_cost + ((current_cell->getLocZ() - next_cell->getLocZ())/1000)));
    	// debug_file << "next_cell->getLocX() -> " << next_cell->getLocX() << "\n";
    	// debug_file << "next_cell->getLocY() -> " << next_cell->getLocY() << "\n";
    	// debug_file << "next_state->getX() -> " << next_state->getX() << "\n";
     //  debug_file << "next_state->getY() -> " << next_state->getY() << "\n";
     //  debug_file << "next_state->getCharge() -> " << next_state->getCharge() << "\n\n";
      // debug_file.close();
    	return next_state;
    }
    
    bool Problem::GoalTest(const ai::Search::State * const state_in) const {
    	const cwp::Scavenger::State * const state = dynamic_cast<const cwp::Scavenger::State * const>(state_in);
    	return (fabs(model->getGoalX() - state->getX()) < 0.00001 && fabs(model->getGoalY() - state->getY()) < 0.00001 && state->getCharge() > 0 );
    }
    
    double Problem::StepCost(const ai::Search::State * const state1_in, const ai::Search::Action * const action_in, const ai::Search::State * const state2_in) const{
    	const cwp::Scavenger::State * const state1 = dynamic_cast<const cwp::Scavenger::State * const>(state1_in);
    	const cwp::Scavenger::State * const state2 = dynamic_cast<const cwp::Scavenger::State * const>(state2_in);
    	const cwp::Scavenger::Action * const action = dynamic_cast<const cwp::Scavenger::Action * const>(action_in);
    	cwp::Scavenger::CellData* cell1 = model->getCell(state1->getX(), state1->getY());
    	cwp::Scavenger::CellData* cell2 = model->getCell(state2->getX(), state2->getY());
    	std::string interface;
    	double interface_cost = 0.0;
    	if (action->getAction() == ai::Scavenger::Action::GO_NORTH){
  			interface = cell1->getCellNorth();
    	}else if(action->getAction() == ai::Scavenger::Action::GO_EAST){
  			interface = cell1->getCellEast();
    	}else if(action->getAction() == ai::Scavenger::Action::GO_WEST){
  			interface = cell1->getCellWest();
    	}else if(action->getAction() == ai::Scavenger::Action::GO_SOUTH){
  			interface = cell1->getCellSouth();
    	}else{
    		std::cout << "quit" << "\n";
    	}

    	if (interface == "plain"){
    		interface_cost = 1.0;
    	} else if (interface == "mud"){
    		interface_cost = 2.0;
    	} else if (interface == "rock"){
    		interface_cost = 2.0;
    	} else if (interface == "wall"){
    		interface_cost = 2.0;
    	} else if (interface == "cliff"){
    		interface_cost = 2.0;
    	}
    	return (interface_cost + ((cell2->getLocZ() - cell1->getLocZ())/1000));
    }

	}
}