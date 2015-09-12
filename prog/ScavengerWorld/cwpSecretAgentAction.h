#pragma once

#include <ai_search.h>

namespace cwp {
	namespace Scavenger {

		class Action : public ai::Search::Action
		{
		public:
			Action();
			Action(int a);
			void updateAction(int a);
			int getAction();
		protected:
			int action;
		};

	}
}