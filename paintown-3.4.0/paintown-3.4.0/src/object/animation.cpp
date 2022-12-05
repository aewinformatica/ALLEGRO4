#include <iostream>
#include <sstream>
#include <string>

#include "animation.h"
#include "animation_event.h"
#include "animation_trail.h"
#include "attack.h"
#include "util/bitmap.h"
#include "util/lit_bitmap.h"
#include "character.h"
#include "util/funcs.h"
#include "util/ebox.h"
#include "globals.h"
#include "projectile.h"
#include "util/load_exception.h"
#include "object.h"
#include "util/file-system.h"
#include "util/sound.h"
#include "util/timedifference.h"
#include "util/token.h"
#include "util/token_exception.h"
#include "game/mod.h"

using namespace std;

namespace Paintown{

Frame::Frame( Bitmap * p, ECollide * e ){
	pic = p;
	collide = e;
	mine = true;
}
	
Frame::Frame( const Frame & f ){
    pic = f.pic;
    collide = f.collide;
    mine = false;
}

Frame::~Frame(){
    if ( mine ){
        delete pic;
        delete collide;
    }
}

static const char * key_names[] = {
    /* key_idle isn't a real key anymore */
    // "key_idle",
    "key_jump", "key_forward",
    "key_back", "key_grab", "key_right",
    "key_down", "key_up", "key_attack1",
    "key_attack2", "key_attack3", };

namespace Keys{
    /* order of the enums should match the key_names array *exactly* */
    enum Names{
        // Idle = 0,
        Jump = 0, Forward,
        Back, Grab, Right,
        Down, Up, Attack1,
        Attack2, Attack3,

