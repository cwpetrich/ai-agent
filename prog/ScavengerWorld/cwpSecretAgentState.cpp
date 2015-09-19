#include "cwpSecretAgentState.h"
#include <cmath>

namespace cwp {
	namespace Scavenger {

		State::State(){}

		State::State(double x, double y, double c){
			this->x = x;
			this->y = y;
			this->charge = c;
		}

		State::~State(){}

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
			if (fabs(this->getX() - rhs.getX()) < 0.00001 && fabs(this->getY() - rhs.getY()) < 0.00001 && fabs(this->getCharge() - rhs.getCharge()) < 0.00001){
				return true;
			}else{
				return false;
			}
		}

		bool State::operator<(const State rhs) const {
			if (fabs(this->getX() - rhs.getX()) < 0.00001 && fabs(this->getY() - rhs.getY()) < 0.00001 && fabs(this->getCharge() - rhs.getCharge()) < 0.00001){
				return false;
			}
			if (this->getX() < rhs.getX()){
				return true;
			}else if(rhs.getX() < this->getX()){
				return false;
			}else{
				if (this->getY() < rhs.getY()){
					return true;
				}else if(rhs.getY() < this->getY()){
					return false;
				}else{
					if (this->getCharge() < rhs.getCharge()){
						return true;
					}else if(rhs.getCharge() < this->getCharge()){
						return false;
					}else{
						return false;
					}
				}
			}
		}
	}
}