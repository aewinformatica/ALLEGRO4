/*
  Name: images.cpp
  Copyright: 2007 Luneknight Software
  Author: Jeff Bernard
  Date: 09/14/07 08:55
*/

#include "images.h"

namespace images
{
   namespace draw
   {
      void roundedRect(BITMAP *bmp, int x1, int y1, int x2, int y2, int r, int c)
      {
          hline(bmp, x1+r, y1, x2-r, c);
          hline(bmp, x1+r, y2, x2-r, c);
          vline(bmp, x1, y1+r, y2-r, c);
          vline(bmp, x2, y1+r, y2-r, c);
          arc(bmp, x1+r, y1+r,  itofix(64), itofix(128), r, c);
          arc(bmp, x2-r, y1+r,   itofix(0),  itofix(64), r, c);
          arc(bmp, x1+r, y2-r, itofix(128), itofix(192), r, c);
          arc(bmp, x2-r, y2-r, itofix(192), itofix(256), r, c);
          
      }
   }
   
   BITMAP* dialogBox(int width, int height)
   {
      BITMAP* canvas = create_bitmap(width, height);
      clear_to_color(canvas, makecol(255,0,255));
      draw::roundedRect(canvas, 0, 0, 9, 9, 2, makecol(32,80,32));
      floodfill(canvas, 3, 3, makecol(34,139,34));
      draw::roundedRect(canvas, width-10, 0, width-1, 9, 2, makecol(32,80,32));
      floodfill(canvas, width-4, 3, makecol(34,139,34));
      draw::roundedRect(canvas, 0, height-10, 9, height-1, 2, makecol(32,80,32));
      floodfill(canvas, 3, height-4, makecol(34,139,34));
      draw::roundedRect(canvas, width-10, height-10, width-1, height-1, 2, makecol(32,80,32));
      floodfill(canvas, width-4, height-4, makecol(34,139,34));
      rectfill(canvas, 4, 4, width-5, height-5, makecol(68,169,68));
      rect(canvas, 3, 3, width-4, height-4, makecol(32,80,32));
      rectfill(canvas, 3, 3, 6, height-4, makecol(34,139,34));
      rectfill(canvas, 3, 3, width-4, 6, makecol(34,139,34));
      rectfill(canvas, width-4, height-4, width-7, 3, makecol(34,139,34));
      rectfill(canvas, width-4, height-4, 6, height-7, makecol(34,139,34));
      rect(canvas, 7, 7, width-8, height-8, makecol(32,80,32));
      bool alt = false;
      for (int i = 9; i < height-8; i+=2)
         hline(canvas, 8, i, width-9, alt ? makecol(98+(alt = !alt)*0,98,98) : makecol(32+(alt = !alt)*0,128,16));
      return canvas;
   }
   
   BITMAP* shadowText(std::string text)
   {
      BITMAP* canvas = create_bitmap(text.size()*8, 13);
      clear_to_color(canvas, makecol(255,0,255));
      textprintf_ex(canvas,font,0,0,makecol(0,0,102),-1,"%s",text.c_str());
      BITMAP* plate = create_bitmap(text.size()*16, 24);
      clear_to_color(plate, makecol(255,0,255));
      stretch_blit(canvas,plate,0,0,canvas->w,canvas->h,0,-3,plate->w,plate->h);
      clear_to_color(canvas, makecol(255,0,255));
      textprintf_ex(canvas,font,0,0,makecol(153,153,255),-1,"%s",text.c_str());
      masked_stretch_blit(canvas,plate,0,0,canvas->w,canvas->h,2,0,plate->w,plate->h-3);
      destroy_bitmap(canvas);
      return plate;
   }
}
