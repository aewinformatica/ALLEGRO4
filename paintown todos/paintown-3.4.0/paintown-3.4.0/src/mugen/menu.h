#ifndef _mugen_menu_h
#define _mugen_menu_h

#include <vector>
#include <map>

#include "exception.h"
#include "gui/fadetool.h"

#include "menu/menu.h"
#include "menu/menu_option.h"

#include "util.h"

/*
 * We already have a setup for menus, might as well as utilize it to accomodate mugen
 */

class MugenAnimation;
class MugenFont;
class MugenSprite;
class MugenSection;

namespace Ast{
    class Section;
}

struct MugenMenuArea{
    int x1;
    int y1;
    int x2;
    int y2;
    int alpha;
    int alphaMove;
    bool visible;
};

class CursorHandler{
    public:
	CursorHandler();
	virtual ~CursorHandler();
	
	// Handles cursor box blinking
	virtual void act();
	virtual void renderText(int x, int y, bool active, const std::string & text, std::vector<MugenFont *> &, const Bitmap &);
	int getFontHeight(std::vector<MugenFont *> & fonts);
	virtual inline void setCursor(int x1, int y1, int x2, int y2){
	    this->cursor.x1 = x1;
	    this->cursor.y1 = y1;
	    this->cursor.x2 = x2;
	    this->cursor.y2 = y2;
	}
	
	virtual int getCursorHeight() const;
	
	virtual inline void setCursorVisible(bool visible){
	    this->cursor.visible = visible;
	}
	
	virtual inline void setItemFont(int index, int bank, int position){
	    this->itemFont.index = index;
	    this->itemFont.position = position;
	    this->itemFont.bank = bank;
	}
	
	virtual inline void setActiveFont(int index, int bank, int position){
	    this->activeFont.index = index;
	    this->activeFont.position = position;
	    this->activeFont.bank = bank;
	}
	
    private:
	virtual void renderCursor(int x, int y, const Bitmap &);
	MugenMenuArea cursor;
	Mugen::FontInfo itemFont;
	Mugen::FontInfo activeFont;
};

namespace NewMenu{
    class Context;
}

namespace Mugen{
    class Background;
    class Character;
    class CharacterSelect;
    class Storyboard;
    
class ItemOption : public MenuOption {
    public:
	ItemOption();
	virtual ~ItemOption();
	
	virtual void logic();
	virtual void run(const Menu::Context &);
	
	virtual void render(int x, int y, CursorHandler &, std::vector<MugenFont *> &, const Bitmap &);
	
	virtual void executeOption(const Mugen::PlayerType &, bool & endGame)=0;
};

}

class MugenStage;

class MugenMenu {
    public:
	/*! ctor dtor */
        explicit MugenMenu(const MugenMenu & menu);
	MugenMenu(const Filesystem::RelativePath &filename);
	virtual ~MugenMenu();
	
	/*! load */
	virtual void loadData();
	
	/*! do logic, draw whatever */
	virtual void run();
	
        virtual inline Filesystem::AbsolutePath getSelectInfoFile() const {
            return selectInfoFile;
        }
        
        inline void setName(const std::string & name){
            this->name = name;
        }
        
        inline const std::string & getName() const {
            return this->name;
        }
	
    private:
        
        std::string name;
        
        // Add option
	void addMenuOption(Mugen::ItemOption * option);

	// Option offset
	int optionLocation;
	
        Filesystem::RelativePath location;
	
	//spr = system.sff          ;Filename of sprite data
	std::string spriteFile;
	// snd = system.snd          ;Filename of sound data
	std::string soundFile;
	//logo.storyboard =         ;Logo storyboard definition (optional)
	std::string logoFile;
	//intro.storyboard =        ;Intro storyboard definition (optional)
	std::string introFile;
	//select = select.def       ;Character and stage selection list
	std::string selectFile;
	//fight = fight.def         ;Fight definition filename
	std::string fightFile;
	
	/*! Fonts */
	std::vector<MugenFont *>fonts;
	
	/*! Fade tool */
	Gui::FadeTool fader;
	
	/*! Menu Position */
	Mugen::Point position;
        
        /*! Current menu position for smooth scrolling */
        int currentMenuPosition;

        /*! Y Menu range positioning x = top y = bottom */
        Mugen::Point menuRange;
	
	/*! Limit speed on scroll */
	int menuScrollWait;
	
	/*! Box cursor and fonts */
	CursorHandler fontCursor;
	
	/*! Fonts spacing */
	Mugen::Point fontSpacing;
	
	/*
	; These parameters define the window in which the items are visible
	; in.*/
	//menu.window.margins.y = 12, 8 I can only assume there is an X version of this as well
	Mugen::Point windowMargin;
	//menu.window.visibleitems = 5
	int windowVisibleItems;
	//; These are the sounds for cursor movement (will be implemented later)
	//cursor.move.snd = 100,0
	//cursor.done.snd = 100,1
	//cancel.snd = 100,2
	
	// Our ticker/frames during duration of title
	int ticker;
	
	/* Sprites */
	Mugen::SpriteMap sprites;

        /*! Sounds */
        Mugen::SoundMap sounds;

        Mugen::Point moveSound;
        Mugen::Point doneSound;
        Mugen::Point cancelSound;
	
	// Title background
	Mugen::Background *background;
	
	// Logo
	Mugen::Storyboard *logo;
	
	// Intro
	Mugen::Storyboard *intro;
	
	// Select Info
	Mugen::CharacterSelect *characterSelect;
	
	// Cleanup
	void cleanup();
	
	// Cleanup sprites
	void cleanupSprites();

        // Move menu up
        void moveMenuUp();

        // Move menu down
        void moveMenuDown();

        // Handle movement
        void doMenuMovement();
	
	// Draw text
	void renderText(Bitmap *bmp);
	
	// Move text (for opening sequence where text scrolls into place from the sides)
	bool moveText;

        bool doInput(InputMap<Mugen::Keys> & input, Mugen::PlayerType & chosenPlayer, Mugen::PlayerType type);
	
        /* FIXME: don't use Point, make up a new object so we can call the
         * data 'left' and 'right' instead of 'x' and 'y'
         */
	// side position x left y right
	Mugen::Point movePosition;

        Filesystem::AbsolutePath selectInfoFile;
	
	// Options
	std::vector<Mugen::ItemOption *> options;
	
	std::vector<Mugen::ItemOption *>::iterator currentOption;
};

namespace Mugen{
    /* run the mugen menu */
    void run();
}

#endif
