#ifndef _RANDOMPICK_H_
#define _RANDOMPICK_H_

#include <ai_ccheckers.h>

namespace ai
{
  namespace Sample
  {
    class RandomPick : public ai::Agent::AgentProgram
    {
    public:
      RandomPick();
      ~RandomPick();
      virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
    protected:
    private:
    };
  }
}

#endif /* _RANDOMPICK_H_ */
