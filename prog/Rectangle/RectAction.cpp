#include <iostream>
#include "RectAction.h"

namespace cgl
{
  RectAction::RectAction(const int p, const int r, const int f, const int x_in, const int y_in)
    : piece(p), rotation(r), flip(f), x(x_in), y(y_in)
  {
  }
  RectAction::~RectAction()
  {
  }
  void RectAction::Display() const
  {
    std::cout << "Place "
	      << "piece(" << this->piece << ") "
	      << "rotation(" << this->rotation << ") "
	      << "flip(" << this->flip << ") "
	      << "x,y(" << this->x << "," << this->y << ") "
	      << std::endl;
  }
  int RectAction::GetPiece() const
  {
    return this->piece;
  }
  int RectAction::GetRotation() const
  {
    return this->rotation;
  }
  int RectAction::GetFlip() const
  {
    return this->flip;
  }
  int RectAction::GetX() const
  {
    return this->x;
  }
  int RectAction::GetY() const
  {
    return this->y;
  }
}
