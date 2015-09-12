#ifndef _SMARTY_H_
#define _SMARTY_H_

#include <ai_ccheckers.h>

namespace ai
{
  namespace Sample
  {
    class Smarty : public ai::Agent::AgentProgram
    {
    public:
      Smarty(ai::Agent::Options *opts);
      ~Smarty();
      virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
    protected:
    private:
    };
  }
}

#endif /* _SMARTY_H_ */
