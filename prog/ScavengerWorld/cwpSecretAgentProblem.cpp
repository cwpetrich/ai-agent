#include "cwpSecretAgentProblem.h"
#include <iostream>
#include <fstream>
#include <cmath>

namespace cwp {
	namespace Scavenger {

		Problem::Problem(ai::Search::State * initial_state, cwp::Scavenger::SecretAgentModel * const model, bool to_base)
		:ai::Search::Problem(initial_state)
		{
			std::ofstream debug_file;
			// debug_file.open("debug.txt", std::ofstream::app | std::ofstream::out);
			// debug_file << "Start of Problem::Problem" << std::endl;
			debug_file.close();
			this->initial_state = dynamic_cast<cwp::Scavenger::State * >(initial_state);
			this->model = model;
			this->to_base = to_base;
		}

		Problem::~Problem(){}

		std::vector<ai::Search::Action *> Problem::Actions(const ai::Search::State * const state_in)
		{
			// std::ofstream debug_file;
			// debug_file.open("debug.txt", std::ofstream::app | std::ofstream::out);
			// debug_file << "Start of Problem::Actions" << std::endl;
			// debug_file.close();
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
			// std::ofstream debug_file;
			// debug_file.open("debug.txt", std::ofstream::app | std::ofstream::out);
			// debug_file << "Start of Problem::Result" << std::endl;
			// debug_file.close();
			const cwp::Scavenger::State * const state = dynamic_cast<const cwp::Scavenger::State * const>(state_in);
			const cwp::Scavenger::Action * const action = dynamic_cast<const cwp::Scavenger::Action * const>(action_in);

			double x = state->getX();
			double y = state->getY();

			cwp::Scavenger::CellData* current_cell = model->getCell(x, y);
			cwp::Scavenger::CellData* next_cell = new cwp::Scavenger::CellData;
			cwp::Scavenger::State * next_state;
			std::string interface;
			double interface_cost = 0.0;
			if (action->getAction() == ai::Scavenger::Action::GO_NORTH){
				next_cell = model->getCell(x, y + 1000);
				interface = current_cell->getCellNorth();
			}else if(action->getAction() == ai::Scavenger::Action::GO_EAST){
				next_cell = model->getCell(x + 1000, y);
				interface = current_cell->getCellEast();
			}else if(action->getAction() == ai::Scavenger::Action::GO_WEST){
				next_cell = model->getCell(x - 1000, y);
				interface = current_cell->getCellWest();
			}else if(action->getAction() == ai::Scavenger::Action::GO_SOUTH){
				next_cell = model->getCell(x, y - 1000);
				interface = current_cell->getCellSouth();
			}else{
				next_cell = model->getCell(x, y);
				interface = "quit";
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
			} else {
				interface_cost = 0.0;
			}
			next_state = new cwp::Scavenger::State(next_cell->getLocX(), next_cell->getLocY(), state->getCharge() - (interface_cost + ((current_cell->getLocZ() - next_cell->getLocZ())/1000)));
			return next_state;
		}

		bool Problem::GoalTest(const ai::Search::State * const state_in) const {
			// std::ofstream debug_file;
			// debug_file.open("debug.txt", std::ofstream::app | std::ofstream::out);
			// debug_file << "Start of Problem::GoalTest" << std::endl;
			// debug_file.close();
			const cwp::Scavenger::State * const state = dynamic_cast<const cwp::Scavenger::State * const>(state_in);
			if (to_base){
				return (fabs(0.0 - state->getX()) < 0.00001 && fabs(0.0 - state->getY()) < 0.00001 && state->getCharge() > 0 );
			}else{
				return (fabs(model->getGoalX() - state->getX()) < 0.00001 && fabs(model->getGoalY() - state->getY()) < 0.00001 && state->getCharge() > 0 );
			}
		}

		double Problem::StepCost(const ai::Search::State * const state1_in, const ai::Search::Action * const action_in, const ai::Search::State * const state2_in) const{
			// std::ofstream debug_file;
			// debug_file.open("debug.txt", std::ofstream::app | std::ofstream::out);
			// debug_file << "Start of Problem::StepCost" << std::endl;
			// debug_file.close();
			const cwp::Scavenger::State * const state1 = dynamic_cast<const cwp::Scavenger::State * const>(state1_in);
			const cwp::Scavenger::State * const state2 = dynamic_cast<const cwp::Scavenger::State * const>(state2_in);
			const cwp::Scavenger::Action * const action = dynamic_cast<const cwp::Scavenger::Action * const>(action_in);
			cwp::Scavenger::CellData* cell1 = model->getCell(state1->getX(), state1->getY());
			cwp::Scavenger::CellData* cell2 = model->getCell(state2->getX(), state2->getY());
			std::string interface = "quit";
			double interface_cost = 0.0;
			if (action->getAction() == ai::Scavenger::Action::GO_NORTH) {
				interface = cell1->getCellNorth();
			} else if (action->getAction() == ai::Scavenger::Action::GO_EAST) {
				interface = cell1->getCellEast();
			} else if (action->getAction() == ai::Scavenger::Action::GO_WEST) {
				interface = cell1->getCellWest();
			} else if (action->getAction() == ai::Scavenger::Action::GO_SOUTH) {
				interface = cell1->getCellSouth();
			} else {
				interface = "quit";
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
			} else {
				interface_cost = 0.0;
			}
			return (interface_cost + ((cell2->getLocZ() - cell1->getLocZ())/1000));
		}

		double Problem::Heuristic(const ai::Search::State * const state_in) const{
			const cwp::Scavenger::State * const state = dynamic_cast<const cwp::Scavenger::State * const>(state_in);
			double distance = 0.0;
			if (to_base){
				distance += fabs(0.0 - state->getX());
				distance += fabs(0.0 - state->getY());
			} else {
				distance += fabs(model->getGoalX() - state->getX());
				distance += fabs(model->getGoalY() - state->getY());
			}
			return distance;
		}

	}
}