#include <iostream>
#include "NumbersAction.h"

namespace ai
{
  namespace Sample
  {
    namespace Numbers
    {
      Action::Action()
        : type(A_MIN),
          op(O_MIN),
          number(0.0)
      {
        /* empty */
      }
      
      Action::Action(const Action &rhs)
      {
        *this = rhs;
      }
      
      Action::~Action()
      {
        /* empty */
      }
      
      Action &Action::operator=(const Action &rhs)
      {
        this->type   = rhs.type;
        this->op     = rhs.op;
        this->number = rhs.number;
        return *this;
      }
      
      bool Action::operator==(const Action &rhs) const
      {
        return ((this->type == rhs.type) &&
                ((this->type == A_PUSH_NUMBER && this->number == rhs.number) ||
                 (this->type == A_PUSH_OPERATOR && this->op == rhs.op)));
      }
      
      void Action::Display() const
      {
        switch(this->type)
          {
          case A_PUSH_NUMBER:
            std::cout << "Push: " << this->number << std::endl;
            break;
          case A_PUSH_OPERATOR:
            std::cout << "Push: ";
            switch(this->op)
              {
              case O_PLUS:
                std::cout << "+";
                break;
              case O_MINUS:
                std::cout << "-";
                break;
              case O_TIMES:
                std::cout << "*";
                break;
              case O_DIVIDE:
                std::cout << "/";
                break;
              default:
                std::cout << "unknown operation";
                break;
              }
            std::cout << std::endl;
            break;
          default:
            std::cout << "Unknown action" << std::endl;
            break;
          }
      }

      bool Action::SetType(int type_in)
      {
        if(type_in > A_MIN && type_in < A_MAX)
          {
            this->type = type_in;
            return true;
          }
        return false;
      }
      
      bool Action::SetOperator(int op_in)
      {
        if(op_in > O_MIN && op_in < O_MAX)
          {
            this->op = op_in;
            return true;
          }
        return false;
      }
      
      bool Action::SetNumber(double number_in)
      {
        this->number = number_in;
        return true;
      }
      
      int Action::GetType() const
      {
        return this->type;
      }

      int Action::GetOperator() const
      {
        return this->op;
      }

      double Action::GetNumber() const
      {
        return this->number;
      }
      
      int type; /* A_* */
      int op;   /* O_* */
      double number;

    }
  }
}
