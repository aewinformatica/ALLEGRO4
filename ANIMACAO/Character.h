//Character.h

#ifndef CHARACTER_H
#define CHARACTER_H

#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4

#include <allegro.h>

class Character{

public:
       Character();
       ~Character();
       
       // The following 4 methods will be used to set the images for the different
       //stages of movement
       void SetImagesLeft( char image1[], char image2[], char image3[]);
       void SetImagesRight( char image1[], char image2[], char image3[]);
       void SetImagesUp( char image1[], char image2[], char image3[]);
       void SetImagesDown( char image1[], char image2[], char image3[]);
       
       //To keep the program from crashing, this function will be used before 
       //calling any movement methods
       bool CheckImages();
       
       int GetX();
       void SetX( int newValue);
       void MoveOnX( int newValue, BITMAP* tempBitmap, BITMAP* mainBitmap);
       
       int GetY();
       void SetY( int newValue);
       void MoveOnY( int newValue, BITMAP* tempBitmap, BITMAP* mainBitmap);
       
       void EraseOldSprite( BITMAP* tempBitmap);
       void DrawNewSprite( BITMAP* tempBitmap, BITMAP* spriteToDraw);  
       
       void Show(BITMAP* mainBitmap);     
       
private:
        bool leftDone;
        bool rightDone;
        bool upDone;
        bool downDone;
        
        int direction;
        
        
        
        int x;
        int y;
        
        //This will contain all the images, [0][0-2] will be left movements, [1][0-2] right
        //[2][0-2] upwards, and [3][0-2] downwards
        BITMAP *images[4][3]; 


};

#endif
