#include "cwpSecretAgentModel.h"
#include <iostream>
#include <fstream>

namespace cwp {

	namespace Scavenger {

		SecretAgentModel::SecretAgentModel(){
			this->direction = -1;
			this->hopper_full = false;
		}

		SecretAgentModel::~SecretAgentModel(){}

		double SecretAgentModel::getCurrX() const {
			return curr_x;
		}

		double SecretAgentModel::getCurrY() const {
			return curr_y;
		}

		double SecretAgentModel::getCurrZ() const {
			return curr_z;
		}

		void SecretAgentModel::updateGoalLocation(double x, double y, double z){
			goal_x = x;
			goal_y = y;
			goal_z = z;
		}

		double SecretAgentModel::getGoalX() const {
			return goal_x;
		}

		double SecretAgentModel::getGoalY() const {
			return goal_y;
		}

		double SecretAgentModel::getGoalZ() const {
			return goal_z;
		}

		CellData* SecretAgentModel::getCell(double x, double y){
			CellData* cell = new CellData();
			CellKey key = CellKey(x, y);
			std::pair<std::map<CellKey, CellData*>::iterator, bool> existing_cell;
			existing_cell = known_cells.insert(std::pair<CellKey, CellData*>(key, cell));
			return existing_cell.first->second;
		}

		CellData* SecretAgentModel::getClosestUnvisitedCell(double x, double y){
			std::ofstream debug_file;
			debug_file.open("debug.txt", std::ofstream::app | std::ofstream::out);
			CellData* closest_cell = NULL;
			double min = 1000000.00;
			std::map<CellKey, CellData*>::iterator iter;
			for (iter = known_cells.begin(); iter != known_cells.end(); iter++) {
				if (!isCellVisited(iter->first.getX(), iter->first.getY())) {
					if (iter->second->isSafe()) {
						double a = fabs(iter->first.getX() - x);
						double b = fabs(iter->first.getY() - y);
						double c = sqrt(a*a + b*b);
						if (c < min) {
							min = c;
							closest_cell = iter->second;
						}
					}
				}
			}
			if (closest_cell == NULL) {
				debug_file << "closest_cell == NULL" << std::endl;
				double min = 1000000.00;
				std::map<CellKey, CellData*>::iterator iter;
				for (iter = known_cells.begin(); iter != known_cells.end(); iter++) {
					if (!isCellVisited(iter->first.getX(), iter->first.getY())) {
						if (iter->second->isPassable()) {
							double a = fabs(iter->first.getX() - x);
							double b = fabs(iter->first.getY() - y);
							double c = sqrt(a*a + b*b);
							if (c < min) {
								min = c;
								closest_cell = iter->second;
							}
						}
					}
				}
			}
			if (closest_cell == NULL){
				closest_cell = getCell(0.0, 0.0);
			}
			return closest_cell;
		}

		bool SecretAgentModel::isCellVisited(double x, double y){
			CellData * existing_cell = getCell(x, y);
			return existing_cell->isVisited();
		}

		double SecretAgentModel::getCharge() const {
			return charge;
		}

		double SecretAgentModel::getHitPoints() const {
			return hit_points;
		}

		int SecretAgentModel::getBaseNum() const {
			return base_num;
		}

