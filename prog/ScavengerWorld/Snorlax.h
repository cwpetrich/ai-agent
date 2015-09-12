#ifndef _SNORLAX_H_
#define _SNORLAX_H_

#include <ai_scavenger.h>

namespace ai
{
  namespace Sample
  {
    class Snorlax : public ai::Agent::AgentProgram
    {
    public:
      Snorlax(ai::Agent::Options *opts);
      ~Snorlax();
      virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
    protected:
    private:
    };
  }
}

#endif /* _SNORLAX_H_ */
