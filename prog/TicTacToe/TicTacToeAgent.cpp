#include <ctime>
#include <cstdlib>
#include <ai_agent.h>

int main(int argc, char **argv)
{
  std::srand(time(0));
  ai::Agent::AgentDriver d(argc, argv);
  d.Run();
  return 0;
}
