#pragma once

#include "cwpSecretAgentAction.h"
#include <ai_scavenger.h>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <map>
#include <queue>

namespace cwp {

	namespace Scavenger {

		class CellData {
		public:
			CellData();
			void updateId(std::string new_id);
			std::string getId() const;
			void updateCellLocation(double x, double y, double z);
			void updateLocX(double x);
			void updateLocY(double y);
			void updateLocZ(double z);
			double getLocX() const;
			double getLocY() const;
			double getLocZ() const;
			void updateCellInterfaces(std::string n, std::string s, std::string e, std::string w);
			void updateCellNorth(std::string n);
			void updateCellSouth(std::string s);
			void updateCellEast(std::string e);
			void updateCellWest(std::string w);
			std::string getCellNorth() const;
			std::string getCellSouth() const;
			std::string getCellEast() const;
			std::string getCellWest() const;
		private:
			std::string id;
			double loc_x, loc_y, loc_z;
			std::string north, south, east, west;
		};

		class CellKey {
		public:
			CellKey(double x, double y);
			void updateX(double x);
			void updateY(double y);
			bool operator==(CellKey rkey) const;
			bool operator<(CellKey rkey) const;
			double getX() const;
			double getY() const;
		private:
			double x, y;
		};

		class SecretAgentModel {
		public:
			SecretAgentModel();
			~SecretAgentModel();
			CellData* getCell(double x, double y);
			void updateCell(std::string id, double x, double y, double z, std::string north, std::string south, std::string east, std::string west);
			void updateCurrLocation(double x, double y, double z);
			double getCurrX() const;
			double getCurrY() const;
			double getCurrZ() const;
			void updateCharge(double c);
			double getCharge() const;
			void updateHitPoints(double hp);
			double getHitPoints() const;
			void updateBaseNum(int b);
			int getBaseNum() const;
			void updateGoalLocation(double x, double y, double z);
			double getGoalX() const;
			double getGoalY() const;
			double getGoalZ() const;
			void gatherData(const ai::Agent::Percept * percept);
			void addActionToGoal(cwp::Scavenger::Action * action);
			cwp::Scavenger::Action* getNextActionToGoal();
			std::queue<cwp::Scavenger::Action *> actions_to_goal;
			bool searched;
		protected:
		private:
			std::map<CellKey, CellData*> known_cells;
			int base_num;
			double curr_x, curr_y, curr_z, charge, hit_points, goal_x, goal_y, goal_z;
		};
		
	}
}