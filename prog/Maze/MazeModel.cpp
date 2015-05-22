#include <iostream>
#include <string>
#include <cstring>
#include <fstream>

#include "MazeModel.h"

namespace ai
{
  namespace Sample
  {
    namespace Maze
    {
      Model::Model(const char *file_name)
        : start_x(-1), start_y(-1),
          goal_x(-1), goal_y(-1),
          width(-1), height(-1)
      {
        this->ReadFile(file_name);
      }
      
      Model::~Model()
      {
        /* empty */
      }

      int Model::GetStartX() const
      {
        return this->start_x;
      }
      int Model::GetStartY() const
      {
        return this->start_y;
      }
      int Model::GetGoalX() const
      {
        return this->goal_x;
      }
      int Model::GetGoalY() const
      {
        return this->goal_y;
      }
      int Model::GetWidth() const
      {
        return this->width;
      }
      int Model::GetHeight() const
      {
        return this->height;
      }
        
      int Model::GetMazeCell(const int &x, const int &y) const
      {
        int i = y * this->width + x;
        std::map<int, int>::const_iterator it;
        it = this->maze.find(i);
        if(it == this->maze.end())
          {
            return T_OUT_OF_BOUNDS;
          }
        return it->second;
      }
      
      bool Model::SetPathCell(const int &x, const int &y)
      {
        int i = y * this->width + x;
        std::map<int, int>::iterator it;
        it = this->maze.find(i);
        if(it == this->maze.end())
          { /* this method is intended to modify existing entries */
            return false;
          }
        if(!(it->second == T_START || it->second == T_GOAL || it->second == T_BLOCKED))
          {
            it->second = T_PATH;
          }
        return true;
      }

      static int string_to_type(const char *s)
      {
        int type = Model::T_MIN;
        if(std::strcmp(s, ".") == 0)
          {
            type = Model::T_EMPTY;
          }
        else if(std::strcmp(s, "*") == 0)
          {
            type = Model::T_BLOCKED;
          }
        else if(std::strcmp(s, "s") == 0)
          {
            type = Model::T_START;
          }
        else if(std::strcmp(s, "g") == 0)
          {
            type = Model::T_GOAL;
          }
        return type;
      }

      void Model::Display() const
      {
        std::map<int, int>::const_iterator it;
        int x, y, i;
        char m;
        for(y = 0; y < this->height; y++)
          {
            for(x = 0; x < this->width; x++)
              {
                i = y*this->width + x;
                it = this->maze.find(i);
                switch(it->second)
                  {
                  case T_EMPTY:   m = '.'; break;
                  case T_BLOCKED: m = '*'; break;
                  case T_START:   m = 's'; break;
                  case T_GOAL:    m = 'g'; break;
                  case T_PATH:    m = '+'; break;
                  default:
                    m = '?';
                    break;
                  }
                std::cout << m << " ";
              }
            std::cout << std::endl;
          }
      }
      
      bool Model::ReadFile(const char *file_name)
      {
        std::string line;
        std::ifstream fin(file_name);
        this->maze.clear();
        if(fin.is_open())
          {
            this->height = 0;
            while(fin.good())
              {
                std::getline(fin, line);
                if(fin.good())
                  {
                    const char *v = line.c_str();
                    
                    int i;
                    int w = 0;
                    for(i = 0; i < (int)std::strlen(v); i++)
                      {
                        if(v[i] != ' ')
                          {
                            w++;
                          }
                      }
                    if(this->width < 0)
                      {
                        this->width = w;
                      }
                    else if(this->width != w)
                      {
                        std::cout << "Widths don't match" << this->width << " " << w << std::endl;
                      }
                    
                    char *values = new char[std::strlen(v) + 1];
                    std::strcpy(values, v);
                  
                    char *s = std::strtok(values, " ");
                    int type;
                    int x = 0;
                    while(s)
                      {
                        type =string_to_type(s);
                        if(type == T_START)
                          {
                            this->start_x = x;
                            this->start_y = this->height;
                          }
                        else if(type == T_GOAL)
                          {
                            this->goal_x = x;
                            this->goal_y = this->height;
                          }
                        
                        i = this->height*this->width + x;
                        this->maze[i] = type;
                        s = std::strtok(NULL, " ");
                        x++;
                      }
                    this->height++;
                  }
              }
            fin.close();
          }
        else
          {
            std::cout << "Unable to open file " << file_name << std::endl;
            return false;
          }
        return true;
      }
        
    }
  }
}

/* Local Variables: */
/* mode:c++         */
/* End:             */
