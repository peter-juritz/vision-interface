#include <cstring>
#include <cstdlib>
#include "frame.h"
#ifndef LREC_H
#define LREC_H
class l_rec
{
public:
  int top[2], bottom[2], left[2], right[2];
  int top_left[2], bottom_right[2];
  int buffer_size;
  int type;			// enum, R,G,B,Y
  int state;			// 0 = not found, 1 = found, add more
  int lastx, lasty;
  int initx, inity;
  frame *l_frame;
  short *recog_buffer;
  int num_points;		// number of points detected in ff
    l_rec (frame * f, int t);
  void floodfill_search (int x, int y);
  void floodfill_rec (int x, int y);
  bool accept (int x, int y);
  void register_active (int x, int y);
  bool check_solution ();
  void init ();
  void find ();
  bool find (int *tl, int *br);

};

#endif
