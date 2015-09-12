#include "cwpSecretAgentState.h"
#include <cmath>

namespace cwp {
	namespace Scavenger {

		State::State(){}

		// State::State(double x, double y){
		// 	this->x = x;
		// 	this->y = y;
		// }

		State::State(double x, double y, double c){
			this->x = x;
			this->y = y;
			this->charge = c;
		}

		State::~State(){}

		// void State::updateXandY(double x, double y) const {
		// 	this->x = x;
		// 	this->y = y;
		// }

		// void State::updateX(double x) const {
		// 	this->x = x;
		// }

		// void State::updateY(double y) const {
		// 	this->y = y;
		// }

		// void State::updateCharge(double c) const {
		// 	this->charge = c;
		// }

		double State::getX() const {
			return x;
		}

		double State::getY() const {
			return y;
		}

		double State::getCharge() const {
			return charge;
		}

    bool State::IsEqual(const ai::Search::State * const state_in) const {
      const State * const state = dynamic_cast<const State * const>(state_in);
      return (*this) == (*state);
    }

		bool State::IsLessThan(const ai::Search::State * const state_in) const {
			const State * const state = dynamic_cast<const State * const>(state_in);
			return (*this) < (*state);
		}

		bool State::operator==(const State rhs) const {
			if (fabs(this->getX() - rhs.getX()) < 0.00001 && fabs(this->getY() - rhs.getY()) < 0.00001){
				return true;
			}else{
				return false;
			}
		}

		bool State::operator<(const State rhs) const {
			if (this->getCharge() < rhs.getCharge()){
				return true;
			}else{
				return false;
			}
		}
	}
}