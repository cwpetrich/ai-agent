#ifndef _DUMBO1_H_
#define _DUMBO1_H_

#include <ai_ccheckers.h>

namespace ai
{
  namespace Sample
  {
    class Dumbo1 : public ai::Agent::AgentProgram
    {
    public:
      Dumbo1();
      ~Dumbo1();
      virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
    protected:
    private:
    };
  }
}

#endif /* _DUMBO1_H_ */
