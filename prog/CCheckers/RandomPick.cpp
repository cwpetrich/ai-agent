#include "RandomPick.h"

namespace ai
{
  namespace Sample
  {

    RandomPick::RandomPick()
    {
      SetName("RandomPick");
    }

    RandomPick::~RandomPick()
    {
    }

    ai::Agent::Action * RandomPick::Program(const ai::Agent::Percept * percept)
    {
      ai::CCheckers::Action *action = new ai::CCheckers::Action;
      ai::CCheckers::MoveData move;

      std::string board_str = percept->GetAtom("BASIC_BOARD").GetValue();
      ai::CCheckers::BasicBoard board(board_str);

      int player = atoi(percept->GetAtom("PLAYER_NUMBER").GetValue().c_str());

      const std::vector<ai::CCheckers::MoveData> & moves = board.DetermineLegalMoves(player);

      move = moves[rand() % moves.size()];

      action->SetMove(move);
      action->SetCode(ai::CCheckers::Action::MOVE);

      return action;
    }
  }
}
