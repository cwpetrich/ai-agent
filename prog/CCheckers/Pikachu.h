#ifndef _PIKACHU_H_
#define _PIKACHU_H_

#include <ai_ccheckers.h>

namespace cgl
{
  class Pikachu : public ai::Agent::AgentProgram
  {
  public:
    Pikachu();
    ~Pikachu();
    virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
  protected:
  private:
  };
}

#endif /* _PIKACHU_H_ */
