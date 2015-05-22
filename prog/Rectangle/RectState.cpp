#include <iostream>
#include "RectState.h"

namespace cgl
{
	bool RectSquare::operator==(const RectSquare &rhs) const
	{
		return ((this->x == rhs.x) && (this->y == rhs.y));
	}
	bool RectSquare::operator!=(const RectSquare &rhs) const
	{
		return !(*this == rhs);
	}
	
	bool RectPiece::operator==(const RectPiece &rhs) const
	{
		unsigned int i;
		for(i = 0; i < sizeof(this->squares)/sizeof(this->squares[0]); i++)
			{
				if(this->squares[i] != rhs.squares[i]) { return false; }
			}
		if(this->rotation != rhs.rotation) { return false; }
		if(this->flip != rhs.flip) { return false; }
		
		return true;
	}
	bool RectPiece::operator!=(const RectPiece &rhs) const
	{
		return !(*this == rhs);
	}

	
  RectState::RectState(int board_width_in, int board_height_in)
    : board_width(board_width_in),
      board_height(board_height_in)
  {
    RectPiece p1;
    int i = 0;
    int x, y;
    board.resize(board_width);
    for(x = 0; x < board_width; x++)
      {
	board[x].resize(board_height);
	for(y = 0; y < board_height; y++)
	  {
	    board[x][y] = ' ';
	  }
      }
    /*  
     *   x
     *  xxx
     *	 x
     */		
    i = 0;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = -1;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = 1;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = -1;
    p1.rotation = 0;
    p1.flip     = 0;
    p1.placed   = 0;
    p1.max_rotation = 1;
    p1.max_flip     = 1;
    pieces.push_back(p1);

    /*  
     *   xxxxx 
     *	
     */		
    i = 0;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 2;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 3;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 4;
    p1.squares[i++].y = 0;
    p1.rotation = 0;
    p1.flip     = 0;
    p1.placed   = 0;
    p1.max_rotation = 2;
    p1.max_flip     = 1;
    pieces.push_back(p1);

    /*  
     *  x
     *  xx
     *   xx
     */		
    i = 0;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = 1;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = 1;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = 2;
    p1.squares[i].x   = 2;
    p1.squares[i++].y = 2;
    p1.rotation = 0;
    p1.flip     = 0;
    p1.placed   = 0;
    p1.max_rotation = 4;
    p1.max_flip     = 1;

    pieces.push_back(p1);


    /*   x
     *   x
     *	xxx
     */
    i = 0;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 2;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = -1;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = -2;
    p1.rotation = 0;
    p1.flip     = 0;
    p1.placed   = 0;
    p1.max_rotation = 4;
    p1.max_flip     = 1;

    pieces.push_back(p1);



    /*    xx
     *    x
     *	 xx
     */
    i = 0;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = -1;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = -2;
    p1.squares[i].x   = 2;
    p1.squares[i++].y = -2;
    p1.rotation = 0;
    p1.flip     = 0;
    p1.placed   = 0;
    p1.max_rotation = 2;
    p1.max_flip     = 2;
    pieces.push_back(p1);

    /*   x x
     *   xxx 
     *	 
     */
    i = 0;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = -1;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = -1;
    p1.squares[i].x   = -1;
    p1.squares[i++].y = -1;
    p1.rotation = 0;
    p1.flip     = 0;
    p1.placed   = 0;
    p1.max_rotation = 4;
    p1.max_flip     = 1;

    pieces.push_back(p1);

    /*  xxx
     *  x 
     *	x
     */		
    i = 0;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 2;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = -1;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = -2;
    p1.rotation = 0;
    p1.flip     = 0;
    p1.placed   = 0;
    p1.max_rotation = 4;
    p1.max_flip     = 1;

    pieces.push_back(p1);


    /*    xx
     *	xxx
     */
    i = 0;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = -1;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = -2;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = -2;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = -3;
    p1.rotation = 0;
    p1.flip     = 0;
    p1.placed   = 0;
    p1.max_rotation = 4;
    p1.max_flip     = 2;

    pieces.push_back(p1);


    /*   x
     *   xxxx 
     *	
     */		
    i = 0;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = 1;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 2;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 3;
    p1.squares[i++].y = 0;
    p1.rotation = 0;
    p1.flip     = 0;
    p1.placed   = 0;
    p1.max_rotation = 4;
    p1.max_flip     = 2;

    pieces.push_back(p1);



    /*   x
     *   x
     *   xx
     *	 x
     */		
    i = 0;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = 1;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = 2;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = 3;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = 2;
    p1.rotation = 0;
    p1.flip     = 0;
    p1.placed   = 0;
    p1.max_rotation = 4;
    p1.max_flip     = 2;

    pieces.push_back(p1);


    /*  
     *  x
     *  xx
     *  xx
     */		
    i = 0;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = 1;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = 2;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = 1;
    p1.rotation = 0;
    p1.flip     = 0;
    p1.placed   = 0;
    p1.max_rotation = 4;
    p1.max_flip     = 2;

    pieces.push_back(p1);


    /*  
     *  x
     *  xx
     * xx
     */		
    i = 0;
    p1.squares[i].x   = 0;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = 0;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = 1;
    p1.squares[i].x   = 1;
    p1.squares[i++].y = 2;
    p1.squares[i].x   = 2;
    p1.squares[i++].y = 1;
    p1.rotation = 0;
    p1.flip     = 0;
    p1.placed   = 0;
    p1.max_rotation = 4;
    p1.max_flip     = 2;

    pieces.push_back(p1);
  }
  RectState::RectState(const RectState &rhs)
    : board_width(rhs.board_width),
      board_height(rhs.board_height)
  {
    unsigned int i;
    for(i = 0; i < rhs.pieces.size(); i ++)
      {
	pieces.push_back(rhs.pieces[i]);
      }
    int x, y;
    board.resize(board_width);
    for(x = 0; x < board_width; x++)
      {
	board[x].resize(board_height);
	for(y = 0; y < board_height; y++)
	  {
	    board[x][y] = rhs.board[x][y];
	  }
      }
  }
  RectState::~RectState()
  {
  }

