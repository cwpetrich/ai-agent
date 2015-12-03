#include "cwpSecretAgentObject.h"

namespace cwp {
	namespace Scavenger {

		Object::Object(){
			this->examined = false;
			this->picked_up = false;
			this->deposited = false;
		}

		Object::Object(std::string id){
			this->id = id;
			this->examined = false;
			this->picked_up = false;
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

		bool Object::wasDeposited(){
			return this->deposited;
		}

		void Object::markPickedUp(){
			this->picked_up = true;
		}

		bool Object::wasPickedUp(){
			return this->picked_up;
		}

		void Object::markDeposited(){
			this->deposited = true;
		}

		bool Object::wasExamined(){
			return this->examined;
		}

		void Object::markExamined(){
			this->examined = true;
		}

	}
}