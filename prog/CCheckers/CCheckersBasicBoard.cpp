/*
 * This is included for your reference, in case you decide to
 * derive from this class.  A compiled version is available
 * in the libraries you link to.
 */
#include <ai_ccheckers.h>

#include <cstring>
#include <cstdio>

namespace ai
{
  namespace CCheckers
  {
#ifdef WIN32
    static const char *index(const char *s, int c)
    {
      while(s && *s && *s != c)
        {
          s++;
        }
      return s;
    }
#endif
    struct BoardPoint
    {
      int x;
      int y;
    };

    static const BoardPoint player_start[][10] =
      {
        // 1 (bottom)
        { {4, 0}, {4, 1},{5, 1}, {4, 2},{5, 2},{6, 2}, {4, 3},{5, 3},{6, 3},{7, 3} },
        // 2 (top)
        { {12, 16}, {11, 15},{12, 15}, {10, 14},{11, 14},{12, 14}, { 9, 13},{10, 13},{11, 13},{12, 13} },
        // 3 (bottom, left)
        { {0, 4}, {1, 5},{1, 4}, {2, 6},{2, 5},{2, 4}, {3, 7},{3, 6},{3, 5},{3, 4} },
        // 4 (top, left)
        { {4, 12}, {5, 12},{4, 11}, {6, 12},{5, 11},{4, 10}, {7, 12},{6, 11},{5, 10},{4, 9} },
        // 5 (bottom, right)
        { {12, 4}, {12, 5},{11, 4}, {12, 6},{11, 5},{10, 4}, {12, 7},{11, 6},{10, 5},{9, 4} },
        // 6 (top, right)
        { {16, 12}, {15, 11},{15, 12}, {14, 10},{14, 11},{14, 12}, {13, 9},{13, 10},{13, 11},{13, 12} }
      };
    static const int player_end_map[] =
      {
        2, // bottom to top
        1, // top to bottom
        6, // bottom,left to top,right
        5, // top,left to bottom,right
        4, // bottom,right to top,left
        3  // top,right to bottom,left
      };

    BasicBoard::BasicBoard()
      : number_of_turns(0),
        legal_moves(0),
        last_moves(6),
        player_times(6)
    {
      InitBoard();
    }

    BasicBoard::BasicBoard(const BasicBoard &board_in)
      : number_of_turns(board_in.number_of_turns),
        legal_moves_valid(board_in.legal_moves_valid),
        legal_moves(board_in.legal_moves),
        last_moves(board_in.last_moves),
        player_times(board_in.player_times)
    {
      board = board_in.board;
    }

    BasicBoard::BasicBoard(const std::string &board_str_in)
      : legal_moves_valid(false),
        legal_moves(0),
        last_moves(6),
        player_times(6)
    {
      SetBoardString(board_str_in);
    }

    BasicBoard::~BasicBoard()
    {
    }

    bool BasicBoard::Move(int player, const MoveData &move_in, double seconds_in)
    {
      if((IsLegal(player, move_in)) &&
         (player == board.player_turn))
        {
          board.hole[move_in.from_x][move_in.from_y] = 0;
          board.hole[move_in.to_x][move_in.to_y] = player;
          board.player_turn = ((board.player_turn)%board.num_players) + 1;

          last_moves[player-1]    = move_in;
          player_times[player-1] += seconds_in;

          number_of_turns ++;
          legal_moves_valid = false;
          return true;
        }
      return false;
    }
    bool BasicBoard::Move(int player, const MoveData &move_in)
    {
      return Move(player, move_in, 0.);
    }

    bool BasicBoard::IsLegal(int player, const MoveData &move_in)
    {
      DetermineLegalMoves(player);

      for(unsigned int i=0; i<legal_moves.size(); i++)
        {
          if(move_in.from_x == legal_moves[i].from_x &&
             move_in.from_y == legal_moves[i].from_y &&
             move_in.to_x == legal_moves[i].to_x &&
             move_in.to_y == legal_moves[i].to_y)
            {
              return true;
            }
        }
      return false;
    }

    bool BasicBoard::Winner(int player) const
    {
      unsigned int i = player_end_map[player-1]; // identify the player end location
      unsigned int j;
      for(j = 0; j < 10; j++)
        {
          if(board.hole[player_start[i-1][j].x][player_start[i-1][j].y] != player)
            {
              return false;
            }
        }
      return true;
    }

    int BasicBoard::HaveWinner() const
    {
      int i;
      for(i = 1; i <= 6; i++)
        {
          if(Winner(i))
            {
              //std::cout << "winner is " << i << std::endl;
              return i;
            }
        }
      return 0;
    }