		void SecretAgentModel::gatherData(const ai::Agent::Percept * percept){
			std::ofstream debug_file;
      debug_file.open("debug.txt", std::ofstream::out | std::ofstream::app);
			std::stringstream ss1;

			ss1.str(percept->GetAtom("BASE").GetValue()); ss1.clear();
			ss1 >> base_num;

			ss1.str(percept->GetAtom("X_LOC").GetValue()); ss1.clear();
			ss1 >> curr_x;
			ss1.str(percept->GetAtom("Y_LOC").GetValue()); ss1.clear();
			ss1 >> curr_y;
			ss1.str(percept->GetAtom("Z_LOC").GetValue()); ss1.clear();
			ss1 >> curr_z;

			cwp::Scavenger::CellData * current_cell = getCell(curr_x, curr_y);
			current_cell->updateCellLocation(curr_x, curr_y, curr_z);

			std::string cell_interface;
			ss1.str(percept->GetAtom("LOOK").GetValue()); ss1.clear();
			ss1 >> cell_interface;
			if (cell_interface != ""){
				cwp::Scavenger::CellData * neighbor_cell;
				switch(getLookDirection()) {
					case ai::Scavenger::Location::NORTH:
						current_cell->updateCellNorth(cell_interface);
						neighbor_cell = getCell(curr_x, curr_y + 1000);
						neighbor_cell->updateCellLocation(curr_x, curr_y + 1000, curr_z);
						neighbor_cell->updateCellSouth(cell_interface);
						break;
					case ai::Scavenger::Location::SOUTH:
						current_cell->updateCellSouth(cell_interface);
						neighbor_cell = getCell(curr_x, curr_y - 1000);
						neighbor_cell->updateCellLocation(curr_x, curr_y - 1000, curr_z);
						neighbor_cell->updateCellNorth(cell_interface);
						break;
					case ai::Scavenger::Location::EAST:
						current_cell->updateCellEast(cell_interface);
						neighbor_cell = getCell(curr_x + 1000, curr_y);
						neighbor_cell->updateCellLocation(curr_x + 1000, curr_y, curr_z);
						neighbor_cell->updateCellWest(cell_interface);
						break;
					case ai::Scavenger::Location::WEST:
						current_cell->updateCellWest(cell_interface);
						neighbor_cell = getCell(curr_x - 1000, curr_y);
						neighbor_cell->updateCellLocation(curr_x - 1000, curr_y, curr_z);
						neighbor_cell->updateCellEast(cell_interface);
						break;
				}
			}

			std::string examine = percept->GetAtom("EXAMINE").GetValue();
      if (examine != ""){
				std::string id, color, shape, size;
	      ss1.str(examine); ss1.clear();
	      ss1 >> id; ss1.ignore();
	      ss1 >> color; ss1.ignore();
	      ss1 >> shape; ss1.ignore();
	      ss1 >> size; ss1.ignore();
	      cwp::Scavenger::Object * object = getObject(id);
      	object->updateAttributes(color, shape, size);
      	object->markExamined();
      }

			std::list<std::string> object_ids_in_percept;
			for (uint i = 0; i < percept->NumAtom(); i++){
				if (percept->GetAtom(i).GetName().substr(0, 7) == "OBJECT_"){
					std::string object_name = percept->GetAtom(i).GetName();
					std::string object_id;
					ss1.str(percept->GetAtom(object_name).GetValue()); ss1.clear();
					ss1 >> object_id;
					object_ids_in_percept.push_back(object_id);
					cwp::Scavenger::Object * object = getObject(object_id);
					debug_file << object << std::endl;
					updateObjectForAction(object_id);
					if (object->isExamined()){
						updateActionForObject(ai::Scavenger::Action::PICKUP);
					} else {
						updateActionForObject(ai::Scavenger::Action::EXAMINE);
					}
				}
				if (percept->GetAtom(i).GetName().substr(0, 7) == "HOPPER_"){
					std::string object_name = percept->GetAtom(i).GetName();
					std::string object_id;
					ss1.str(percept->GetAtom(object_name).GetValue()); ss1.clear();
					ss1 >> object_id;
					cwp::Scavenger::Object * object = getObject(object_id);
					object->markInHopper();
				}
			}

			std::string full = percept->GetAtom("PICKUP").GetValue();
			if (full == "Full"){
				updateObjectForAction("");
				updateHopperStatus(true);
			} else {
				updateHopperStatus(false);
			}

			if(!isUndiscoveredDirections(curr_x, curr_y) && object_ids_in_percept.empty()){
				CellData * cell = getCell(curr_x, curr_y);
				cell->markVisited();
			}

			// debug_file << objects_found.size() << std::endl;
			ss1.str(percept->GetAtom("CHARGE").GetValue()); ss1.clear();
			ss1 >> charge;

			ss1.str(percept->GetAtom("HP").GetValue()); ss1.clear();
			ss1 >> hit_points;
		}

		void SecretAgentModel::addActionToGoal(cwp::Scavenger::Action * action){
			actions_to_goal.push(action);
		}

		cwp::Scavenger::Action * SecretAgentModel::getNextActionToGoal(){
			if(actions_to_goal.empty()){
				return NULL;
			}else{
				cwp::Scavenger::Action *action = actions_to_goal.front();
				actions_to_goal.pop();
				return action;
			}
		}

