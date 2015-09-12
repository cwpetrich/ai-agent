#ifndef _AISAMPLEMAXIMUM_H_
#define _AISAMPLEMAXIMUM_H_
#include <ai_mancala.h>
namespace ai
{
  namespace Sample
  {
    class Maximum : public ai::Agent::AgentProgram
    {
    public:
      Maximum(ai::Agent::Options *opts);
      ~Maximum();
      virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
    protected:
    private:
    };
  }
}
#endif /* _AISAMPLEMAXIMUM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
