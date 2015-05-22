#ifndef _RECTSTATE_H_
#define _RECTSTATE_H_
#include "ai_search.h"

namespace cgl
{
  class RectSquare
  {
  public:
    int x;
    int y;
    bool operator==(const RectSquare &rhs) const;
    bool operator!=(const RectSquare &rhs) const;
  };

  class RectPiece
  {
  public:
    RectSquare squares[5];
    int rotation;
    int flip;
    int x_orig;
    int y_orig;
    bool placed;
    int max_rotation;
    int max_flip;
    bool operator==(const RectPiece &rhs) const;
    bool operator!=(const RectPiece &rhs) const;
  };

  class RectState : public ai::Search::State
    {      
    public:
      RectState(int board_width_in, int board_height_in);
      RectState(const RectState &rhs);
      virtual ~RectState();
      virtual void Display() const;
      virtual bool IsEqual(const ai::Search::State * const state_in) const;
      bool PlacePiece(unsigned int which_piece,
		      int rotation, int flip, int x, int y);
	
      bool BoardUsable() const;
      void Transform(int x_in, int y_in, int rot_in, int flip_in,
		     int x_orig_in, int y_orig_in, 
		     int &x_out, int &y_out) const;

      int GetWidth() const;
      int GetHeight() const;

      std::vector<RectPiece> pieces;
      std::vector<std::vector<char> > board;

    protected:
      int board_width;
      int board_height;
    private:
    };

}
#endif /* _RECTSTATE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
