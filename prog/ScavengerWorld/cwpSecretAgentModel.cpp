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
			// std::ofstream debug_file;
			// debug_file.open("debug.txt", std::ofstream::app | std::ofstream::out);
			CellData* closest_cell = NULL;
			double min = 1000000.00;
			std::map<CellKey, CellData*>::iterator iter;
			for (iter = known_cells.begin(); iter != known_cells.end(); iter++) {
				if (!isCellVisited(iter->first.getX(), iter->first.getY())) {
					if (iter->second->isSafe()) {
						double a = fabs(iter->first.getX() - x);
						double b = fabs(iter->first.getY() - y);
						double c = sqrt(a*a + b*b);
						// if (fabs(c - min) < 0.00001){
						// 	double x_from_base = fabs(iter->first.getX());
						// 	double y_from_base = fabs(iter->first.getY());
						// 	double d_from_base = sqrt(x_from_base*x_from_base + y_from_base*y_from_base);
						// 	if (d_from_base > min){
						// 		min = d_from_base;
						// 		closest_cell = iter->second;
						// 	}
						// }
						if (c < min) {
							min = c;
							closest_cell = iter->second;
						}
					}
				}
			}
			// if (closest_cell == NULL) {
				// debug_file << "closest_cell == NULL" << std::endl;
			// 	double min = 1000000.00;
			// 	std::map<CellKey, CellData*>::iterator iter;
			// 	for (iter = known_cells.begin(); iter != known_cells.end(); iter++) {
			// 		if (!isCellVisited(iter->first.getX(), iter->first.getY())) {
			// 			if (iter->second->isPassable()) {
			// 				double a = fabs(iter->first.getX() - x);
			// 				double b = fabs(iter->first.getY() - y);
			// 				double c = sqrt(a*a + b*b);
			// 				if (c < min) {
			// 					min = c;
			// 					closest_cell = iter->second;
			// 				}
			// 			}
			// 		}
			// 	}
			// }
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
			// std::ofstream debug_file;
      // debug_file.open("debug.txt", std::ofstream::out | std::ofstream::app);
			std::stringstream ss1;

			ss1.str(percept->GetAtom("BASE").GetValue()); ss1.clear();
			ss1 >> base_num;
			ss1.str(percept->GetAtom("X_LOC").GetValue()); ss1.clear();
			ss1 >> curr_x;
			ss1.str(percept->GetAtom("Y_LOC").GetValue()); ss1.clear();
			ss1 >> curr_y;
			ss1.str(percept->GetAtom("Z_LOC").GetValue()); ss1.clear();
			ss1 >> curr_z;
			ss1.str(percept->GetAtom("CHARGE").GetValue()); ss1.clear();
			ss1 >> charge;
			ss1.str(percept->GetAtom("HP").GetValue()); ss1.clear();
			ss1 >> hit_points;

			cwp::Scavenger::CellData * current_cell = getCell(curr_x, curr_y);
			if (!current_cell->isVisited()){
				current_cell->updateCellLocation(curr_x, curr_y, curr_z);

				std::string look, examine, pickup;
				ss1.str(percept->GetAtom("LOOK").GetValue()); ss1.clear();
				ss1 >> look;

				examine = percept->GetAtom("EXAMINE").GetValue();

				if (examine != ""){
					// debug_file << examine << std::endl;
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

				pickup = percept->GetAtom("PICKUP").GetValue();
				ss1 >> pickup;

				if (look != ""){
					// debug_file << look << std::endl;
					cwp::Scavenger::CellData * neighbor_cell;
					switch(getLookDirection()) {
						case ai::Scavenger::Location::NORTH:
							current_cell->updateCellNorth(look);
							neighbor_cell = getCell(curr_x, curr_y + 1000);
							neighbor_cell->updateCellLocation(curr_x, curr_y + 1000, curr_z);
							neighbor_cell->updateCellSouth(look);
							break;
						case ai::Scavenger::Location::SOUTH:
							current_cell->updateCellSouth(look);
							neighbor_cell = getCell(curr_x, curr_y - 1000);
							neighbor_cell->updateCellLocation(curr_x, curr_y - 1000, curr_z);
							neighbor_cell->updateCellNorth(look);
							break;
						case ai::Scavenger::Location::EAST:
							current_cell->updateCellEast(look);
							neighbor_cell = getCell(curr_x + 1000, curr_y);
							neighbor_cell->updateCellLocation(curr_x + 1000, curr_y, curr_z);
							neighbor_cell->updateCellWest(look);
							break;
						case ai::Scavenger::Location::WEST:
							current_cell->updateCellWest(look);
							neighbor_cell = getCell(curr_x - 1000, curr_y);
							neighbor_cell->updateCellLocation(curr_x - 1000, curr_y, curr_z);
							neighbor_cell->updateCellEast(look);
							break;
					}
				}

				if (pickup == "Full"){
					// debug_file << "PICKUP -> " << pickup << std::endl;
					hopper_full = true;
				} else {
					// debug_file << "PICKUP -> " << pickup << std::endl;
					hopper_full = false;
				}
			}

			std::list<std::string> tmp_objects_in_cell;
      std::list<std::string> tmp_objects_in_hopper;
			for (uint i = 0; i < percept->NumAtom(); i++){
				std::string object_name;
				std::string object_id;
				if (percept->GetAtom(i).GetName().substr(0, 7) == "OBJECT_"){
					object_name = percept->GetAtom(i).GetName();
					ss1.str(percept->GetAtom(object_name).GetValue()); ss1.clear();
					ss1 >> object_id;
					tmp_objects_in_cell.push_back(object_id);
				}
				if (percept->GetAtom(i).GetName().substr(0, 7) == "HOPPER_"){
					object_name = percept->GetAtom(i).GetName();
					ss1.str(percept->GetAtom(object_name).GetValue()); ss1.clear();
					ss1 >> object_id;
					tmp_objects_in_hopper.push_back(object_id);
				}
			}

			std::swap(tmp_objects_in_cell, objects_in_cell);
			std::swap(tmp_objects_in_hopper, objects_in_hopper);

			if(!Look() && !Examine() && !Pickup()){
				current_cell->markVisited();
			}
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

		ai::Scavenger::Location::Direction SecretAgentModel::getNextUndiscoveredDirection(double x, double y) {
			ai::Scavenger::Location::Direction direction = ai::Scavenger::Location::NORTH;
			CellData * existing_cell = getCell(x, y);
			if(existing_cell->getCellNorth() == "")
			{
				direction = ai::Scavenger::Location::NORTH;
			}
			else if(existing_cell->getCellSouth() == "")
			{
				direction = ai::Scavenger::Location::SOUTH;
			}
			else if(existing_cell->getCellEast() == "")
			{
				direction = ai::Scavenger::Location::EAST;
			}
			else if(existing_cell->getCellWest() == "")
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

		cwp::Scavenger::Object* SecretAgentModel::getObject(std::string id){
			cwp::Scavenger::Object * object = new cwp::Scavenger::Object(id);
			std::pair<std::map<std::string, cwp::Scavenger::Object*>::iterator, bool> existing_object;
			existing_object = objects_found.insert(std::pair<std::string, cwp::Scavenger::Object*>(id, object));
			return existing_object.first->second;
		}

		bool SecretAgentModel::hopperIsFull() {
			return hopper_full;
		}

		std::string SecretAgentModel::getObjectToDeposit() {
			std::string object_to_deposit = "";
			std::list<std::string>::const_iterator it;
			for (it = objects_in_hopper.begin(); it != objects_in_hopper.end(); it++){
				return *it;
			}
			return object_to_deposit;
		}

		std::string SecretAgentModel::getObjectToExamine() {
			std::string object_to_examine = "";
			std::list<std::string>::const_iterator it;
			for (it = objects_in_cell.begin(); it != objects_in_cell.end(); it++){
				cwp::Scavenger::Object * object = getObject(*it);
				if (object->getAttribute("color") == "" || object->getAttribute("size") == "" || object->getAttribute("shape") == "") {
					return object->getId();
				}
			}
			return object_to_examine;
		}

		std::string SecretAgentModel::getObjectToPickup() {
			std::string object_to_pickup = "";
			std::list<std::string>::const_iterator it;
			for (it = objects_in_cell.begin(); it != objects_in_cell.end(); it++){
				cwp::Scavenger::Object * object = getObject(*it);
				if ((object->getAttribute("color") == "dayglo-orange" && object->getAttribute("size") == "medium" && object->getAttribute("shape") == "cylinder") || (object->getAttribute("color") == "orchid" && object->getAttribute("size") == "tiny" && object->getAttribute("shape") == "quadruped")){
					return object->getId();
				}
			}
			return object_to_pickup;
		}

		bool SecretAgentModel::Charge() {
			if (getCurrX() == 0.0 && getCurrY() == 0.0 && getCharge() <= 95.0){
				return true;
			} else {
				return false;
			}
		}

		bool SecretAgentModel::Look() {
			bool look = false;
			cwp::Scavenger::CellData * cell = getCell(curr_x, curr_y);
			if (cell->getCellNorth() == "" || cell->getCellSouth() == "" || cell->getCellEast() == "" || cell->getCellWest() == ""){
				look = true;
			}
			return look;
		}

		bool SecretAgentModel::Examine() {
			std::list<std::string>::const_iterator it;
			for (it = objects_in_cell.begin(); it != objects_in_cell.end(); it++){
				cwp::Scavenger::Object * object = getObject(*it);
				if (object->getAttribute("color") == "" || object->getAttribute("size") == "" || object->getAttribute("shape") == "") {
					return true;
				}
			}
			return false;
		}

		bool SecretAgentModel::Pickup() {
			std::list<std::string>::const_iterator it;
			for (it = objects_in_cell.begin(); it != objects_in_cell.end(); it++){
				cwp::Scavenger::Object * object = getObject(*it);
				if ((object->getAttribute("color") == "dayglo-orange" && object->getAttribute("size") == "medium" && object->getAttribute("shape") == "cylinder") || (object->getAttribute("color") == "orchid" && object->getAttribute("size") == "tiny" && object->getAttribute("shape") == "quadruped")){
					return true;
				}
			}
			return false;
		}

		bool SecretAgentModel::Deposit() {
			if (getCurrX() == 0.0 && getCurrY() == 0.0 && !objects_in_hopper.empty()){
				return true;
			} else {
				return false;
			}
		}

		bool SecretAgentModel::Move() {
			if (!actions_to_goal.empty()){
				return true;
			} else {
				return false;
			}
		}

		bool SecretAgentModel::SearchCell() {
			cwp::Scavenger::CellData * cell = getCell(curr_x, curr_y);
			if (actions_to_goal.empty() && cell->isVisited()){
				return true;
			} else {
				return false;
			}
		}

		bool SecretAgentModel::SearchBase() {
			if (hopperIsFull() && getCurrX() != 0.0 && getCurrY() != 0.0){
				return true;
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