#pragma once
#include <iostream>
#include <allegro.h>
#include <vector>
using namespace std;

struct anim_t {
  char name[20];
  int frame_speed;
  int frame_count;
  int xorigin, yorigin;
  int w, h;
  std::vector<RLE_SPRITE*> anim_data;
};

class Sprite{
      public:
             Sprite();
             vector<anim_t*>animations;
             
             void Sprite::getAnim(DATAFILE *d);
             void draw(BITMAP *pBuffer);
             void logstr();
};
