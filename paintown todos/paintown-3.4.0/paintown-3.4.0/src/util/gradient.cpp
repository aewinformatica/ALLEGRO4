#include "gradient.h"
#include "funcs.h"

namespace Effects{

/* this class does virtually no error checking. great job */
Gradient::Gradient(int size, int startColor, int endColor):
colors(0),
size(size),
index(0){
    colors = new int[size];
    Util::blend_palette(colors, size / 2, startColor, endColor);
    Util::blend_palette(colors + size / 2, size / 2, endColor, startColor);
}

void Gradient::forward(){
    index = (index + 1) % size;
}

void Gradient::backward(){
    index = (index - 1 + size) % size;
}

void Gradient::update(){
    forward();
}

void Gradient::reset(){
    index = 0;
}

int Gradient::current(){
    return colors[index];
}
    
int Gradient::current(int offset){
    return colors[(index + offset + size) % size];
}

Gradient::~Gradient(){
    delete[] colors;
}

}