#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <allegro.h>
using namespace std;

const int BLOCO_X = 160;
const int BLOCO_Y =  60;

const int CAMADAS = 3;

class Bloco
{
      public:
             int x,y,w,h,cx,cy;
             Bloco();
      };
