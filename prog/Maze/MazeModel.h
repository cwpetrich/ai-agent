#ifndef _MAZEMODEL_H_
#define _MAZEMODEL_H_
#include <map>

namespace ai
{
  namespace Sample
  {
    namespace Maze
    {
      class Model
      {
      public:
        enum
          {
            T_MIN,
            T_EMPTY,
            T_BLOCKED,
            T_START,
            T_GOAL,
            T_MAX,
            T_OUT_OF_BOUNDS,
            T_PATH
          };
        
        Model(const char *file_name);
        virtual ~Model();

        int GetStartX() const;
        int GetStartY() const;
        int GetGoalX() const;
        int GetGoalY() const;

        int GetWidth() const;
        int GetHeight() const;
        int GetMazeCell(const int &x, const int &y) const;

        bool SetPathCell(const int &x, const int &y);
        void Display() const;

      protected:
        bool ReadFile(const char *file_name);
        
        int start_x, start_y;
        int goal_x, goal_y;
        int width;
        int height;
        std::map<int, int> maze;
      private:
      };
    }
  }
}

#endif /* _MAZEMODEL_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