		bool SecretAgentModel::actionQueueEmpty() {
			return actions_to_goal.empty();
		}

		bool SecretAgentModel::isUndiscoveredDirections(double x, double y) {
			// std::ofstream debug_file;
			// debug_file.open("debug.txt", std::ofstream::out | std::ofstream::app);
			bool undiscovered_directions = false;
			CellData * existing_cell = getCell(x, y);
			std::string interface_north = existing_cell->getCellNorth();
			std::string interface_south = existing_cell->getCellSouth();
			std::string interface_east = existing_cell->getCellEast();
			std::string interface_west = existing_cell->getCellWest();
			// debug_file << "interface_north -> " << interface_north << "\ninterface_south -> " << interface_south;
			// debug_file << "\ninterface_east -> " << interface_east << "\ninterface_west -> " << interface_west << std::endl;
			if (interface_north == "") {
				// debug_file << "NORTH" << std::endl;
				undiscovered_directions = true;
			} else if (interface_south == "") {
				// debug_file << "SOUTH" << std::endl;
				undiscovered_directions = true;
			} else if (interface_east == "") {
				// debug_file << "EAST" << std::endl;
				undiscovered_directions = true;
			} else if (interface_west == "") {
				// debug_file << "WEST" << std::endl;
				undiscovered_directions = true;
			}
			return undiscovered_directions;
		}

		ai::Scavenger::Location::Direction SecretAgentModel::getNextUndiscoveredDirection(double x, double y) {
			ai::Scavenger::Location::Direction direction = ai::Scavenger::Location::NORTH;
			CellData * existing_cell = getCell(x, y);
			std::string interface_north = existing_cell->getCellNorth();
			std::string interface_south = existing_cell->getCellSouth();
			std::string interface_east = existing_cell->getCellEast();
			std::string interface_west = existing_cell->getCellWest();
			if(interface_north == "")
			{
				direction = ai::Scavenger::Location::NORTH;
			}
			else if(interface_south == "")
			{
				direction = ai::Scavenger::Location::SOUTH;
			}
			else if(interface_east == "")
			{
				direction = ai::Scavenger::Location::EAST;
			}
			else if(interface_west == "")
			{
				direction = ai::Scavenger::Location::WEST;
			}
			return direction;
		}

		void SecretAgentModel::updateLookDirection(int d) {
			direction = d;
		}

		int SecretAgentModel::getLookDirection() const {
			return direction;
		}

		bool SecretAgentModel::chargeAgent() const {
			if (getCurrX() == 0.0 && getCurrY() == 0.0 && getCharge() <= 95.0){
				return true;
			} else {
				return false;
			}
		}

		cwp::Scavenger::Object* SecretAgentModel::getObject(std::string id){
			cwp::Scavenger::Object * object = new cwp::Scavenger::Object(id);
			std::pair<std::map<std::string, cwp::Scavenger::Object*>::iterator, bool> existing_object;
			existing_object = objects_found.insert(std::pair<std::string, cwp::Scavenger::Object*>(id, object));
			return existing_object.first->second;
		}

		void SecretAgentModel::updateObjectForAction(std::string object_id){
			this->object_for_action = object_id;
		}

		void SecretAgentModel::updateActionForObject(int action){
			this->action_for_object = action;
		}

		std::string SecretAgentModel::getObjectForAction(){
			return object_for_action;
		}

		int SecretAgentModel::getActionForObject(){
			return action_for_object;
		}

		void SecretAgentModel::updateHopperStatus(bool full){
			this->hopper_full = full;
		}

		bool SecretAgentModel::hopperFull() const {
			return hopper_full;
		}

		bool SecretAgentModel::depositObjects() const{
			if (getCurrX() == 0.0 && getCurrY() == 0.0){
				std::map<std::string, cwp::Scavenger::Object*>::const_iterator it;
				for (it = objects_found.begin(); it != objects_found.end(); it++){
					if (it->second->isInHopper()){
						return true;
					}
				}
				return false;
			} else {
				return false;
			}
		}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

		CellData::CellData(){
			this->visited = false;
			this->north = "";
			this->south = "";
			this->east = "";
			this->west = "";
			this->id = "";
			this->loc_x = -1.0;
			this->loc_y = -1.0;
			this->loc_z = -1.0;
		}

