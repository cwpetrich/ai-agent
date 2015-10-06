#include "cwpSecretAgent.h"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>

namespace cwp
{
  namespace Scavanger
  {
    /* For more on options look at the files:
     * ai-lib/include/Agent/Options.h
     * ai-lib/src/Agent/Options.cpp
     * al-lib/src/Agent/AgentDriver.cpp
     */
    /*
     * Run with:
     * ./RunProg ./SA_Test -a s -U 1
     */
    SecretAgent::SecretAgent(ai::Agent::Options *opts)
    {
      SetName("SecretAgent");
      std::cout << "The value of the -U option is: " << opts->GetArgInt("user1") << std::endl;
      this->model = new cwp::Scavenger::SecretAgentModel;
      this->model->searched = false;
    }

    SecretAgent::~SecretAgent()
    {
      delete(model);
    }

    ai::Agent::Action * SecretAgent::Program(const ai::Agent::Percept * percept)
    {

      std::ofstream debug_file;
      debug_file.open("debug.txt", std::ofstream::out | std::ofstream::app);

      ai::Scavenger::Action *action = new ai::Scavenger::Action;
      model->gatherData(percept);

      cwp::Scavenger::State * initial_state = new cwp::Scavenger::State(model->getCurrX(), model->getCurrY(), model->getCharge());
      cwp::Scavenger::Problem * problem = new cwp::Scavenger::Problem(dynamic_cast<ai::Search::State *>(initial_state), model);
      
      ai::Search::Frontier *fringe  = new ai::Search::UCFrontier;
      ai::Search::Graph *search = new ai::Search::Graph(problem, fringe);

      if(model->searched == false){

        if(search->Search()) {
          std::list<ai::Search::Node *> *solution = search->GetSolution().GetList();
          std::list<ai::Search::Node *>::const_iterator it;

          // debug_file << "Actions: " << std::endl;
          for(it = solution->begin(); it != solution->end(); it++) {
            if((*it)->GetAction()) {
              // (*it)->GetAction()->Display();
              cwp::Scavenger::Action* next_action = dynamic_cast<cwp::Scavenger::Action *>((*it)->GetAction());
              // debug_file << next_action->getAction() << std::endl;
              model->addActionToGoal(next_action);
            }
          }
          // debug_file << "Path Cost: " << solution->back()->GetPathCost() << std::endl;
          // debug_file << "Nodes generated: " << search->GetNumberNodesGenerated() << std::endl;
          // debug_file << "Nodes stored:    " << search->GetMaxNodesStored() << std::endl;
          // debug_file << std::endl;
        }
        model->searched = true;
      }
      cwp::Scavenger::Action * next_action = model->getNextActionToGoal();
      if (next_action == NULL){
        action->SetCode(ai::Scavenger::Action::QUIT);
      }else{
        action->SetCode(next_action->getAction());
      }
      debug_file.close();
      return action;
    }
  }
}
