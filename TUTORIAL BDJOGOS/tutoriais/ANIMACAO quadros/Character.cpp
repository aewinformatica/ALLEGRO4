//Character.cpp
#include "Character.h"

Character::Character(){

        leftDone= false;
        rightDone= false;
        upDone= false;
        downDone= false;
        
        direction = DOWN;
        
        int x= 0;
        int y= 0;

}

Character::~Character(){

    delete [] images;

}

void Character::SetImagesLeft( char image1[], char image2[], char image3[]){
       
   images[0][0]= load_bitmap(image1, NULL);
   images[0][1]= load_bitmap(image2, NULL);
   images[0][2]= load_bitmap(image3, NULL);
   
   leftDone= true;
       
}

void Character::SetImagesRight(char image1[], char image2[], char image3[]){

   images[1][0]= load_bitmap(image1, NULL);
   images[1][1]= load_bitmap(image2, NULL);
   images[1][2]= load_bitmap(image3, NULL);
   
   rightDone= true;

}

void Character::SetImagesUp(char image1[], char image2[], char image3[]){

   images[2][0]= load_bitmap(image1, NULL);
   images[2][1]= load_bitmap(image2, NULL);
   images[2][2]= load_bitmap(image3, NULL);
   
   upDone= true;

}
       
void Character::SetImagesDown(char image1[], char image2[], char image3[]){

   images[3][0]= load_bitmap(image1, NULL);
   images[3][1]= load_bitmap(image2, NULL);
   images[3][2]= load_bitmap(image3, NULL);
   
   downDone= true;

}

bool Character::CheckImages(){

   if( rightDone && leftDone && upDone && downDone)
       return true;
       
   return false;         
                        
}
       
int Character::GetX(){

    return x;

}
       
int Character::GetY(){

    return y;

}

void Character::SetX( int newValue){
    x = newValue;
}

void Character::SetY( int newValue){
    y = newValue;
}

void Character::MoveOnX( int newDirection, BITMAP* tempBitmap, BITMAP* mainBitmap){

   direction = newDirection;

    if( CheckImages()){
        if( direction == LEFT){
            
            for( int i=0; i<=3; i++){
                EraseOldSprite( tempBitmap);
                x -= 5;
                
                if (i == 1 || i == 3)
                    DrawNewSprite( tempBitmap, images[0][0]);
                    
                if (i == 0)
                    DrawNewSprite( tempBitmap, images[0][1]);
    
                if (i == 2)
                    DrawNewSprite( tempBitmap, images[0][2]);
                
                
                draw_sprite( mainBitmap, tempBitmap, 0, 0);
                
                rest(50);
    
            }
        } else {
        
            for( int i=0; i<=3; i++){
                EraseOldSprite( tempBitmap);
                x += 5;
                
                if (i == 1 || i == 3)
                    DrawNewSprite( tempBitmap, images[1][0]);
                    
                if (i == 0)
                    DrawNewSprite( tempBitmap, images[1][1]);
    
                if (i == 2)
                    DrawNewSprite( tempBitmap, images[1][2]);
                
                draw_sprite( mainBitmap, tempBitmap, 0, 0);
                
                rest(50);
    
            }
               
        }
        
    }

}

void Character::MoveOnY( int newDirection, BITMAP* tempBitmap, BITMAP* mainBitmap){

   direction = newDirection;

    if( CheckImages()){
    
        if( direction == UP){
            
            for( int i=0; i<=3; i++){
                EraseOldSprite( tempBitmap);
                y -= 5;
                
                if (i == 1 || i == 3)
                    DrawNewSprite( tempBitmap, images[2][0]);
                    
                if (i == 0)
                    DrawNewSprite( tempBitmap, images[2][1]);
    
                if (i == 2)
                    DrawNewSprite( tempBitmap, images[2][2]);
                
                draw_sprite( mainBitmap, tempBitmap, 0, 0);
                
                rest(50);
    
            }
        } else {
        
            for( int i=0; i<=3; i++){
                EraseOldSprite( tempBitmap);
                y += 5;
                
                if (i == 1 || i == 3)
                    DrawNewSprite( tempBitmap, images[3][0]);
                    
                if (i == 0)
                    DrawNewSprite( tempBitmap, images[3][1]);
    
                if (i == 2)
                    DrawNewSprite( tempBitmap, images[3][2]);
                
                draw_sprite( mainBitmap, tempBitmap, 0, 0);
                
                rest(50);
    
            }
               
        }
    }
}


void Character::EraseOldSprite( BITMAP* tempBitmap){

    rectfill( tempBitmap, GetX(), GetY(), GetX() + 20, GetY() + 20, makecol ( 0, 0, 0));

}

void Character::DrawNewSprite( BITMAP* tempBitmap, BITMAP* spriteToDraw){

    draw_sprite( tempBitmap, spriteToDraw, GetX(), GetY());

}

void Character::Show(BITMAP* mainBitmap){

    if( CheckImages())
        draw_sprite( screen, images[3][0], 0, 0);

}