		void CellData::updateId(std::string new_id){
			this->id = new_id;
		}

		std::string CellData::getId(){
			return id;
		}

		void CellData::updateCellLocation(double x, double y, double z){
			this->loc_x = x;
			this->loc_y = y;
			this->loc_z = z;
		}

		double CellData::getLocX() const {
			return loc_x;
		}

		double CellData::getLocY() const {
			return loc_y;
		}

		double CellData::getLocZ() const {
			return loc_z;
		}

		void CellData::updateCellNorth(std::string n){
			this->north = n;
		}

		void CellData::updateCellSouth(std::string s){
			this->south = s;
		}

		void CellData::updateCellEast(std::string e){
			this->east = e;
		}

		void CellData::updateCellWest(std::string w){
			this->west = w;
		}

		std::string CellData::getCellNorth() const {
			return north;
		}

		std::string CellData::getCellSouth() const {
			return south;
		}

		std::string CellData::getCellEast() const {
			return east;
		}

		std::string CellData::getCellWest() const {
			return west;
		}

		bool CellData::isVisited() const {
			return visited;
		}

		void CellData::markVisited() {
			this->visited = true;
		}

		bool CellData::isPassable() const {
			bool passable = false;
			if (north == "plain" || north == "mud" || north == "rocks"){
				passable = true;
			}else if (east == "plain" || east == "mud" || east == "rocks"){
				passable = true;
			}else if (west == "plain" || west == "mud" || west == "rocks"){
				passable = true;
			}else if (south == "plain" || south == "mud" || south == "rocks"){
				passable = true;
			}
			return passable;
		}

		bool CellData::isSafe() const {
			bool safe = false;
			if (north == "plain" || north == "mud"){
				safe = true;
			}else if (east == "plain" || east == "mud"){
				safe = true;
			}else if (west == "plain" || west == "mud"){
				safe = true;
			}else if (south == "plain" || south == "mud"){
				safe = true;
			}
			return safe;
		}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

		CellKey::CellKey(double x, double y){
			this->x = x;
			this->y = y;
		}

		void CellKey::updateX(double x){
			this->x = x;
		}

		void CellKey::updateY(double y){
			this->y = y;
		}

		double CellKey::getX() const {
			return x;
		}

		double CellKey::getY() const {
			return y;
		}

		bool CellKey::operator==(CellKey rkey) const {
			if (fabs(this->getX() - rkey.getX()) < 0.00001 && fabs(this->getY() - rkey.getY()) < 0.00001){
				return true;
			}else{
				return false;
			}
		}

		bool CellKey::operator<(CellKey rkey) const {
			if (fabs(this->getX() - rkey.getX()) < 0.00001 && fabs(this->getY() - rkey.getY()) < 0.00001){
				return false;
			}
			if (this->getX() < rkey.getX()){
				return true;
			}else if(rkey.getX() < this->getX()){
				return false;
			}else{
				if (this->getY() < rkey.getY()){
					return true;
				}else{
					return false;
				}
			}
		}

		std::ostream& operator<<(std::ostream& os, CellKey rkey) {
			os << "CellKey ->" << "\nX -> " << rkey.getX() << "\nY -> " << rkey.getY();
			return os;
		}

		std::ostream& operator<<(std::ostream& os, cwp::Scavenger::Object * object) {
			os << "Ojbect ->" << "\n ID -> " << object->getId() << "\ncolor -> " << object->getAttribute("color");
			os << "\nshape -> " << object->getAttribute("shape") << "\nsize -> " << object->getAttribute("size");
			os << "\nexamined -> " << object->isExamined() << "\npicked up -> " << object->isInHopper();
			os << "\ndeposited -> " << object->isDeposited();
			return os;
		}

		std::ostream& operator<<(std::ostream& os, CellData* cell) {
			os << "Cell ->";
			os << "\nX -> " << cell->getLocX() << "\nY -> " << cell->getLocY() << "\nZ -> " << cell->getLocZ();
			os << "\nNorth - > " << cell->getCellNorth() << "\nSouth -> " << cell->getCellSouth();
			os << "\nEast -> " << cell->getCellEast() << "\nWest -> " << cell->getCellWest();
			return os;
		}
	}
}