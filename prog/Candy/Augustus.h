#ifndef _AUGUSTUS_H_
#define _AUGUSTUS_H_
#include <ai_candy.h>

namespace ai
{
  namespace Sample
  {
    class Augustus : public ai::Agent::AgentProgram
    {
    public:
      Augustus(ai::Agent::Options *opts);
      ~Augustus();
      virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
    protected:
    private:
    };
  }
}

#endif /* _AUGUSTUS_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
