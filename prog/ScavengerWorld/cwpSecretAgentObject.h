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
			bool isExamined();
			bool isInHopper();
			bool isDeposited();
			void markExamined();
			void markInHopper();
			void markDeposited();
		private:
			bool examined;
			bool in_hopper;
			bool deposited;
			std::string id;
			std::string color, shape, size;
		};

	}
}