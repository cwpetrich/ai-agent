#include "cwpSecretAgentModel.h"
#include <iostream>
#include <fstream>

namespace cwp {

	namespace Scavenger {

		SecretAgentModel::SecretAgentModel(){
			this->searched = false;
			this->direction = -1;
		}

		SecretAgentModel::~SecretAgentModel(){}

		void SecretAgentModel::updateCurrLocation(double x, double y, double z){
			curr_x = x;
			curr_y = y;
			curr_z = z;
		}

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
			cell = known_cells[key];
			return cell;
		}

		bool SecretAgentModel::isCellVisited(double x, double y){
			CellData* cell = new CellData();
			CellKey key = CellKey(x, y);
			cell = known_cells[key];
			if (cell->isVisited()){
				return true;
			}else{
				if (cell->getCurrX() && cell->getCurrY() && cell->getCurrZ() && cell->getCellNorth() && cell->getCellEast() && cell->getCellWest() && cell->getCellSouth()){
					cell->markVisited();
					return true;
				}else{
					return false;
				}
			}
		}

		void SecretAgentModel::updateCell(std::string id, double x, double y, double z, std::string north, std::string south, std::string east, std::string west){
			CellData* cell = new CellData();
			CellKey key = CellKey(x, y);
			std::pair<std::map<CellKey, CellData*>::iterator, bool> existing_cell;
			existing_cell = known_cells.insert(std::pair<CellKey, CellData*>(key, cell));
			existing_cell.first->second->updateId(id);
			existing_cell.first->second->updateCellLocation(x, y, z);
			existing_cell.first->second->updateCellInterfaces(north, south, east, west);
		}

		void SecretAgentModel::updateCharge(double c){
			this->charge = c;
		}
		double SecretAgentModel::getCharge() const {
			return charge;
		}
		void SecretAgentModel::updateHitPoints(double hp){
			this->hit_points = hp;
		}
		double SecretAgentModel::getHitPoints() const {
			return hit_points;
		}
		void SecretAgentModel::updateBaseNum(int b){
			this->base_num = b;
		}
		int SecretAgentModel::getBaseNum() const {
			return base_num;
		}

		void SecretAgentModel::gatherData(const ai::Agent::Percept * percept){
			std::stringstream ss1;

			if (this->getLookDirection() != -1){
				ss1.str(percept->GetAtom("LOOK").GetValue()); ss1.clear();
				std::ofstream debug_file;
      	debug_file.open("debug.txt", std::ofstream::out | std::ofstream::app);
				debug_file << ss1 << std::endl;
				debug_file.close();
				this->setLookDirection(-1);
			}

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

      std::string agent_goal_loc = percept->GetAtom("GOAL_LOC").GetValue();
      ss1.str(agent_goal_loc); ss1.clear();
      ss1 >> goal_x; ss1.ignore();
      ss1 >> goal_y; ss1.ignore();
      ss1 >> goal_z; ss1.ignore();

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

	        this->updateCell(percept->GetAtom(i).GetName().substr(5), cell_x, cell_y, cell_z, cell_north, cell_south, cell_east, cell_west);
        }
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

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

		CellData::CellData(){}

		void CellData::updateId(std::string new_id){
			this->id = new_id;
		}
		std::string CellData::getId() const {
			return id;
		}
		void CellData::updateCellLocation(double x, double y, double z){
			this->loc_x = x;
			this->loc_y = y;
			this->loc_z = z;
		}
		void CellData::updateLocX(double x){
			this->loc_x = x;
		}
		void CellData::updateLocY(double y){
			this->loc_y = y;
		}
		void CellData::updateLocZ(double z){
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
		void CellData::updateCellInterfaces(std::string n, std::string s, std::string e, std::string w){
			this->north = n;
			this->south = s;
			this->east = e;
			this->west = w;
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

		bool CellData::markVisited() {
			this->visited = true;
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
	}
}