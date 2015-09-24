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
      
      ai::Search::Frontier *fringe  = new ai::Search::BFFrontier;
      ai::Search::Tree *search1 = new ai::Search::Tree(problem, fringe);

      if(model->searched == false){

        if(search1->Search()) {
          std::list<ai::Search::Node *> *solution = search1->GetSolution().GetList();
          std::list<ai::Search::Node *>::const_iterator it;

          debug_file << "Actions: " << std::endl;
          for(it = solution->begin(); it != solution->end(); it++) {
            if((*it)->GetAction()) {
              (*it)->GetAction()->Display();
              cwp::Scavenger::Action* next_action = dynamic_cast<cwp::Scavenger::Action *>((*it)->GetAction());
              debug_file << next_action->getAction() << std::endl;
              model->addActionToGoal(next_action);
            }
          }
          debug_file << std::endl;
        }
        model->searched = true;
      }

      if(cwp::Scavenger::Action * next_action = model->getNextActionToGoal()){
        action->SetCode(next_action->getAction());
        debug_file << "Action: " << next_action->getAction() << std::endl;
        debug_file << "Charge: " << model->getCharge() << std::endl;
      }else{
        if(fabs(model->getCurrX() - 0.0) < 0.00001 && fabs(model->getCurrY() - 0.0) < 0.00001){
          debug_file << "Charge: " << model->getCharge() << std::endl;
          debug_file << "Hit Points: " << model->getHitPoints() << std::endl;
          debug_file << "CurrentX: " << model->getCurrX() << std::endl;
          debug_file << "CurrentY: " << model->getCurrY() << std::endl;
          debug_file << "CurrentZ: " << model->getCurrZ() << std::endl;
          action->SetCode(ai::Scavenger::Action::QUIT);
        }else{
          model->updateGoalLocation(0.0, 0.0, 0.0);
          cwp::Scavenger::State * initial_state = new cwp::Scavenger::State(model->getCurrX(), model->getCurrY(), model->getCharge());
          cwp::Scavenger::Problem * problem = new cwp::Scavenger::Problem(dynamic_cast<ai::Search::State *>(initial_state), model);
          
          ai::Search::Frontier *fringe  = new ai::Search::BFFrontier;
          ai::Search::Tree *search1 = new ai::Search::Tree(problem, fringe);
          if(search1->Search()) {
            std::list<ai::Search::Node *> *solution = search1->GetSolution().GetList();
            std::list<ai::Search::Node *>::const_iterator it;
            

            for(it = solution->begin(); it != solution->end(); it++) {
              if((*it)->GetAction()) {
                (*it)->GetAction()->Display();
                model->addActionToGoal(dynamic_cast<cwp::Scavenger::Action *>((*it)->GetAction()));
              }
            }
            debug_file << "Charge Left: " << model->getCharge() - solution->back()->GetPathCost() << std::endl;

            debug_file << "Path Cost: " << solution->back()->GetPathCost() << std::endl;
          }
        }
      }
      return action;
    }
  }
}
