/*
  Name: dialog.h
  Copyright: 2007 Lune Knight Software
  Author: Jeff Bernard
  Date: 09/14/07 09:03
*/

#ifndef __LK_DIALOG_H__
#define __LK_DIALOG_H__

#include <allegro.h>
#include <string>
#include <cctype>
#include "images.h"

class dialog
{
   private:
      std::string in, msg;
      BITMAP* box;
      BITMAP* text;
      int delay, x, y, at, cursor;
      char prev;
      bool getInput, focus;
   protected:
      dialog();
   public:
      virtual ~dialog();
      static dialog& instance();
      void draw(BITMAP* buffer);
      void update();
      void setInput(bool get);
      void setMsg(std::string _msg);
      bool hasMsg();
      void show(bool _focus);
      std::string input();
};

#endif
