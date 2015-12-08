#include "cwpSecretAgentObject.h"

namespace cwp {
	namespace Scavenger {

		Object::Object(){
			this->examined = false;
			this->in_hopper = false;
			this->deposited = false;
		}

		Object::Object(std::string id){
			this->id = id;
			this->examined = false;
			this->in_hopper = false;
			this->deposited = false;
		}

		void Object::updateId(std::string id){
			this->id = id;
		}

		std::string Object::getId(){
			return this->id;
		}

		void Object::updateAttributes(std::string color, std::string shape, std::string size){
			this->color = color;
			this->shape = shape;
			this->size = size;
		}

		std::string Object::getAttribute(std::string attr_name){
			if (attr_name == "color"){
				return this->color;				
			} else if (attr_name == "shape"){
				return this->shape;
			} else if (attr_name == "size"){
				return this->size;
			} else {
				std::string response = attr_name + " doesn't exist";
				return response;
			}
		}

		void Object::markDeposited(){
			this->deposited = true;
		}

		bool Object::isDeposited(){
			return this->deposited;
		}

		void Object::markInHopper(){
			this->in_hopper = true;
		}

		bool Object::isInHopper(){
			return this->in_hopper;
		}

		void Object::markExamined(){
			this->examined = true;
		}

		bool Object::isExamined(){
			return this->examined;
		}

	}
}