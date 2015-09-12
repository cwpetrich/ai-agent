#pragma once

#include <ai_search.h>

namespace cwp {
	namespace Scavenger {

		class State : public ai::Search::State
		{
		public:
			State();
			// State(double x, double y);
			State(double x, double y, double c);
			~State();
			bool operator<(const State rhs) const;
			bool operator==(const State rhs) const;
			// void updateXandY(double x, double y) const;
			// void updateX(double x) const;
			// void updateY(double y) const;
			// void updateCharge(double c) const;
			bool IsEqual(const ai::Search::State * const state_in) const;
			bool IsLessThan(const ai::Search::State * const state_in) const;
			double getX() const;
			double getY() const;
			double getCharge() const;
		private:
			double x, y, charge;
		};

	}
}