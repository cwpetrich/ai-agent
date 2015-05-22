#include <iostream>
#include "NumbersState.h"
#include "NumbersAction.h"

namespace ai
{
  namespace Sample
  {
    namespace Numbers
    {
      State::State()
      {
        /* empty */
      }
      
      State::State(const State &rhs)
      {
        *this = rhs;
      }
      
      State::~State()
      {
        /* empty */
      }
      
      State &State::operator=(const State &rhs)
      {
        std::vector<double>::const_iterator vit;
        std::set<double>::const_iterator sit;

        this->number_stack.clear();
        this->unused_numbers.clear();
        
        for(vit = rhs.number_stack.begin();
            vit != rhs.
number_stack.end();
            vit++)
          {
            this->number_stack.push_back(*vit);
          }
        
        for(sit = rhs.unused_numbers.begin();
            sit != rhs.unused_numbers.end();
            sit++)
          {
            this->unused_numbers.insert(*sit);
          }

        return *this;
      }
      
      bool State::operator==(const State &rhs) const
      {
        int i;
        std::set<double>::const_iterator sit;
        
        if(this->number_stack.size() != rhs.number_stack.size())
          {
            return false;
          }
        for(i = 0; i < (int)this->number_stack.size(); i++)
          {
            if(this->number_stack[i] != rhs.number_stack[i])
              {
                return false;
              }
          }

        if(this->unused_numbers.size() != rhs.unused_numbers.size())
          {
            return false;
          }
        for(sit = this->unused_numbers.begin();
            sit != this->unused_numbers.end();
            sit++)
          {
            if(rhs.unused_numbers.find(*sit) == rhs.unused_numbers.end())
              {
                return false;
              }
          }
        
        return true;
      }
      
      void State::Display() const
      {
        std::vector<double>::const_iterator vit;
        std::set<double>::const_iterator sit;
        
        std::cout << "Stack: ";
        for(vit = this->number_stack.begin();
            vit != this->number_stack.end();
            vit++)
          {
            std::cout << *vit << " ";
          }
        
        std::cout << "  Unused: ";
        for(sit = this->unused_numbers.begin();
            sit != this->unused_numbers.end();
            sit++)
          {
            std::cout << *sit << " ";
          }
        std::cout << std::endl;
      }
      
      bool State::IsEqual(const ai::Search::State * const state_in) const
      {
        const State * const state = dynamic_cast<const State * const>(state_in);
        return (*this) == (*state);
      }
      
      std::vector<double> &State::GetStack()
      {
        return this->number_stack;
      }
      const std::vector<double> &State::GetStack() const
      {
        return this->number_stack;
      }
      
      std::set<double> &State::GetUnused()
      {
        return this->unused_numbers;
      }

      const std::set<double> &State::GetUnused() const
      {
        return this->unused_numbers;
      }

      /* adds new_number to the top of the stack,
       * and removes it from the set */
      bool State::PushStack(const double &new_number)
      {
        if(this->unused_numbers.erase(new_number) == 0)
          { /* number not available */
            return false;
          }
        this->number_stack.push_back(new_number);
        return true;
      }
      
      /* adds new_number to the set */
      bool State::AddNumber(const double &new_number)
      {
        std::pair<std::set<double>::iterator,bool> ret;
        ret = this->unused_numbers.insert(new_number);
        return ret.second;
      }

      /* applies new_operator to top values of stack,
       * removing them, and replacing with the result */
      bool State::PushOperator(const int &new_operator)
      {
        if(this->number_stack.size() < 2)
          {
            return false;
          }
        double rhs = this->number_stack.back();
        this->number_stack.pop_back();
        double lhs = this->number_stack.back();
        this->number_stack.pop_back();
        double new_number = 0.0;
        switch(new_operator)
          {
          case Action::O_PLUS:
            new_number = lhs + rhs;
            break;
          case Action::O_MINUS:
            new_number = lhs - rhs;
            break;
          case Action::O_TIMES:
            new_number = lhs * rhs;
            break;
          case Action::O_DIVIDE:
            new_number = lhs / rhs;
            break;
          default:
            return false;
          }
        this->number_stack.push_back(new_number);
        return true;
      }

    }
  }
}

/* Local Variables: */
/* mode:c++         */
/* End:             */