    const std::vector<MoveData> & BasicBoard::DetermineLegalMoves(int player)
    {
      int try_x;
      int try_y;
      int x,y;
      MoveData d;

      if(legal_moves_valid)
        {
          return legal_moves;
        }

      legal_moves.resize(0);

      for(x=0; x<17; x++)
        {
          for(y=0; y<17; y++)
            {
              if(board.hole[x][y] == player)
                {
                  // Check all jumping directions
                  MarkAllNotVisited();
                  visited[x][y] = true;

                  RecursiveVisit(x,y, x,y);

                  // check all 6 adjacent directions

                  try_x = x;
                  try_y = y+1;
                  if(Available(try_x,try_y))
                    {
                      d.from_x = x;
                      d.from_y = y;
                      d.to_x   = try_x;
                      d.to_y   = try_y;
                      legal_moves.push_back(d);
                    }

                  try_x = x+1;
                  try_y = y+1;
                  if(Available(try_x,try_y))
                    {
                      d.from_x = x;
                      d.from_y = y;
                      d.to_x   = try_x;
                      d.to_y   = try_y;
                      legal_moves.push_back(d);
                    }

                  try_x = x-1;
                  try_y = y;
                  if(Available(try_x,try_y))
                    {
                      d.from_x = x;
                      d.from_y = y;
                      d.to_x   = try_x;
                      d.to_y   = try_y;
                      legal_moves.push_back(d);
                    }

                  try_x = x+1;
                  try_y = y;
                  if(Available(try_x,try_y))
                    {
                      d.from_x = x;
                      d.from_y = y;
                      d.to_x   = try_x;
                      d.to_y   = try_y;
                      legal_moves.push_back(d);
                    }

                  try_x = x-1;
                  try_y = y-1;
                  if(Available(try_x,try_y))
                    {
                      d.from_x = x;
                      d.from_y = y;
                      d.to_x   = try_x;
                      d.to_y   = try_y;
                      legal_moves.push_back(d);
                    }

                  try_x = x;
                  try_y = y-1;
                  if(Available(try_x,try_y))
                    {
                      d.from_x = x;
                      d.from_y = y;
                      d.to_x   = try_x;
                      d.to_y   = try_y;
                      legal_moves.push_back(d);
                    }
                }
            }
        }
      legal_moves_valid = true;
      return legal_moves;
    }

    const std::vector<MoveData> & BasicBoard::GetLastMoves() const
    {
      return last_moves;
    }

    const std::vector<double> & BasicBoard::GetPlayerTimes() const
    {
      return player_times;
    }

    unsigned int BasicBoard::GetTotalMoves() const
    {
      return number_of_turns;
    }

    const BoardData & BasicBoard::GetBoard() const
    {
      return board;
    }

    void BasicBoard::SetBoard(const BoardData &board_in)
    {
      board = board_in;
    }

    std::string BasicBoard::GetBoardString() const
    {
      unsigned int i,j;
      std::string str;
      char str_tmp[16];

      for(i = 0; i < 17; i++)
        {
          for(j = 0; j < 17; j++)
            {
              std::sprintf(str_tmp, "%d,", board.hole[i][j]);
              str += str_tmp;
            }
        }
      std::sprintf(str_tmp, "%d,", board.num_players);
      str += str_tmp;

      std::sprintf(str_tmp, "%d,", board.player_turn);
      str += str_tmp;

      std::sprintf(str_tmp, "%d,", number_of_turns);
      str += str_tmp;

      return str;
    }

    void BasicBoard::SetBoardString(const std::string &board_str_in)
    {
      unsigned int i,j;
      const char *str = board_str_in.c_str();

      for(i = 0; i < 17; i++)
        {
          for(j = 0; j < 17; j++)
            {
              std::sscanf(str, "%d,", &board.hole[i][j]);
              str = index(str, ',');
              str++;
            }
        }

      std::sscanf(str, "%d,", &board.num_players);
      str = index(str, ',');
      str++;

      std::sscanf(str, "%d,", &board.player_turn);
      str = index(str, ',');
      str++;

      std::sscanf(str, "%d,", &number_of_turns);
      str = index(str, ',');
      str++;
    }

    void BasicBoard::SetNumPlayer(int num_player_in)
    {
      board.num_players = num_player_in;
    }

    int BasicBoard::Forwardness(int player) const
    {
      int h = 0;
      int x,y;

      for(x=0; x<17; x++)
        {
          for(y=0; y<17; y++)
            {
              if(board.hole[x][y] == player)
                {
                  // grant one point for overall "forwardness"
                  switch(player)
                    {
                    case 1:
                      h += y;
                      break;
                    case 2:
                      h += 16-y;
                      break;
                    default:
                      // FIXME add calculation for each player
                      h += 0;
                      break;
                    }
                }
            }
        }

      return h;
    }