  void RectState::Display() const
  {
    int x, y;

    std::cout << std::endl;
    std::cout << "+------+" << std::endl;
    for(y = 0; y < board_height; y++)
      {
	std::cout << "|";
	for(x = 0; x < board_width; x ++)
	  {
	    std::cout << board[x][y];
	  }
	std::cout << "|" << std::endl;
      }
    std::cout << "+------+" << std::endl;
  }

  bool RectState::IsEqual(const ai::Search::State * const state_in) const
  {
    const RectState * const rhs = dynamic_cast<const RectState * const>(state_in);
    
    unsigned int i;
    for(i = 0; i < this->pieces.size(); i ++)
      {
	if(this->pieces[i] != rhs->pieces[i])
	  {
	    return false;
	  }
      }
    return true;
  }

  bool RectState::PlacePiece(unsigned int which_piece,
			     int rotation, int flip, int x, int y)
  {
    unsigned int i;
    int x1, y1;
    for(i = 0; i < 5; i++) // loop over squares in the piece to be placed
      {
	Transform(pieces[which_piece].squares[i].x,
		  pieces[which_piece].squares[i].y,	
		  rotation, flip, 
		  x, y,
		  x1, y1);

	if(x1 < 0 || x1 >= board_width || y1 < 0 || y1 >= board_height)
	  {
	    return false;
	  }
	if(board[x1][y1] != ' ') 
	  { 
	    return false; 
	  }
      }
    pieces[which_piece].x_orig   = x;
    pieces[which_piece].y_orig   = y;
    pieces[which_piece].rotation = rotation;
    pieces[which_piece].flip     = flip;
    pieces[which_piece].placed   = true;

    for(i = 0; i < 5; i++)
      {	
	Transform(pieces[which_piece].squares[i].x,
		  pieces[which_piece].squares[i].y,	
		  pieces[which_piece].rotation, pieces[which_piece].flip, 
		  pieces[which_piece].x_orig, pieces[which_piece].y_orig,
		  x1, y1);		
	board[x1][y1] = 'a' + which_piece;
      }
	
    return BoardUsable();
  }

  /* don't use a board that has holes with no neighbors */
  bool RectState::BoardUsable() const
  {
    int x, y;
    int xn[4] = { 1, -1, 0, 0 };
    int yn[4] = { 0, 0, 1, -1 };
    int i;
    int x1, y1;
    bool ok;

    for(y = 0; y < board_height; y++)
      {
	for(x = 0; x < board_width; x ++)
	  {
	    if(board[x][y] == ' ')
	      {
		ok = false;
		for(i = 0; i < 4; i++)
		  {
		    x1 = x + xn[i];
		    y1 = y + yn[i];
		    if(x1 >= 0 && x1 < board_width && y1 >= 0 && y1 < board_height)
		      {
			if(board[x1][y1] == ' ')
			  {
			    ok = true;
			    break;
			  }
		      }
		  }
		if(! ok) { return false; }
	      }
	  }
      }

    return true;
  }

  void RectState::Transform(int x_in, int y_in, int rot_in, int flip_in,
			    int x_orig_in, int y_orig_in, 
			    int &x_out, int &y_out) const
  {
    switch(rot_in)
      {
      case 0:
	x_out = x_in;
	y_out = y_in;
	break;
      case 1:
	x_out = -y_in;
	y_out = x_in;
	break;
      case 2:
	x_out = -x_in;
	y_out = -y_in;
	break;
      case 3:
	x_out = y_in;
	y_out = -x_in;
	break;
      }

    switch(flip_in)
      {
      case 0:
	break;
      case 1:
	x_out = -x_out;
	break;
      }
    x_out += x_orig_in;
    y_out += y_orig_in;
  }

  int RectState::GetWidth() const
  {
    return board_width;
  }
  int RectState::GetHeight() const
  {
    return board_height;
  }

}
