/*
  Name: images.h
  Copyright: 2007 Lune Knight Software
  Author: Jeff Bernard
  Date: 09/14/07 08:57 
*/

#ifndef __LK_IMAGES_H__
#define __LK_IMAGES_H__

#include <allegro.h>
#include <string>

namespace images
{
   namespace draw
   {
      void roundedRect(BITMAP *bmp, int x1, int y1, int x2, int y2, int r, int c);
   }
   
   BITMAP* dialogBox(int width, int height);
   BITMAP* shadowText(std::string text);
}

#endif