    void BasicBoard::InitBoard()
    {
      unsigned int i,j;
      unsigned int x,y;

      /* Mark all as unusable */
      for(x=0; x<17; x++)
        {
          for(y=0; y<17; y++)
            {
              board.hole[x][y] = -1;
            }
        }

      /* Mark usable part of open field */
      for(y=4; y<9; y++)
        {
          for(x=0+y-4; x<13; x++)
            {
              board.hole[x][y] = 0;
            }
        }

      for(y=9; y<13; y++)
        {
          for(x=4; x<14+(y-9); x++)
            {
              board.hole[x][y] = 0;
            }
        }

      /* Set Player Starts */
      for(i = 0; i < 6; i++)
        {
          for(j = 0; j < 10; j++)
            {
              /*
                std::cout << i << " " << j << " "
                << player_start[i][j].x << ", "
                << player_start[i][j].y << std::endl;
              */
              board.hole[player_start[i][j].x][player_start[i][j].y] = (i + 1);
            }
        }

      board.num_players = 0;
      board.player_turn = 1;

      number_of_turns   = 0;
      legal_moves_valid = false;

      MarkAllNotVisited();

      for(i = 0; i < 6; i++)
        {
          last_moves[i].from_x =
            last_moves[i].from_y =
            last_moves[i].to_x =
            last_moves[i].to_y = -1;

          player_times[i] = 0.;
        }
    }

    void BasicBoard::RecursiveVisit(int original_x, int original_y,
                                    int current_x, int current_y)
    {
      int try_x;
      int try_y;
      MoveData d;

      try_x = current_x;
      try_y = current_y+2;
      if(Occupied((current_x+try_x)/2,(current_y+try_y)/2)
         && Available(try_x, try_y) && (visited[try_x][try_y] == false) )
        {
          visited[try_x][try_y] = true;
          d.from_x = original_x;
          d.from_y = original_y;
          d.to_x = try_x;
          d.to_y = try_y;
          legal_moves.push_back(d);
          RecursiveVisit(original_x, original_y, try_x, try_y);
        }

      try_x = current_x+2;
      try_y = current_y+2;
      if(Occupied((current_x+try_x)/2,(current_y+try_y)/2)
         && Available(try_x, try_y) && (visited[try_x][try_y] == false) )
        {
          visited[try_x][try_y] = true;
          d.from_x = original_x;
          d.from_y = original_y;
          d.to_x = try_x;
          d.to_y = try_y;
          legal_moves.push_back(d);
          RecursiveVisit(original_x, original_y, try_x, try_y);
        }


      try_x = current_x-2;
      try_y = current_y;
      if(Occupied((current_x+try_x)/2,(current_y+try_y)/2)
         && Available(try_x, try_y) && (visited[try_x][try_y] == false) )
        {
          visited[try_x][try_y] = true;
          d.from_x = original_x;
          d.from_y = original_y;
          d.to_x = try_x;
          d.to_y = try_y;
          legal_moves.push_back(d);
          RecursiveVisit(original_x, original_y, try_x, try_y);
        }

      try_x = current_x+2;
      try_y = current_y;
      if(Occupied((current_x+try_x)/2,(current_y+try_y)/2)
         && Available(try_x, try_y) && (visited[try_x][try_y] == false) )
        {
          visited[try_x][try_y] = true;
          d.from_x = original_x;
          d.from_y = original_y;
          d.to_x = try_x;
          d.to_y = try_y;
          legal_moves.push_back(d);
          RecursiveVisit(original_x, original_y, try_x, try_y);
        }

      try_x = current_x-2;
      try_y = current_y-2;
      if(Occupied((current_x+try_x)/2,(current_y+try_y)/2)
         && Available(try_x, try_y) && (visited[try_x][try_y] == false) )
        {
          visited[try_x][try_y] = true;
          d.from_x = original_x;
          d.from_y = original_y;
          d.to_x = try_x;
          d.to_y = try_y;
          legal_moves.push_back(d);
          RecursiveVisit(original_x, original_y, try_x, try_y);
        }

      try_x = current_x;
      try_y = current_y-2;
      if(Occupied((current_x+try_x)/2,(current_y+try_y)/2)
         && Available(try_x, try_y) && (visited[try_x][try_y] == false) )
        {
          visited[try_x][try_y] = true;
          d.from_x = original_x;
          d.from_y = original_y;
          d.to_x = try_x;
          d.to_y = try_y;
          legal_moves.push_back(d);
          RecursiveVisit(original_x, original_y, try_x, try_y);
        }
    }


    void BasicBoard::MarkAllNotVisited()
    {
      int x,y;
      for(x=0; x<17; x++)
        {
          for(y=0; y<17; y++)
            {
              visited[x][y] = false;
            }
        }
    }

    bool BasicBoard::Usable(int x, int y) const
    {
      if(x>=0 && x<17 && y>=0 && y<17 && board.hole[x][y]!=-1)
        return true;
      else
        return false;
    }

    bool BasicBoard::Occupied(int x, int y) const
    {
      if(Usable(x,y) && board.hole[x][y]>=1)
        return true;
      else
        return false;
    }

    bool BasicBoard::Available(int x, int y) const
    {
      if(Usable(x,y) && board.hole[x][y]==0)
        return true;
      else
        return false;
    }

  }
}
