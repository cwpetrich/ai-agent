#ifndef _RECTACTION_H_
#define _RECTACTION_H_
#include "ai_search.h"

namespace cgl
{
  class RectAction : public ai::Search::Action
  {
    public:
    RectAction(const int p, const int r, const int f, const int x, const int y);
    virtual ~RectAction();
    virtual void Display() const;
    int GetPiece() const;
    int GetRotation() const;
    int GetFlip() const;
    int GetX() const;
    int GetY() const;
  protected:
    int piece;
    int rotation;
    int flip;
    int x, y;
  private:
  };
}

#endif /* _RECTACTION_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
