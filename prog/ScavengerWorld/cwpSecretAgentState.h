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