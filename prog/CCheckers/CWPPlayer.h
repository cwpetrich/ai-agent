#ifndef _CWPPLAYER_H_
#define _CWPPLAYER_H_

#include <ai_ccheckers.h>

namespace ai
{
  namespace sample
  {
    class CWPPlayer : public ai::Agent::AgentProgram
    {
    public:
      CWPPlayer(ai::Agent::Options *opts);
      ~CWPPlayer();
      virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
      ai::Agent::Action * Max(const ai::Agent::Percept * percept);
    protected:
    private:
    };
  }
}

#endif /* _CWPPLAYER_H_ */
