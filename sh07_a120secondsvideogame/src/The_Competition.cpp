#include "allegro.h"
#include "The_Competition.h"
#include "Easy_Writer.h"
#include "MathUtils.h"
#include <string>

extern int Challenge_X;
extern int Challenge_Y;
extern Easy_Writer * Writer;
extern SAMPLE * Success_Sound;

extern float Size_Multiplier;

extern void Try_Finished(int);

using namespace std;

extern void Aborta_Con_Error(std::string);
The_Competition::The_Competition(void) : Sprite_Grandma(NULL), Sprite_Child(NULL), Sprite_Arm(NULL), 
	Question(0), Correct_Option(0), Chosen_Option(0), mouse_pressed(false)
{
	PALETTE palette;

	// Load the sprite
	Sprite_Grandma = load_bitmap("media/grandma.tga", palette);
	if(!Sprite_Grandma)
		Aborta_Con_Error("ERROR:\nError de ejecucion en la funcion Inicializa() de Objeto_Fisico.cpp.\n - Can't load  media/grandma.tga");

	Sprite_Child = load_bitmap("media/lilgirl.tga", palette);
	if(!Sprite_Child)
		Aborta_Con_Error("ERROR:\nError de ejecucion en la funcion Inicializa() de Objeto_Fisico.cpp.\n - Can't load  media/cat.tga");

	Sprite_Arm = load_bitmap("media/grandmaarm.tga", palette);
	if(!Sprite_Arm)
		Aborta_Con_Error("ERROR:\nError de ejecucion en la funcion Inicializa() de Objeto_Fisico.cpp.\n - Can't load  media/grandmaarm.tga");

}

The_Competition::~The_Competition(void)
{
	if(Sprite_Grandma != NULL)
		destroy_bitmap(Sprite_Grandma);
	if(Sprite_Child != NULL)
		destroy_bitmap(Sprite_Child);
}

void The_Competition::Render(BITMAP * sc)
{
	Vec2 VectorToMouse = Vec2(mouse_x, mouse_y) - Vec2(Challenge_X-350, Challenge_Y-63);
	//float Aux_Rotation = fixatan(ftofix(VectorToMouse.y/VectorToMouse.x));
	float Aux_Rotation = VectorToMouse.y/VectorToMouse.x;

	draw_sprite(sc,												// Se dibujará en el bitmap indicado
				Sprite_Grandma,									// bitmap a dibujar
				Challenge_X-300,		// Posición
				Challenge_Y-63);
	rotate_sprite(sc,												// Se dibujará en el bitmap indicado
				Sprite_Arm,											// bitmap a dibujar
				Challenge_X-300-27,	// Posición
				Challenge_Y-63+23,
				itofix((128*Aux_Rotation)/(2*3.141516)));	// Rotación

	draw_sprite(sc,												// Se dibujará en el bitmap indicado
				Sprite_Child,									// bitmap a dibujar
				Challenge_X+250,		// Posición
				Challenge_Y-63);

	Writer->Write_String("ANSUER THIS", makecol(50, 50, 255), Challenge_X, Challenge_Y-200*Size_Multiplier, 40*Size_Multiplier);
	Writer->Write_String(Question_String, makecol(50, 50, 255), Challenge_X, Challenge_Y-155*Size_Multiplier, 26*Size_Multiplier);
	Writer->Write_String(Option_1, makecol(50, 50, 255), Challenge_X, Challenge_Y-100*Size_Multiplier, 15*Size_Multiplier);
	Writer->Write_String(Option_2, makecol(50, 50, 255), Challenge_X, Challenge_Y-70*Size_Multiplier, 15*Size_Multiplier);
	Writer->Write_String(Option_3, makecol(50, 50, 255), Challenge_X, Challenge_Y-40*Size_Multiplier, 15*Size_Multiplier);

	Writer->Write_String(Grandma_Text, makecol(50, 50, 255), Challenge_X-250*Size_Multiplier, Challenge_Y-80*Size_Multiplier, 12*Size_Multiplier);
	Writer->Write_String(Child_Text, makecol(50, 50, 255), Challenge_X+250*Size_Multiplier, Challenge_Y-80*Size_Multiplier, 12*Size_Multiplier);
}

void The_Competition::Update()
{
	if(mouse_b && !mouse_pressed)
	{
		mouse_pressed = true;
		if(mouse_x > Challenge_X - 150 && mouse_x < Challenge_X + 150 )
		{
			if(abs(int(mouse_y - (Challenge_Y-100*Size_Multiplier))) < 15*Size_Multiplier)
				Chosen_Option = 1;
			if(abs(int(mouse_y - (Challenge_Y-70*Size_Multiplier))) < 15*Size_Multiplier)
				Chosen_Option = 2;
			if(abs(int(mouse_y - (Challenge_Y-40*Size_Multiplier))) < 15*Size_Multiplier)
				Chosen_Option = 3;
		}
		if(Correct_Option == Chosen_Option)
		{
			Question++;
			if(Question > 2)
				Try_Finished(1);
			else
				play_sample(Success_Sound, 255, 0, 1000, 0);
		}
		else
		{
			Try_Finished(-1);
		}
	}
	if(!mouse_b)
		mouse_pressed = false;

	switch(Question)
	{
	case 0:
		Question_String = "53 PLUS 52"; // 105
		Option_1 = "OPTION 1  105";
		Option_2 = "OPTION 2  115";
		Option_3 = "OPTION 3  150";
		Correct_Option = 1;
		
		Grandma_Text = "I UONT FAIL, LIL BASTARD";
		Child_Text = "ANSUER THIS IF YOU CAN";
		break;
	case 1:
		Question_String = "MINUS 37";	// 68
		Option_1 = "OPTION 1  58";
		Option_2 = "OPTION 2  78";
		Option_3 = "OPTION 3  68";
		Correct_Option = 3;
		
		Grandma_Text = "THAT UAS EASY";
		Child_Text = "BITCH";
		break;
	case 2:
		Question_String = "PLUS 276";	// 344
		Option_1 = "OPTION 1  343";
		Option_2 = "OPTION 2  444";
		Option_3 = "OPTION 3  344";
		Correct_Option = 3;
		
		Grandma_Text = "YEAH FUCK YOU LITTLE GIRL";
		Child_Text = "BUAAHH YOURE A BAD GRANDMA";
		break;
	}
}
