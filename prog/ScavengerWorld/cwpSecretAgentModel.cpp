#include "cwpSecretAgentModel.h"
#include <iostream>
#include <fstream>

namespace cwp {

	namespace Scavenger {

		SecretAgentModel::SecretAgentModel(){
			this->direction = -1;
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
			if(existing_cell->isVisited()){
				return true;
			}
			if(x == getCurrX() && y == getCurrY() && !isUndiscoveredDirections(x, y)){
				existing_cell->markVisited();
				return true;
			}
			return false;
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

			if (object_to_pick_up != ""){
				for (uint i = 0; i < percept->NumAtom(); i++){
					std::string atom_name;
					std::string object_id;
					if(percept->GetAtom(i).GetName().substr(0, 7) == "HOPPER_"){
						atom_name = percept->GetAtom(i).GetName();
						ss1.str(percept->GetAtom(atom_name).GetValue()); ss1.clear();
						ss1 >> object_id;
						if(object_id == object_to_pick_up){
							cwp::Scavenger::Object * object = getObject(object_id);
							object->markPickedUp();
							object_to_pick_up = "";
						}
					}
					else if(percept->GetAtom(i).GetName().substr(0, 7) == "OBJECT_"){
						atom_name = percept->GetAtom(i).GetName();
						ss1.str(percept->GetAtom(atom_name).GetValue()); ss1.clear();
						ss1 >> object_id;
						cwp::Scavenger::Object * object = getObject(object_id);
						if (object->wasExamined()){
							
						}

					}
				}
			}

			object_to_examine = "";
			for (uint i = 0; i < percept->NumAtom(); i++){
				if (percept->GetAtom(i).GetName().substr(0, 7) == "OBJECT_"){
					std::string object_name = percept->GetAtom(i).GetName();
					std::string object_id;
					ss1.str(percept->GetAtom(object_name).GetValue()); ss1.clear();
					ss1 >> object_id;

					cwp::Scavenger::Object * object = getObject(object_id);
					if (object->getId().length() == 0){
						object->updateId(object_id);
					}
					if (object->wasExamined()){
						object_to_pick_up = object_id;
					}else{
						object_to_examine = object_id;
					}
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


			cwp::Scavenger::CellData * current_cell = getCell(curr_x, curr_y);
			current_cell->updateCellLocation(curr_x, curr_y, curr_z);
			if (getLookDirection() == ai::Scavenger::Location::NORTH || getLookDirection() == ai::Scavenger::Location::EAST || getLookDirection() == ai::Scavenger::Location::WEST || getLookDirection() == ai::Scavenger::Location::SOUTH) {
				std::string interface;
				ss1.str(percept->GetAtom("LOOK").GetValue()); ss1.clear();
				ss1 >> interface;
				cwp::Scavenger::CellData * neighbor_cell;
				switch(getLookDirection()) {
					case ai::Scavenger::Location::NORTH:
					current_cell->updateCellNorth(interface);
					neighbor_cell = getCell(curr_x, curr_y + 1000);
					neighbor_cell->updateCellLocation(curr_x, curr_y + 1000, curr_z);
					neighbor_cell->updateCellSouth(interface);
					break;
					case ai::Scavenger::Location::SOUTH:
					current_cell->updateCellSouth(interface);
					neighbor_cell = getCell(curr_x, curr_y - 1000);
					neighbor_cell->updateCellLocation(curr_x, curr_y - 1000, curr_z);
					neighbor_cell->updateCellNorth(interface);
					break;
					case ai::Scavenger::Location::EAST:
					current_cell->updateCellEast(interface);
					neighbor_cell = getCell(curr_x + 1000, curr_y);
					neighbor_cell->updateCellLocation(curr_x + 1000, curr_y, curr_z);
					neighbor_cell->updateCellWest(interface);
					break;
					case ai::Scavenger::Location::WEST:
					current_cell->updateCellWest(interface);
					neighbor_cell = getCell(curr_x - 1000, curr_y);
					neighbor_cell->updateCellLocation(curr_x - 1000, curr_y, curr_z);
					neighbor_cell->updateCellEast(interface);
					break;
				}
			}

			debug_file << objects_found.size() << std::endl;
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
			CellData * existing_cell = getCell(x, y);
			std::string interface_north = existing_cell->getCellNorth();
			std::string interface_south = existing_cell->getCellSouth();
			std::string interface_east = existing_cell->getCellEast();
			std::string interface_west = existing_cell->getCellWest();
			if (interface_north != "plain" && interface_north != "mud" && interface_north != "rocks" && interface_north != "cliff" && interface_north != "wall") {
				return true;
			} else if (interface_south != "plain" && interface_south != "mud" && interface_south != "rocks" && interface_south != "cliff" && interface_south != "wall") {
				return true;
			} else if (interface_east != "plain" && interface_east != "mud" && interface_east != "rocks" && interface_east != "cliff" && interface_east != "wall") {
				return true;
			} else if (interface_west != "plain" && interface_west != "mud" && interface_west != "rocks" && interface_west != "cliff" && interface_west != "wall") {
				return true;
			} else {
				return false;	
			}
		}

		ai::Scavenger::Location::Direction SecretAgentModel::getNextUndiscoveredDirection(double x, double y) {
			ai::Scavenger::Location::Direction direction;
			CellData * existing_cell = getCell(x, y);
			std::string interface_north = existing_cell->getCellNorth();
			std::string interface_south = existing_cell->getCellSouth();
			std::string interface_east = existing_cell->getCellEast();
			std::string interface_west = existing_cell->getCellWest();
			if(interface_north != "plain" && interface_north != "mud" && interface_north != "rocks" && interface_north != "cliff" && interface_north != "wall")
			{
				direction = ai::Scavenger::Location::NORTH;
			}
			else if(interface_south != "plain" && interface_south != "mud" && interface_south != "rocks" && interface_south != "cliff" && interface_south != "wall")
			{
				direction = ai::Scavenger::Location::SOUTH;
			}
			else if(interface_east != "plain" && interface_east != "mud" && interface_east != "rocks" && interface_east != "cliff" && interface_east != "wall")
			{
				direction = ai::Scavenger::Location::EAST;
			}
			else if(interface_west != "plain" && interface_west != "mud" && interface_west != "rocks" && interface_west != "cliff" && interface_west != "wall")
			{
				direction = ai::Scavenger::Location::WEST;
			}
			else {
				direction = ai::Scavenger::Location::NORTH;
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
			cwp::Scavenger::Object * object = new cwp::Scavenger::Object();
			std::pair<std::map<std::string, cwp::Scavenger::Object*>::iterator, bool> existing_object;
			existing_object = objects_found.insert(std::pair<std::string, cwp::Scavenger::Object*>(id, object));
			return existing_object.first->second;
		}

		std::string SecretAgentModel::getNextObjectToExamine(){
			std::string object_id = unexamined_objects.front();
			unexamined_objects.pop();
			return object_id;
		}

		bool SecretAgentModel::unexaminedObjectsEmpty(){
			return unexamined_objects.empty();
		}

		std::string SecretAgentModel::getNextObjectToPickUp(){
			std::string object_id = objects_to_pick_up.front();
			objects_to_pick_up.pop();
			return object_id;
		}

		bool SecretAgentModel::objectsToPickUpEmtpy(){
			return objects_to_pick_up.empty();
		}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

		CellData::CellData(){
			this->visited = false;
		}

		void CellData::updateId(std::string new_id){
			this->id = new_id;
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

		std::ostream& operator<<(std::ostream& os, CellData* cell) {
			os << "CellData ->";
			os << "\nX -> " << cell->getLocX() << ", Y -> " << cell->getLocY() << ", Z -> " << cell->getLocZ();
			os << "\nNorth - > " << cell->getCellNorth() << ", South -> " << cell->getCellSouth();
			os << ", East -> " << cell->getCellEast() << ", West -> " << cell->getCellWest();
			return os;
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
			os << "\nexamined -> " << object->wasExamined() << "\npicked up -> " << object->wasPickedUp();
			os << "\ndeposited -> " << object->wasDeposited();
			return os;
		}
	}
}