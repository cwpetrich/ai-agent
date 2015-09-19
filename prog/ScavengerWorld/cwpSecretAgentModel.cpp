#include "cwpSecretAgentModel.h"
#include <iostream>
#include <fstream>

namespace cwp {

	namespace Scavenger {

		SecretAgentModel::SecretAgentModel(){}

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