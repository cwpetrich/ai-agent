#ifndef _AISAMPLERANDOM_H_
#define _AISAMPLERANDOM_H_
#include <ai_mancala.h>
namespace ai
{
  namespace Sample
  {
    class Random : public ai::Agent::AgentProgram
    {
    public:
      Random(ai::Agent::Options *opts);
      ~Random();
      virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
    protected:
    private:
    };
  }
}
#endif /* _AISAMPLERANDOM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
