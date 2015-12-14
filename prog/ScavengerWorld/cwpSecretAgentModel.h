#pragma once

#include "cwpSecretAgentAction.h"
#include "cwpSecretAgentState.h"
#include "cwpSecretAgentObject.h"
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
		enum AgentMode {
			RETURN, SEARCH, LOOK, EXAMINE, PICKUP, DEPOSIT, CHARGE
		};

		class CellData {
		public:
			CellData();
			void updateId(std::string new_id);
			std::string getId();
			void updateCellLocation(double x, double y, double z);
			double getLocX() const;
			double getLocY() const;
			double getLocZ() const;
			void updateCellNorth(std::string n);
			void updateCellSouth(std::string s);
			void updateCellEast(std::string e);
			void updateCellWest(std::string w);
			std::string getCellNorth() const;
			std::string getCellSouth() const;
			std::string getCellEast() const;
			std::string getCellWest() const;
			void markVisited();
			bool isVisited() const;
			bool isPassable() const;
			bool isSafe() const;
			bool isBase() const;

		private:
			bool visited;
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
			CellData* getClosestUnvisitedCell(double x, double y);
			bool isCellVisited(double x, double y);
			double getCurrX() const;
			double getCurrY() const;
			double getCurrZ() const;
			double getCharge() const;
			double getHitPoints() const;
			int getBaseNum() const;
			void updateGoalLocation(double x, double y, double z);
			double getGoalX() const;
			double getGoalY() const;
			double getGoalZ() const;
			void gatherData(const ai::Agent::Percept * percept);
			void addActionToGoal(cwp::Scavenger::Action * action);
			cwp::Scavenger::Action* getNextActionToGoal();
			bool actionQueueEmpty();
			ai::Scavenger::Location::Direction getNextUndiscoveredDirection(double x, double y);
			cwp::Scavenger::Object* getObject(std::string id);
			void updateLookDirection(int d);
			int getLookDirection() const;
			bool hopperIsFull();
			std::string getObjectToExamine();
			std::string getObjectToPickup();
			std::string getObjectToDeposit();
			bool Charge();
			bool Look();
			bool Examine();
			bool Pickup();
			bool Deposit();
			bool Move();
			bool SearchCell();
			bool SearchBase();

		protected:
		private:
			int direction;
			bool hopper_full;
			std::list<std::string> objects_in_cell;
			std::list<std::string> objects_in_hopper;
			std::map<std::string, cwp::Scavenger::Object *> objects_found;
			std::queue<cwp::Scavenger::Action *> actions_to_goal;
			std::map<CellKey, CellData*> known_cells;
			int base_num;
			double curr_x, curr_y, curr_z, charge, hit_points, goal_x, goal_y, goal_z;
		};

		std::ostream& operator<<(std::ostream& os, CellData* cell);
		std::ostream& operator<<(std::ostream& os, CellKey key);
		std::ostream& operator<<(std::ostream& os, cwp::Scavenger::Object * object);
		std::ostream& operator<<(std::ostream& os, cwp::Scavenger::State * state);
		std::ostream& operator<<(std::ostream& os, cwp::Scavenger::Action * action);
		
	}
}