#pragma once
#include <string>

struct BITMAP;

class The_Competition
{
public:
	The_Competition(void);
	~The_Competition(void);

	void Render(BITMAP * sc);
	void Update();

protected:
	BITMAP * Sprite_Grandma;
	BITMAP * Sprite_Arm;
	BITMAP * Sprite_Child;

	int Question;
	std::string Question_String;
	std::string Option_1;
	std::string Option_2;
	std::string Option_3;
	int Correct_Option;
	int Chosen_Option;

	bool mouse_pressed;
	
	std::string Grandma_Text;
	std::string Child_Text;
};
