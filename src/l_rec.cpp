#include "l_rec.h"
#include "frame.h"
#include <stdio.h>

/*
 * Recog buffer: 0 unexplored, 1 good, 2 bad
  add varying stuff ( accept within range , topology)
 */

l_rec::l_rec (frame * f, int rec_type)
{				// XXX XXX FIXME chane back to reference
  buffer_size = f->width * f->height * sizeof (short);
  recog_buffer = (short *) malloc (buffer_size);
  l_frame = f;
  type = rec_type;
  lastx = -1;
  lasty = -1;
  init ();
}

void
l_rec::find ()
{				// XXX cleanup repetition in full search
  memset (recog_buffer, 0, buffer_size);
  init ();
  floodfill_search (lastx, lasty);	// try last position
  if (!check_solution ())
    {				// try local search
      for (int y = l_frame->heightclip (lasty - 15);
	   y < l_frame->heightclip (y + 15); y += 3)
	{
	  for (int x = l_frame->widthclip (lastx - 15);
	       x < l_frame->widthclip (lastx + 15); x += 3)
	    {
	      floodfill_search (x, y);
	      if (check_solution ())
		{
		  printf ("LOCAL SEARCH WORKED!!!!!!!!!!!!!!!!!!!\n");
		  return;
		}
	    }
	}
      for (int y = 0; y < l_frame->height; y += 5)
	{
	  for (int x = 0; x < l_frame->width; x += 5)
	    {
	      floodfill_search (x, y);
	      if (check_solution ())
		return;
	    }
	}
      printf ("Nothing found!\n");
    }
  else
    {

      printf ("LAST SEARCH WORKED!!!!!!!!!!!!!!!!!!!\n");
    }

}

bool
l_rec::find (int *tl, int *br)
{
  memset (recog_buffer, 0, buffer_size);
  init ();
  for (int y = tl[1]; y < br[1]; y += 5)
    {
      for (int x = tl[0]; x < br[0]; x += 5)
	{
	  floodfill_search (x, y);
	  if (check_solution ())
	    return true;
	}
    }
  return false;

}

void
l_rec::init ()
{
  top[0] = -1;
  bottom[0] = -1;
  right[0] = -1;
  left[0] = -1;
  initx = -1;
  inity = -1;
  state = -1;
  num_points = 0;
}

void
l_rec::floodfill_rec (int x, int y)
{
  if (!l_frame->in_bounds (x, y))
    return;

  if (recog_buffer[l_frame->width * y + x] != 0)
    return;
  if (accept (x, y))
    {

      recog_buffer[l_frame->width * y + x] = 1;
      num_points++;
      if (x > right[0] || right[0] == -1)
	{
	  right[0] = x;
	  right[1] = y;
	}
      if (x < left[0] || left[0] == -1)
	{
	  left[0] = x;
	  left[1] = y;
	}
      if (y > bottom[1] || bottom[0] == -1)
	{
	  bottom[0] = x;
	  bottom[1] = y;
	}
      if (y < top[1] || top[0] == -1)
	{
	  top[0] = x;
	  top[1] = y;
	}
      floodfill_rec (x + 1, y);
      floodfill_rec (x - 1, y);
      floodfill_rec (x, y + 1);
      floodfill_rec (x, y - 1);

    }
  else
    {
      recog_buffer[l_frame->width * y + x] = 2;
    }

}

void
l_rec::floodfill_search (int x, int y)
{				// XXX fix ugliness
  if (x == -1)
    return;
  initx = x;
  inity = y;
  if (!accept (x, y))
    return;
  init ();
  initx = x;
  inity = y;
  floodfill_rec (x, y);

}

void
l_rec::register_active (int x, int y)
{
  l_frame->set_r (x, y, 0);
  l_frame->set_g (x, y, 0);
  l_frame->set_b (x, y, 0);
}

bool
l_rec::check_solution ()
{				// XXX FIXME add gemoetry checks and such
  if (num_points > 100)
    {
      state = 1;		// found
      lastx = initx;
      lasty = inity;
      top_left[0] = left[0];
      top_left[1] = top[1];
      bottom_right[0] = right[0];
      bottom_right[1] = bottom[1];
      return true;
    }
  else
    {
      state = 0;		// not found
      return false;
    }
}

bool
l_rec::accept (int x, int y)
{
  short R = l_frame->get_r (x, y);
  short G = l_frame->get_g (x, y);
  short B = l_frame->get_b (x, y);
  switch (type)
    {
    case 0:			//R
      //  if (R>160 && G<100 && B < 100){
      if (R > 200 && G < 180 && B < 50)
	{
	  return true;
	}
      break;
    case 1:			//G
      //  if (G>150 && R < 200 && B < 200){
      if (G > 1.5 * R && G > 1.5 * B && G > 80)
	{
	  return true;
	}
      break;
    case 2:			//B
      if (B > 130 && R < 100 && G < 130)
	{
	  return true;
	}
      break;
    case 3:			//Y
      if (R > 180)
	{
	  return true;
	}
      break;
    }
  return false;
}
