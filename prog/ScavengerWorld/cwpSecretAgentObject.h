#include <iostream>
#include <ai_scavenger.h>

namespace cwp {
	namespace Scavenger {

		class Object {
		public:
			Object();
			Object(std::string id);
			void updateId(std::string id);
			std::string getId();
			void updateAttributes(std::string color, std::string shape, std::string size);
			std::string getAttribute(std::string attr_name);
			bool wasPickedUp();
			bool wasDeposited();
			void markPickedUp();
			void markDeposited();
			void markExamined();
			bool wasExamined();
		private:
			bool examined;
			bool picked_up;
			bool deposited;
			std::string id;
			std::string color, shape, size;
		};

	}
}