        /* sentinal value. should always be last */
        Max,
    };
}

static string join(const char * names[], const int max, const string & sep){
    ostringstream out;

    for (int i = 0; i < max; i++){
        out << names[i];
        if (i != max - 1){
            out << sep;
        }
    }

    return out.str();
}

Animation::Animation(const Token * tok, Character * const owner ):
parent( owner ),
current_frame( NULL ),
attack_collide( NULL ),
delay( 1 ),
minZDistance( 10 ),
bbox_x1( 0 ),
bbox_y1( 0 ),
bbox_x2( 1 ),
bbox_y2( 1 ),
/*
attack_x1( 0 ),
attack_y1( 0 ),
attack_x2( 1 ),
attack_y2( 1 ),
*/
offset_x( 0 ),
offset_y( 0 ),
shadowX( 0 ),
shadowY( 0 ),
range( 0 ),
is_attack( false ),
status( Status_Ground ),
damage( 0 ),
commision( true ),
contact( NULL ){

    prev_sequence = "none";
    next_sequence = "none";

    if ( *tok != "anim" && *tok != "animation" ){
        Global::debug( 0 ) <<"Not an animation"<<endl;
        throw LoadException(__FILE__, __LINE__, "Not an animation");
    }

    TimeDifference diff;

    const Token * current1;

    string basedir("");
    diff.startTime();
    TokenView view = tok->view();
    while (view.hasMore()){
        try {
            view >> current1;
            const Token & current = *current1;

            if (current == "name" ){
                current.view() >> name;

                /* this should be deprecated soon */
                if ( name.find("attack" ) != string::npos ){
                    is_attack = true;
                }
            } else if ( current == "loop" ){
                current.view() >> loop;
            } else if ( current == "type" ){
                string x;
                current.view() >> x;
                if ( x == "attack" )
                    is_attack = true;
            } else if ( current == "basedir" ){
                current.view() >> basedir;
            } else if ( current == "offset" ){
                // *current >> offset_x >> offset_y;
                int x = 0;
                int y = 0;
                try{
                    current.view() >> x >> y;
                } catch ( const TokenException & te ){
                    /* ignore token exceptions here */
                }
                // cout<<"Read offset as "<<x<<" "<<y<<endl;
                AnimationEventOffset * ani = new AnimationEventOffset( x, y );
                events.push_back( ani );
            } else if (current == "projectile" ){
                AnimationEventProjectile * ani = new AnimationEventProjectile(current1);
                events.push_back( ani );
            } else if ( current == "move" ){
                try{
                    int x, y, z;
                    x = y = z = 0;
                    current.view() >> x >> y >> z;

                    AnimationEventMove * em = new AnimationEventMove( x, y, z );
                    events.push_back( em );
                } catch( const TokenException & te ){
                    Global::debug( 0 ) << "Could not read move event: " << te.getTrace() << endl;
                    /* ignore token exceptions here */
                }
            } else if (current == "blast"){
            } else if (current == "trail"){
                int produce = 0;
                int life = 0;
                TokenView trailView = current.view();
                while (trailView.hasMore()){
                    const Token * sub;
                    trailView >> sub;
                    if (*sub == "generate"){
                        sub->view() >> produce;
                    } else if (*sub == "length"){
                        sub->view() >> life;
                    }
                }
                AnimationEventTrail * trail = new AnimationEventTrail(produce, life);
                events.push_back(trail);
            } else if ( current == "damage" ){
                current.view() >> damage;
            } else if ( current == "sequence" ){
                string p, n;
                current.view() >> p >> n;
                // sequences[ p ] = n;
                // cout<<getName() << ":Adding sequence "<<p<<endl;
                sequences.push_back( p );
                // *current >> prev_sequence >> next_sequence;
            } else if ( current == "contact" ){
                string st;
                current.view() >> st;
                contact = new Sound(Filesystem::find(Filesystem::RelativePath(st)).path());
            } else if ( current == "keys" ){

                TokenView keyView = current.view();
                while (keyView.hasMore()){
                    const Token * nm;
                    keyView >> nm;
                    KeyPress press;
                    // nm->print("combo ");
                    if (nm->isData()){
                        string key_name = nm->getName();
                        Input::PaintownInput actualKey = convertKeyPress( key_name );
                        if ( actualKey == Input::Unknown ){
                            Global::debug( 0 ) << "Warning: '"<<key_name<<"' is not a valid key name. Valid keys are " << join(key_names, Keys::Max, ", ") << endl;
                        }
                        // cout<<"Convert "<<key_name<<" to "<<actualKey<<endl;
                        press.combo.push_back(actualKey);
                    } else {
                        TokenView moreView = nm->view();
                        while (moreView.hasMore()){
                            string key_name;
                            moreView >> key_name;
                            Input::PaintownInput actualKey = convertKeyPress( key_name );
                            if (actualKey == Input::Unknown){
                                Global::debug( 0 ) <<"Warning: '"<<key_name<<"' is not a valid key name. Valid keys are " << join(key_names, Keys::Max, ", ") << endl;
                            }
                            press.combo.push_back(actualKey);
                        }
                    }
                    keys.push_back( press );
                }
            } else if ( current == "status" ){

                string st;
                current.view() >> st;
                if ( st == "jump" ){
                    status = Status_Jumping;
                } else if ( st == "ground" ){
                    status = Status_Ground;
                } else if ( st == "grab" ){
                    status = Status_Grab;
                } else {
                    Global::debug( 0 ) <<"Unhandled status "<<st<<endl;
                }
            } else if ( current == "shadow" ){
                int x, y;
                current.view() >> x >> y;
                AnimationEvent * ani = new AnimationEventShadow( x, y );
                events.push_back( ani );
            } else if ( current == "coords" ){
                const Token * c;
                TokenView coordsView = current.view();
                coordsView >> c;
                int x, y, z;
                x = y = z = -1;
                while (c != NULL){
                    string dimension;
                    int d;
                    c->view() >> dimension >> d;
                    if ( dimension == "x" )
                        x = d;
                    if ( dimension == "y" )
                        y = d;
                    if ( dimension == "z" )
                        z = d;
                    coordsView >> c;
                }
                AnimationEvent * ani = new AnimationEventCoords( x, y, z );
                events.push_back( ani );
            } else if ( current == "bbox" ){

                /*
                   int x1, y1, x2, y2;
                   x1 = y1 = 0;
                   x2 = y2 = 1;
                   in >> x1 >> y1 >> x2 >> y2;
                   AnimationEvent * ani = new AnimationEventBBox( x1, y1, x2, y2 );
                   events.push_back( ani );
                   */
            } else if ( current == "attack" ){

                Attack ak(current);

                /*
                   int x1 = 0; 
                   int y1 = 0;
                   int x2 = 0;
                   int y2 = 0;
                   int damage = 0;
                   int force = 0;

                 *current >> x1 >> y1 >> x2 >> y2 >> damage >> force;
                 */

                // AnimationEvent * ani = new AnimationEventAttack( x1, y1, x2, y2, damage, force );
                AnimationEvent * ani = new AnimationEventAttack( ak );
                events.push_back( ani );
            } else if (current == "z-distance" ){
                double d;
                current.view() >> d;
                AnimationEvent * e = new AnimationEventZDistance( d );
                events.push_back( e );
            } else if ( current == "sound" ){
                string st;
                current.view() >> st;

                if (sounds.find(st) == sounds.end()){
                    Sound * sp = new Sound(Filesystem::find(Filesystem::RelativePath(st)).path());
                    sounds[st] = sp;
                }

                AnimationEvent * aes = new AnimationEventSound(st);
                events.push_back( aes );
            } else if ( current == "setstatus" ){
                string st;
                current.view() >> st;
                int m = Status_Ground;
                if ( st == "jump" ){
                    m = Status_Jumping;	
                } else if ( st == "grab" ){
                    m = Status_Grab;
                }
                AnimationEvent * ani = new AnimationEventStatus( m );
                events.push_back( ani );
            } else if ( current == "jump" ){
                double x, y, z;
                current.view() >> x >> y >> z;
                AnimationEvent * ani = new AnimationEventJump( x, y, z );
                events.push_back( ani );
            } else if ( current == "decommision" ){
                string l;
                current.view() >> l;
                addDecommision( l );
            } else if ( current == "commision" ){
                string l;
                current.view() >> l;
                addCommision( l );
            } else if ( current == "face" ){
                string way;
                current.view() >> way;
                int direction = Object::FACING_RIGHT;
                if ( way == "right" ){
                    direction = Object::FACING_RIGHT;
                } else if ( way == "left" ){
                    direction = Object::FACING_LEFT;
                } else if ( way == "reverse" ){
                    direction = -1;
                }
                AnimationEvent * ani = new AnimationEventFace( direction );
                events.push_back( ani );
            } else if ( current == "nop" ){
                events.push_back( new AnimationEventNOP() );
            } else if ( current == "next-ticket" ){
                events.push_back( new AnimationEventTicket() );
            } else if ( current == "range" ){
                int r;
                current.view() >> r;
                setRange(r);
            } else if ( current == "delay" ){
                int delay = 0;
                current.view() >> delay;
                AnimationEvent * ani = new AnimationEventDelay( delay );
                events.push_back( ani );
            } else if ( current == "frame" ){
                string path;
                current.view() >> path;
                Filesystem::RelativePath full = Filesystem::RelativePath(basedir + path);
                // Filesystem::AbsolutePath full = Filesystem::find(Filesystem::RelativePath(basedir + path));
                if (frames.find(full.path()) == frames.end()){
                    Bitmap * pic = Paintown::Mod::getCurrentMod()->createBitmap(full);
                    ECollide * collide = new ECollide(pic);
                    Frame * f = new Frame(pic, collide);
                    frames[full.path()] = f;
                    if (pic->getError()){
                        // Global::debug( 0 ) <<"Pic error"<<endl;
                        throw LoadException(__FILE__, __LINE__, "Could not load picture");
                    }
                }
                AnimationEvent * ani = new AnimationEventFrame(full.path());
                events.push_back(ani);
            } else {
                Global::debug( 0 ) << tok->getFileName() << " Unhandled animation attribute: "<<endl;
                current.print(" ");
            }

        } catch ( const TokenException & te ){
            current1->print(" ");
            throw LoadException(__FILE__, __LINE__, te, "Animation parse error");
        } catch (const Exception::Base & e){
            throw LoadException(__FILE__, __LINE__, e, "Could not load animation");
        }
    }

    diff.endTime();

    string xls = "Animation ";
    xls += getName();
    Global::debug( 4 ) << diff.printTime( xls ) << endl;

    if ( frames.empty() ){
        Global::debug( 0 )<<"No frames given"<<endl;
        // throw exception();
        throw LoadException(__FILE__, __LINE__, "No frames given");
    }

    delay_counter = 0;

    current_event = events.begin();

    current_frame = NULL;
    current_collide = NULL;
    map< string, Frame * >::iterator first = frames.begin();
    if ( first != frames.end() ){
        Frame * x = (*first).second;
        current_frame = x->pic;
        current_collide = x->collide;
    }

    own_bitmaps = true;
    own_events = true;

    // cout<<"Create animation "<<name<<endl;

    /*
       cout<<getName()<<" has sequences"<<endl;
       for ( vector<string>::iterator it = sequences.begin(); it != sequences.end(); it++ ){
       cout<< *it << endl;
       }
       */

}

Animation::Animation( const Animation & animation, Character * const owner ):
parent( owner ),
attack_collide( NULL ),
commision( true ){

	own_bitmaps = false;
	own_events = false;
	frames = animation.frames;
	// for ( vector< AnimationEvent * >::const_iterator it = animation.events.begin(); it != animation.events.end(); it++ )
		// events.push_back( *it );

	events = animation.events;

	name = animation.getName();

	setMinZDistance( animation.getMinZDistance() );

	bbox_x1 = animation.bbox_x1;
	bbox_y1 = animation.bbox_y1;
	bbox_x2 = animation.bbox_x2;
	bbox_y2 = animation.bbox_y2;
	/*
	attack_x1 = animation.attack_x1;
	attack_x2 = animation.attack_x2;
	attack_y1 = animation.attack_y1;
	attack_y2 = animation.attack_y2;
	*/
	delay = animation.delay;
	delay_counter = animation.delay_counter;
	// offset_x = animation.offset_x;
	// offset_y = animation.offset_y;
	offset_x = animation.offset_x;
	offset_y = animation.offset_y;
	shadowX = animation.shadowX;
	shadowY = animation.shadowY;
	is_attack = animation.isAttack();
	status = animation.getStatus();
	damage = animation.getDamage();

	setCommision( animation.isCommisioned() );

	sounds = animation.sounds;
	contact = animation.contact;

	disable_animations = animation.getDecommisions();
	enable_animations = animation.getCommisions();

	setRange( animation.getRange() );

	prev_sequence = animation.getPreviousSequence();
	next_sequence = animation.getNextSequence();
	
	// cout<<"Animation sequences = " << animation.getSequences().size() << endl;
	sequences = animation.getSequences();

	/*
	for ( map<string,string>::iterator it = sequences.begin(); it != sequences.end(); it++ ){
		cout<< (*it).first<<" - " << (*it).second << endl;
	}
	*/
	/*
	cout<<"Copy "<<getName()<<":Sequences"<<endl;
	for ( vector<string>::iterator it = sequences.begin(); it != sequences.end(); it++ ){
		cout<< *it << endl;
	}
	*/

	/* copy the key combos */
	keys = animation.keys;

	current_event = events.begin();

	current_frame = animation.current_frame;
	current_collide = animation.current_collide;

	/*
	map< string, Bitmap * >::iterator first = frames.begin();
	if ( first != frames.end() ){
		current_frame = (*first).second;
	}
	*/
}

void Animation::createProjectile( int x, int y, Projectile * projectile ){
	if ( parent ){
		int direction = parent->getFacing() == Object::FACING_LEFT ? -1 : 1;
		projectile->setX( parent->getX() + (getOffsetX() + x) * direction );
		projectile->setY( -getOffsetY() + y + parent->getY() );
		projectile->setZ( parent->getZ() );
		parent->createProjectile( projectile );
	} else {
		delete projectile;
	}
}
	
void Animation::nextTicket(){
	if ( parent ){
		parent->nextTicket();
	}
}
	
int Animation::getDamage() const{
	return attack.getDamage();
	// return damage;
}

void Animation::setDamage( const int d ){
	damage = d;
}

void Animation::upperCase( string & who ){
	for ( string::size_type q = 0; q < who.length(); q++ )
		if ( who[q] >= 'a' && who[q] <= 'z' )
			who[q] = (char)(who[q] - 'a' + 'A');
}
	
void Animation::setDelay( int _delay ){
	delay = _delay;
}
	
void Animation::addDecommision( const string & s ){
	disable_animations.push_back( s );	
}

void Animation::addCommision( const string & s ){
	enable_animations.push_back( s );
}
        
Animation * Animation::copy(Character * owner) const {
    return new Animation(*this, owner);
}

void Animation::setTrails(const int produce, const int life){
    if (parent){
        parent->setTrails(produce, life);
    }
}

void Animation::Delay(){
	delay_counter = delay;
}
	
void Animation::playSound( const string & path ){
	if ( sounds.find( path ) != sounds.end() ){
		Sound * s = sounds[ path ];
		s->play();
	}
}

AnimationTrail * Animation::makeTrail(const int x, const int y, const int facing, const int life) const {
    return new AnimationTrail(x, y, facing, life, *current_frame);
}
	
void Animation::setFacing( const int direction ){
	if ( parent ){
		if ( direction == -1 ){
			parent->reverseFacing();
		} else {
			parent->setFacing( direction );
		}
	}
}
	
void Animation::reMap( map< int, int > & colors ){
    /* if they arent are own bitmaps, make them so */
    if ( !own_bitmaps ){
        for ( map<string,Frame*>::iterator it = frames.begin(); it != frames.end(); it++ ){
            Frame* & xframe = (*it).second;

            Bitmap * xpic = new Bitmap( *(xframe->pic), true );
            ECollide * xcollide = new ECollide( xframe->collide );

            /* oh evil! I am altering a map while iterating over it.
             * It should be ok, though, since Im altering an element
             * that already exists.
             */
            frames[ (*it).first ] = new Frame( xpic, xcollide );
        }
    }

    for ( map<string,Frame*>::iterator it = frames.begin(); it != frames.end(); it++ ){
        Frame * xframe = (*it).second;

        Bitmap * use = xframe->pic;
        reMap(use, colors);
    }

    own_bitmaps = true;
}

void Animation::reMap( Bitmap * work, map< int, int > & colors ){
    const map<int,int>::iterator it_end = colors.end();

    /* maybe this is a little faster than just reading every pixel
     * and writing it back. i dunno
     */
    for ( int y1 = 0; y1< work->getHeight(); y1++ ){
        vector< int > xcols;
        work->readLine( xcols, y1 );
        for ( unsigned int x1 = 0; x1 < xcols.size(); x1++ ){
            int pixel = xcols[ x1 ];

            map<int,int>::iterator find_it = colors.find( pixel );
            if (find_it != it_end){
                work->putPixel( x1, y1, (*find_it).second );
            }
            /*
               if ( colors.find( pixel ) != it_end ){
               work->putPixel( x1, y1, colors[ pixel ] );
               }
               */
        }
    }

    /*
       for ( int x1 = 0; x1 < work->getWidth(); x1++ ){
       for ( int y1 = 0; y1 < work->getHeight(); y1++ ){
       int pixel = work->getPixel( x1, y1 );
       if ( colors.find( pixel ) != it_end ){
       work->putPixel( x1, y1, colors[ pixel ] );
       }
       }
       }
       */
}
	
void Animation::getAttackCoords( int & x, int & y ){	
	// x = (attack_x1+attack_x2)/2;
	// y = (attack_y1+attack_y2)/2;
	x = (attack.getX1() + attack.getX2() ) / 2;
	y = (attack.getY1() + attack.getY2() ) / 2;
	Global::debug( 0 ) << getName()<<"[ "<<attack.getX1()<<","<<attack.getY1()<<","<<attack.getX2()<<","<<attack.getY2()<<"]"<<endl;
}
	
ECollide * Animation::getNormalCollide(){
	return current_collide;
}

ECollide * Animation::getCollide( int facing ){
	if ( isAttack() ){
		if ( attack.getX1() != attack.getX2() && attack.getY1() != attack.getY2() ){
			if ( attack_collide ){
				delete attack_collide;
			}

			// attack_collide = new ECollide( attack_x2-attack_x1, attack_y2-attack_y1 );

			/* Have to create our own ECollide structure */
			attack_collide = new ECollide( getWidth(), getHeight() );
			EQuad * ac = attack_collide->getHead();
			EQuad * adder = new EQuad( attack.getXLen(), attack.getYLen(), NULL );
			if ( facing == Object::FACING_LEFT ){
				adder->setMinX( getWidth() - attack.getX2() );
				adder->setMinY( attack.getY1() );
			} else {
				adder->setMinX( attack.getX1() );
				adder->setMinY( attack.getY1() );
			}

			adder->setFull( true );
			ac->addQuad( adder );

			return attack_collide;
		}
		return NULL;
	}
	return current_collide;
}

// int Animation::convertKeyPress( const string & key_name ) throw( LoadException ){
Input::PaintownInput Animation::convertKeyPress( const string & key_name ) {
	// if (key_name == key_names[Keys::Idle]) return Unknown;
	if (key_name == key_names[Keys::Jump]) return Input::Jump;
	if (key_name == key_names[Keys::Forward]) return Input::Forward;
	if (key_name == key_names[Keys::Back]) return Input::Back;
	/* im not sure we need a grab key */
	if (key_name == key_names[Keys::Grab]) return Input::Grab;
	// if ( key_name == "key_right" ) return KEY_RIGHT;
	if (key_name == key_names[Keys::Down]) return Input::Down;
	if (key_name == key_names[Keys::Up]) return Input::Up;
	if (key_name == key_names[Keys::Attack1]) return Input::Attack1;
	if (key_name == key_names[Keys::Attack2]) return Input::Attack2;
	if (key_name == key_names[Keys::Attack3]) return Input::Attack3;

    Global::debug(1) << "Unknown key name '" << key_name << "'" << endl;

	return Input::Unknown;

	// return KEY_ENTER;
}
	
bool Animation::properSequence( const string & seq ){
	/*
	for ( map<string,string>::iterator it = sequences.begin(); it != sequences.end(); it++ ){
		const string & prev = (*it).first;
		if ( prev == "none" || prev == seq )
			return true;
	}
	*/
	if ( sequences.empty() )
		return true;
	for ( vector< string >::iterator it = sequences.begin(); it != sequences.end(); it++ ){
		// cout<<getName()<<": proper sequence with "<<seq<<endl;
		if ( *it == seq || *it == "none" )
			return true;
	}
	return false;
}

bool Animation::hasSequence( const string & seq ){
	for ( vector< string >::iterator it = sequences.begin(); it != sequences.end(); it++ ){
		////  cout<<"Testing has sequence "<<seq<<" against my "<<*it<<endl;
		if ( *it == seq )
			return true;
	}
	/*
	for ( map<string,string>::iterator it = sequences.begin(); it != sequences.end(); it++ ){
		const string & prev = (*it).first;
		if ( prev == seq )
			return true;
	}
	*/
	return false;
}

Bitmap * Animation::getFrame( int x ){
	int i = 0;
	map< string, Frame * >::iterator it;
	for ( it = frames.begin(); it != frames.end() && i < x; it++, i++ );

	if ( it == frames.end() ){
		return NULL;
	}

	Frame * frame = (*it).second;
	return frame->pic;
}
	
int Animation::getWidth() const{
	if ( current_frame )
		return current_frame->getWidth();

	return 0;
}

int Animation::getHeight() const{
	if ( current_frame )
		return current_frame->getHeight();
	return 0;
}
	
const Bitmap * Animation::getCurrentFrame() const {
	return current_frame;
}

const string Animation::getCurrentFramePath() const {
	return current_frame->getPath();
}

void Animation::DrawLit( int x, int y, Bitmap * work ){
	doDraw( x, y, LitBitmap( *current_frame ), work );
}

void Animation::DrawLitFlipped( int x, int y, Bitmap * work ){
	doDrawFlipped( x, y, LitBitmap( *current_frame ), work );
}

void Animation::doDraw( int x, int y, const Bitmap & frame, Bitmap * work ){
	int w = frame.getWidth() / 2;
	int h = frame.getHeight();

	/*
	if ( Global::globalDebug() ){
		work->rectangle( x, y, x+range, y+1, Bitmap::makeColor(255,255,255) );
	}
	*/

	/*
	x += offset_x;
	y += offset_y;
	*/

	frame.draw( x - w, y - h, *work );

	// cout<<"Animation: "<<this<<" X1: "<<attack_x1<< " X2: "<<attack_x2<< " Y1: "<<attack_y1<< " Y2: "<<attack_y2<<endl;

	// work->rectangle( x+bbox_x1-w, y+bbox_y1-h, x+bbox_x2-w, y+bbox_y2-h, Bitmap::makeColor(255,255,0) );
	if ( Global::getDebug() > 5 ){
		work->rectangle( x - w, y - h, x + w, y, Bitmap::makeColor( 255, 255, 255 ) );
		work->rectangle( x+attack.getX1()-w, y+attack.getY1()-h, x+attack.getX2()-w, y+attack.getY2()-h, Bitmap::makeColor(255,0,0) );
		// current_collide->draw( *work, x-w, y-h );
		ECollide * c = getCollide( Object::FACING_RIGHT );
		if ( c != NULL ){
			c->draw( *work, x - w, y - h, Bitmap::makeColor( 255, 255, 255 ) );
		}
	}

	// work->rectangle( bbox_x1, bbox_y1, bbox_x2, bbox_y2, Bitmap::makeColor(255,255,0) );
	// cout<<"BBox. X1: "<<bbox_x1<<" Y1:"<<bbox_y1<<" X2:"<<bbox_x2<<" Y2:"<<bbox_y2<<endl;
}

void Animation::Draw( int x, int y, Bitmap * work ){

	doDraw( x, y, *current_frame, work );

#if 0
	int w = current_frame->getWidth() / 2;
	int h = current_frame->getHeight();

	if ( global_debug ){
		work->rectangle( x, y, x+range, y+1, Bitmap::makeColor(255,255,255) );
	}

	x += offset_x;
	y += offset_y;

	current_frame->draw( x-w, y-h, *work );

	// cout<<"Animation: "<<this<<" X1: "<<attack_x1<< " X2: "<<attack_x2<< " Y1: "<<attack_y1<< " Y2: "<<attack_y2<<endl;

	// work->rectangle( x+bbox_x1-w, y+bbox_y1-h, x+bbox_x2-w, y+bbox_y2-h, Bitmap::makeColor(255,255,0) );
	if ( global_debug ){
		work->rectangle( x+attack.getX1()-w, y+attack.getY1()-h, x+attack.getX2()-w, y+attack.getY2()-h, Bitmap::makeColor(255,0,0) );
	}

	// current_collide->draw( work, x-w, y-h );
	// work->rectangle( bbox_x1, bbox_y1, bbox_x2, bbox_y2, Bitmap::makeColor(255,255,0) );
	// cout<<"BBox. X1: "<<bbox_x1<<" Y1:"<<bbox_y1<<" X2:"<<bbox_x2<<" Y2:"<<bbox_y2<<endl;
#endif

}

void Animation::doDrawFlipped( int x, int y, const Bitmap & frame, Bitmap * work ){

	int w = frame.getWidth() / 2;
	int h = frame.getHeight();

	/*
	if ( Global::globalDebug() ){
		work->rectangle( x, y, x-range, y+1, Bitmap::makeColor(255,255,255) );
		// work->printfNormal( x, y+2, Bitmap::makeColor(255,255,255), "%s", current_frame->getPath().c_str() );
	}
	*/

	/*
	x -= offset_x;
	y += offset_y;
	*/

	frame.drawHFlip( x-w, y-h, *work );

	// work->rectangle( x-bbox_x1+w, y-bbox_y1+h, x-bbox_x2+w, y-bbox_y2+h, Bitmap::makeColor(255,255,0) );
	// work->circleFill( x, y-h, 4, Bitmap::makeColor(255,255,255) );
	if ( Global::getDebug() > 5 ){
		work->rectangle( x - w, y - h, x + w, y, Bitmap::makeColor( 255, 255, 255 ) );
		work->rectangle( x-attack.getX1()+w, y+attack.getY1()-h, x-attack.getX2()+w, y+attack.getY2()-h, Bitmap::makeColor(255,0,0) );
		// current_collide->draw( *work, x-w, y-h, true );
		ECollide * c = getCollide( Object::FACING_LEFT );
		if ( c != NULL ){
			c->draw( *work, x - w, y - h, Bitmap::makeColor( 255, 255, 255 ), true );
		}
	}

	// cout<<"BBox. X1: "<<bbox_x1<<" Y1:"<<bbox_y1<<" X2:"<<bbox_x2<<" Y2:"<<bbox_y2<<endl;
}

void Animation::DrawFlipped( int x, int y, Bitmap * work ){

	doDrawFlipped( x, y, *current_frame, work );
	
#if 0
	int w = current_frame->getWidth() / 2;
	int h = current_frame->getHeight();

	if ( global_debug ){
		work->rectangle( x, y, x-range, y+1, Bitmap::makeColor(255,255,255) );
		// work->printfNormal( x, y+2, Bitmap::makeColor(255,255,255), "%s", current_frame->getPath().c_str() );
	}

	x -= offset_x;
	y += offset_y;

	current_frame->drawHFlip( x-w, y-h, *work );

	// work->rectangle( x-bbox_x1+w, y-bbox_y1+h, x-bbox_x2+w, y-bbox_y2+h, Bitmap::makeColor(255,255,0) );
	// work->circleFill( x, y-h, 4, Bitmap::makeColor(255,255,255) );
	if ( global_debug ){
		work->rectangle( x-attack.getX1()+w, y+attack.getY1()-h, x-attack.getX2()+w, y+attack.getY2()-h, Bitmap::makeColor(255,0,0) );
	}

	// current_collide->draw( work, x-w, y-h, true );
	// cout<<"BBox. X1: "<<bbox_x1<<" Y1:"<<bbox_y1<<" X2:"<<bbox_x2<<" Y2:"<<bbox_y2<<endl;

#endif
}
	
/*
void Animation::setOffsetX( const int x ){
	// cout<<"Setting offset x to "<<x<<endl;
	offset_x = x;
}

void Animation::setOffsetY( const int y ){
	// cout<<"Setting offset y to "<<y<<endl;
	offset_y = y;
}
*/

/* the parent will jump up( the Y direction ) based on their
 * own starting y velocity so the vy parameter is not really used.
 * maybe this should call parent->jump( vx, vy, vz ) instead..
 */
void Animation::jump( double vx, double vy, double vz ){
	if ( parent ){
		parent->doJump( vx, vz );
	}
}

void Animation::contacted(){
	if ( contact )
		contact->play();
}

void Animation::moveX( const int x ){
	if ( parent ){
		// cout<<"Moving by "<<x<<endl;
		parent->moveX( x );
		/*
		if ( parent->getFacing() == Object::FACING_LEFT ){
			parent->moveX( x );
		} else	parent->moveX( -x );
		*/
	}
}

void Animation::moveY( const int y ){
	if ( parent )
		parent->moveY( y );
}

void Animation::moveZ( const int z ){
	if ( parent )
		parent->moveZ( z );
}

void Animation::setX( const int x ){
	if ( parent )
		parent->setX( x );
}

void Animation::setY( const int y ){
	if ( parent )
		parent->setY( y );
}

void Animation::setZ( const int z ){
	if ( parent )
		parent->setZ( z );
}

void Animation::setStatus( const int status ){
	if ( parent )
		parent->setStatus( status );
}

void Animation::setFrame( const string & path ){
    if ( frames.find(path) != frames.end() ){
        Frame * x = frames[ path ];
        current_frame = x->pic;
        current_collide = x->collide;
    } else {
        Global::debug( 0 ) <<"No frame "<<path<<endl;
    }
}

void Animation::setFrame( const int fr ){
	map< string, Frame * >::iterator it;
	int i = 0;
	for ( it = frames.begin(); it != frames.end() && i < fr; it++, i++ );

	if ( it == frames.end() ){
		Global::debug( 0 ) <<"No frame "<<fr<<endl;
	}

	Frame * x = (*it).second;
	current_frame = x->pic;
	current_collide = x->collide;
}
	
/*
void Animation::setAttack( int x1, int y1, int x2, int y2 ){
	attack_x1 = x1 + offset_x;
	attack_y1 = y1 + offset_y;
	attack_x2 = x2 + offset_x;
	attack_y2 = y2 + offset_y;
}
*/

void Animation::setBBox( int x1, int y1, int x2, int y2 ){
	bbox_x1 = x1 + offset_x;
	bbox_y1 = y1; // + offset_y;
	bbox_x2 = x2 + offset_x;
	bbox_y2 = y2;// + offset_y;
}
	
void Animation::reset(){
	current_event = events.begin();

	
	/* set the delay to 0 in case there the animation is reset
	 * before it completes
	 */
	delay_counter = 0;
	offset_x = 0;
	offset_y = 0;
	
	// cout<<"Reseting "<<getName()<<endl;
	
	/* reset the frame to the first picture. 
	 * This gets around the problem when the animation changes
	 * but the animation hasnt changed frames yet, so when it gets drawn
	 * current_frame is still equal to the last frame in the animation
	 */
	if ( ! frames.empty() ){
		setFrame( 0 );
	}
	
	while ( delay_counter <= 0 && current_event != events.end() ){
		(*current_event)->Interact( this );
		if ( delay_counter <= 0 )
			current_event++;
	}

}

bool Animation::Act(){

	/*
	if ( current_event == events.end() )
		current_event = events.begin();
	*/

	if ( delay_counter > 0 ){
		delay_counter--;
		if ( delay_counter <= 0 )
			current_event++;
	}
	while ( delay_counter <= 0 && current_event != events.end() ){
		(*current_event)->Interact( this );
		if ( delay_counter <= 0 )
			current_event++;
	}
	if ( delay_counter <= 0 )
		return current_event == events.end();
	return false;
}
	
bool Animation::empty(){
	return current_event == events.end();
}

Animation::~Animation(){
    // cout<<"Animation destructor "<<getName()<<":"<<this<<endl;
    if ( own_events ){
        // Global::debug( 1 ) << "Destroy animation events" << endl;
        for ( vector< AnimationEvent * >::iterator it = events.begin(); it != events.end(); it++ )
            delete *it;

        for ( map<string, Sound * >::iterator it = sounds.begin(); it != sounds.end(); it++ ){
            delete (*it).second;
        }

        delete contact;
    }
    if ( own_bitmaps ){
        // Global::debug( 1 ) << "Destroy animation bitmaps" << endl;
        for ( map< string, Frame * >::iterator it = frames.begin(); it != frames.end(); it++ ){
            Frame * x = (*it).second;
            // cout<<"Deleting frame "<<x<<" path = "<<x->pic->getPath()<<endl;
            delete x;
        }
    }

    if ( attack_collide )
        delete attack_collide;
}

}
