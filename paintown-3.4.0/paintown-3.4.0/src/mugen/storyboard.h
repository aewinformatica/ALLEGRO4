#ifndef _mugen_storyboard_h
#define _mugen_storyboard_h

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "mugen/exception.h"
#include "gui/fadetool.h"
#include "mugen/util.h"

#include "input/input-map.h"

class Bitmap;
class MugenSprite;
class MugenSound;
class MugenAnimation;

namespace Ast{
class AstParse;
}

namespace Mugen{
 
class Background;


class Layer{
    public:
	Layer();
	virtual ~Layer();
        virtual void act(int currentTime);
        virtual void render(int x, int y, const Bitmap &);
        virtual void reset();
	virtual inline void setOffset(int x, int y){
	    this->offset.x = x;
	    this->offset.y = y;
	}

        virtual inline void setStartTime(int time){
            this->startTime = time;
        }

        virtual inline int getStartTime() const {
            return this->startTime;
        }

        virtual inline void setAnimation(MugenAnimation * animation){
            this->animation = animation;
        }
        //! Override
        virtual inline void setEnabled(bool enabled){
            this->enabled = enabled;
        }
    private:
	Mugen::Point offset;
	int startTime;
        bool enabled;
	MugenAnimation *animation;
};

class Scene {
    public:
	Scene(Ast::Section * data, const Filesystem::AbsolutePath & file, Ast::AstParse & parsed, SpriteMap & sprites);
	virtual ~Scene();
	
        virtual void act();
	
	virtual void render(const Bitmap &);
	
	virtual bool isDone();
	
	virtual void reset();
	
	virtual inline const Gui::FadeTool & getFadeTool() const {
	    return this->fader;
	}
	
	virtual inline void setClearColor(int color){
	    this->clearColor = color;
	}
	
	virtual inline int getClearColor() const {
	    return this->clearColor;
	}
	
	virtual inline bool getClearColorSet() const {
	    return this->clearColorSet;
	}
	
	virtual inline int getTicker() const {
	    return this->ticker;
	}
	
	virtual inline int getEndTime() const {
	    return this->endTime;
	}
	
	virtual inline void setDefaultPosition(const Mugen::Point & position){
	    this->defaultPosition = position;
	}
	
	virtual inline const Mugen::Point & getDefaultPosition() const {
	    return this->defaultPosition;
	}
	
	virtual inline bool getDefaultPositionSet() const {
	    return this->defaultPositionSet;
	}

    private:
	
        //! fade tool
        Gui::FadeTool fader;
	
        //! Clear color for the scene
	int clearColor;
	//! Clear color set?
	bool clearColorSet;
        //! Ticker for the scene
	int ticker;
        //! End time for this sceen
	int endTime;
	
        //! Default position
	Mugen::Point defaultPosition;
	
	//! Was default position set
	bool defaultPositionSet;
	
        //! Background
	Mugen::Background *background;
	
	//! layers
	std::vector<Layer *>layers;
	
	//! Max Layers - capped at 10
	const int maxLayers;
};

class Storyboard {
    public:
	//! Location at dataPath() + "mugen/stages/"
	Storyboard(const Filesystem::AbsolutePath & file, bool mask);

	virtual ~Storyboard();
	
	//! give it the default bitmap to blit to the screen
	virtual void run(const Bitmap &, bool repeat = false);

        virtual void setInput(const InputMap<Mugen::Keys> & input){
            this->input = input;
        }
	
    private:
        //! File
        Filesystem::AbsolutePath storyBoardFile;
	
        //! Beginning Scene Number
	int startscene;
	
	//! Sprites
        Mugen::SpriteMap sprites;
	
	//! Scenes
	std::vector<Scene *>scenes;

        //! Input
        InputMap<Mugen::Keys> input;
    
};

}

#endif


