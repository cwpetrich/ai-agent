#include "Augustus.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

namespace ai
{
  namespace Sample
  {
    /* For more on options look at the files:
     * ai-lib/include/Agent/Options.h
     * ai-lib/src/Agent/Options.cpp
     * al-lib/src/Agent/AgentDriver.cpp
     */
    /*
     * Run with:
     * ./RunProg ./CandyAgent -a A -U 1
     */
    Augustus::Augustus(ai::Agent::Options *opts)
    {
      SetName("Augustus");
      std::cout << "The value of the -U option is: " << opts->GetArgInt("user1") << std::endl;
    }

    Augustus::~Augustus()
    {
    }

    /*
     * Not the brightest candy taster.
     * Predicts the next flavor will be different from this
     * flavor with a random chance.
     */
    ai::Agent::Action * Augustus::Program(const ai::Agent::Percept * percept)
    {
      ai::Candy::Action *action = new ai::Candy::Action;

      int flavor = atoi(percept->GetAtom("FLAVOR").GetValue().c_str());
      std::cout << "Flavor is " << flavor << "." << std::endl;

      action->SetCode(ai::Candy::Action::PREDICT);
      if(flavor == 0)
        {
          double p = (std::rand() % 10000)/10000.;
          action->SetFlavorPrediction(1);
          action->SetProbabilityPrediction(p);
        }
      else
        {
          double p = (std::rand() % 10000)/10000.;
          action->SetFlavorPrediction(0);
          action->SetProbabilityPrediction(p);
        }

      return action;
    }
  }
}
