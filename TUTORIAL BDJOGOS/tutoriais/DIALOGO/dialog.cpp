#include "dialog.h"

dialog::dialog()
{
   this->box = images::dialogBox(400, 140);
   this->text = create_bitmap(400, 140);
   clear_to_color(text, makecol(255, 0, 255));
   this->getInput = false;
   this->focus = false;
   this->msg = "";
   this->in = "";
   this->delay = this->at = this->prev = 0;
   this->x = this->y = 20;
   this->cursor = 200;
}

dialog::~dialog()
{
   if (this->box)
      destroy_bitmap(this->box);
}

dialog& dialog::instance()
{
   static dialog dlg;
   return dlg;
}

void dialog::draw(BITMAP* buffer)
{
   if (this->focus)
   {
      masked_blit(this->box, buffer, 0, 0, 120, SCREEN_H-this->box->h-20, this->box->w, this->box->h);
      masked_blit(this->text, buffer, 0, 0, 120, SCREEN_H-this->text->h-20, this->text->w, this->text->h);
   }
}

void dialog::setInput(bool get)
{
   this->getInput = get;
   this->in = "";
}

void dialog::show(bool _focus)
{
   this->focus = _focus;
   this->in = "";
   this->msg = "";
   if (this->focus)
      clear_to_color(this->text, makecol(255,0,255));
}

void dialog::setMsg(std::string _msg)
{
   this->msg = _msg;
   this->in = "";
   this->x = this->y = 20;
   this->at = this->delay = 0;
   clear_to_color(this->text, makecol(255,0,255));
}
void dialog::update()
{
   if (this->focus)
   {
      if (this->at < this->msg.size() && this->delay-- <= 0)
      {
         int j = this->x;
         for (int i = this->at; i < this->msg.size(); i++)
         {
            if (this->msg[i] == ' ')
               break;
            else if (j >= this->text->w-20)
            {
               this->x = 20;
               this->y += 15;
               break;
            }
            j += 8;
         }
         textprintf_ex(this->text, font, this->x, this->y, makecol(0,64,0), -1, "%c", this->msg[this->at]);
         this->at++;
         this->delay = 6;
         this->x += 8;
         if ((this->x >= this->text->w-20) || this->at >= this->msg.size())
         {
            this->x = 20;
            this->y += 15;
            if (this->at >= this->msg.size())
               this->y += 15;
         }
         clear_keybuf();
      }
      else if (this->at >= this->msg.size() && this->getInput)
      {
         if (this->cursor > 0)
         {
            rectfill(this->text, this->x, this->y+10, this->x+10, this->y+12, makecol(64,64,64));
            this->cursor--;
            if (this->cursor == 0)
               this->cursor = -100;
         }
         else if (this->cursor < 0)
         {
            rectfill(this->text, this->x, this->y+10, this->x+10, this->y+12, makecol(255,0,255));
            this->cursor++;
            if (this->cursor == 0)
               this->cursor = 200;
         }
         if (keypressed())
         {
            char c = toupper(readkey() & 0xFF);
            if ((isalnum(c) || c == '\b' || c == '\r' || c == '\n' || c == ' ') && c != this->prev)
            {
               this->prev = c;
               switch (c)
               {
                  case '\b': if (this->in.size())
                             {
                                this->in = this->in.substr(0,this->in.size()-1);
                                this->x -= 8;
                                if (this->x <= 12)
                                   this->x = 20;
                                rectfill(this->text, this->x, this->y, this->x+20, this->y+12, makecol(255,0,255));
                             }
                             break;
                  case '\r':
                  case '\n': this->getInput = false;
                             if (!this->in.size())
                                this->in = "-1";
                             return;
                             break;
                  default:   this->in += c;
                             rectfill(this->text, this->x, this->y+10, this->x+10, this->y+12, makecol(255,0,255));
                             textprintf_ex(this->text, font, this->x, this->y, makecol(64,64,64), -1, "%c", c);
                             this->x += 8;
                             if (this->x >= this->text->w-20)
                             {
                                this->x = 20;
                                this->y += 15;
                             }
               }
            }
            clear_keybuf();
         }
         else if (!keypressed())
            this->prev = 0;
      }
   }
}

std::string dialog::input()
{
   if (!this->getInput)
      return this->in;
   std::string str = "";
   return str;
}

bool dialog::hasMsg()
{
   if (this->msg.size())
      return true;
   return false;
}
