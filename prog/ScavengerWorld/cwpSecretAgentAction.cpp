#include "cwpSecretAgentAction.h"

namespace cwp {
	namespace Scavenger {

		Action::Action(){}

		Action::Action(int a){
			this->action = a;
		}

		void Action::updateAction(int a){
			this->action = a;
		}

		int Action::getAction(){
			return action;
		}
	}
}