#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

const int MIN_WIDTH  = 5;
const int MAX_WIDTH  = 50;
const int MIN_HEIGHT = 5;
const int MAX_HEIGHT = 50;

enum
  {
    EMPTY,
    BLOCKED,
    START,
    GOAL
  };

int main(int argc, char **argv)
{
  int c;
  int map_width            = 6;
  int map_height           = 6;
  double block_probability = 0.20;
  int show_help            = 0;
  int map[MAX_WIDTH*MAX_HEIGHT] = { 0 };
  int x, y;
  double r;
  char   m = ' ';
  
  while((c = getopt(argc, argv,"w:l:p:h")) != EOF)
    {
      switch(c)
        {
        case 'w': map_width         = atoi(optarg); break;
        case 'l': map_height        = atoi(optarg); break;
        case 'p': block_probability = atof(optarg); break;
          
        default:
          printf("Unknown option: %c\n", c);
        case 'h': show_help   = 1; break;
        }
    }
  
  if(map_width < MIN_WIDTH || map_width > MAX_WIDTH)
    {
      std::cout << "Invalid map width " << map_width << std::endl;
      show_help ++;
    }
  if(map_height < MIN_HEIGHT || map_height > MAX_HEIGHT)
    {
      std::cout << "Invalid map height " << map_height << std::endl;
      show_help ++;
    }
  if(block_probability < 0.0 || block_probability > 1.0)
    {
      std::cout << "Invalid block probability " << block_probability << std::endl;
      show_help ++;
    }
  
  if(show_help)
    {
      std::cout << "-w num   : map_width" << std::endl;
      std::cout << "-l num   : map_height" << std::endl;
      std::cout << "-p num   : block_probability" << std::endl;
      std::cout << "-h       : show this message" << std::endl;
      return 0;
    }
  
  std::srand(static_cast<unsigned int>(std::time(0)));

  for(y = 0; y < map_height; y++)
    {
      for(x = 0; x < map_width; x++)
        {
          map[y*map_width + x] = EMPTY;
          r = (rand() % 100000)/100000.;
          if(r < block_probability)
            {
              map[y*map_width + x] = BLOCKED;
            }
        }
    }
  x = rand() % map_width;
  y = rand() % map_height;
  map[y*map_width + x] = START;
  while(map[y*map_width + x] == START)
    {
      x = rand() % map_width;
      y = rand() % map_height;
    }
  map[y*map_width + x] = GOAL;

  for(y = 0; y < map_height; y++)
    {
      for(x = 0; x < map_width; x++)
        {
          switch(map[y*map_width + x])
            {
            case EMPTY:   m = '.'; break;
            case BLOCKED: m = '*'; break;
            case START:   m = 's'; break;
            case GOAL:    m = 'g'; break;
            }
          std::cout << m << " ";
        }
      std::cout << std::endl;
    }
  
  return 0;
}
