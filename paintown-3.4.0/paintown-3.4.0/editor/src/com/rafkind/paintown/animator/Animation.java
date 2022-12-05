package com.rafkind.paintown.animator;

import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Vector;
import java.util.HashMap;

import com.rafkind.paintown.animator.events.*;

import com.rafkind.paintown.Token;
import com.rafkind.paintown.exception.*;
import com.rafkind.paintown.Lambda1;

import java.awt.image.*;
import java.awt.Color;

import java.awt.Graphics;
import javax.swing.JComponent;

public class Animation implements Runnable {
	
	private String name;
	private boolean alive = true;
	private boolean running;
	private List drawables;
	private List notifiers;
	private Vector events;
	private String sequence;
	private BufferedImage image;
	private BoundingBox attackArea;
	private BoundingBox defenseArea;
	private int eventIndex;
	private int delay;
	private int delayTime;
	private int x;
	private int y;
	private int offsetX;
	private int offsetY;
	private String baseDirectory;
	private int range;
	private String type;
	private double animationSpeed;
	/* when something is changed in the animation 'listeners' are notified */
	private List listeners;
	
	private Vector keys;
	private HashMap remapData;

	public Animation(){
		drawables = new ArrayList();
		events = new Vector();
		/* give the animation something so it rests a little bit */
		events.add( new NopEvent() );
		notifiers = new ArrayList();
		listeners = new ArrayList();
		image = null;
		animationSpeed = 1.0;
		sequence = "none";
		attackArea = new BoundingBox( 0, 0, 0, 0 );
		defenseArea = new BoundingBox( 0, 0, 0, 0 );
		keys = new Vector();
		baseDirectory = ".";
		type = "none";
		name = "New Animation";
	}

	public Animation( String name ){
		this();
		this.name = name;
	}

	public Animation( Vector events ){
		this();
		this.events = events;
	}

	public Animation( Token data ) throws LoadException {
		this();
		loadData( data );
	}

	public void setType( String s ){
		type = s;

		updateAll();
	}

	public String getType(){
		return type;
	}
	
	public int getHeight(){
		if ( image != null ){
			return image.getHeight( null );
		}
		return 0;
	}
	
	public int getWidth(){
		if ( image != null ){
			return image.getWidth( null );
		}
		return 0;
	}

	public void setRange( int r ){
		range = r;

		updateAll();
	}

	public int getRange(){
		return range;
	}

	public void addKey( String key ){
		keys.add( key );
		updateAll();
	}

	public void setMap( HashMap remap ){
		this.remapData = remap;
	}

	public HashMap getMap(){
		return this.remapData;
	}

	public void setSequence( String s ){
		sequence = s;
		updateAll();
	}

	public String getSequence(){
		return sequence;
	}

	public void removeKey( int index ){
		keys.remove( index );
		updateAll();
	}

	public Vector getKeys(){
		return keys;
	}

	public String toString(){
		return name != null ? name : "No name set";
	}

	public void setAttack( BoundingBox attack ){
		attackArea = attack;
		updateAll();
	}
	
    public void setDefense( BoundingBox defense ){
		defenseArea = defense;
		updateAll();
	}

	private BufferedImage currentImage(){
		return image;
	}

	public void setName( String s ){
		this.name = s;
		updateAll();
	}

	public String getName(){
		return name;
	}
	
	public void addEventNotifier( Lambda1 lambda ){
		notifiers.add( lambda );
	}
	
	public void removeEventNotifier( Lambda1 lambda ){
		notifiers.remove( lambda );
	}

	public synchronized void setImage( BufferedImage image ){
		this.image = image;
		updateDrawables();
		updateAll();
	}

	public void forceRedraw(){
		updateDrawables();
	}

	public void addChangeUpdate( Lambda1 update ){
		listeners.add( update );
	}

	public void removeChangeUpdate( Lambda1 update ){
		listeners.remove( update );
	}

	private void updateAll(){
		for ( Iterator it = listeners.iterator(); it.hasNext(); ){
			Lambda1 update = (Lambda1) it.next();
			update.invoke_( this );
		}
	}

	/* tell the components that know about this animation that its time
	 * to redraw the canvas.
	 * for things like updating the current frame or the attack box
	 */
	private void updateDrawables(){
		synchronized( drawables ){
			for ( Iterator it = drawables.iterator(); it.hasNext(); ){
				JComponent draw = (JComponent) it.next();	
				draw.repaint();
			}
		}
	}

	public synchronized void kill(){
		alive = false;
	}

	private synchronized boolean isAlive(){
		return alive;
	}

	private synchronized boolean isRunning(){
		return running;
	}

	public synchronized void stopRunning(){
		running = false;
	}
	
	public synchronized void startRunning(){
		running = true;
	}

	public int addEvent( AnimationEvent e, int index ){
		synchronized( events ){
			events.add( index, e );
			return index;
		}
	}

	public int addEvent( AnimationEvent e ){
		synchronized( events ){
			return addEvent( e, events.size() );
		}
	}

	public void clearEvents(){
		synchronized( events ){
			events.clear();
		}
	}

	/* swap position of e1 and e2 within event structure */
	public void swapEvents( int e1, int e2 ){
		synchronized( events ){
			Object a1 = events.get( e1 );
			Object a2 = events.get( e2 );
			events.set( e1, a2 );
			events.set( e2, a1 );
		}
		
	}

	/* return a new list that contains the current events
	 * do not edit the returned list, to add new events use
	 * addEvent/removeEvent
	 */ 
	public Vector<AnimationEvent> getEvents(){
		synchronized(events){
			return new Vector<AnimationEvent>(events);
		}
	}

	public void removeEvent( AnimationEvent e ){
		synchronized( events ){
			events.remove( e );
		}
	}

	public void removeEvent( int i ){
		synchronized( events ){
			events.remove( i );
		}
	}

	public String getBaseDirectory(){
		return baseDirectory;
	}
	
	public void setBaseDirectory( String b ){
		baseDirectory = b;
	}

	public void setAnimationSpeed( double n ){
		animationSpeed = n;
	}

	public double getAnimationSpeed(){
		return animationSpeed;
	}

	public synchronized void draw( Graphics g, int x, int y ){
		int trueX = x + this.x + this.offsetX - getWidth() / 2;
		int trueY = y + this.y + this.offsetY;
		if ( currentImage() != null ){
			g.drawImage( currentImage(), trueX, trueY, null );
		}

		if ( getRange() != 0 ){
			Color old = g.getColor();
			g.setColor( new Color( 255, 255, 255 ) );
			g.fillRect( trueX - offsetX + getWidth() / 2, trueY + getHeight(), getRange(), 5 );
			g.setColor( old );
		}

		if ( ! attackArea.empty() ){
			attackArea.render( g, trueX, trueY, new Color(236, 30, 30));
		}
		
        if ( ! defenseArea.empty() ){
			defenseArea.render( g, trueX, trueY, new Color(51, 133, 243));
		}
	}

	public synchronized void setImageX( int x ){
		this.x = x;
	}
	
	public synchronized void setImageY( int y ){
		this.y = y;
	}

	public synchronized void moveX( int x ){
		this.x += x;
	}
	
	public synchronized void moveY( int y ){
		this.y += y;
	}
	
	public synchronized void setOffsetX( int x ){
		this.offsetX = x;
	}

        public synchronized int getOffsetX(){
            return this.offsetX;
        }
	
	public synchronized void setOffsetY( int y ){
		this.offsetY = y;
	}
        
        public synchronized int getOffsetY(){
            return this.offsetY;
        }

	public void addDrawable( JComponent draw ){
		synchronized( drawables ){
			drawables.add( draw );
		}
	}
	
	public void removeDrawable( JComponent draw ){
		synchronized( drawables ){
			drawables.remove( draw );
		}
	}

	private void rest( int m ){
		try{
			Thread.sleep( m );
		} catch ( Exception e ){
		}
	}

	private int previous( int index, int max ){
		int last = index - 1;
		if ( last < 0 ){
			last = max - 1;
		}
		return last;
	}

	private int next( int index, int max ){
		int next = index + 1;
		if ( next >= max ){
			next = 0;
		}
		return next;
	}

	private void updateEvent( AnimationEvent event ){
		event.interact( this );
		for ( Iterator it = notifiers.iterator(); it.hasNext(); ){
			Lambda1 lambda = (Lambda1) it.next();
			lambda.invoke_( event );
		}
	}

	/* can be called to step backward through the animation */
	public void previousEvent(){
		synchronized( events ){
			if ( ! events.isEmpty() ){
				eventIndex = previous( eventIndex, events.size() );
				if ( eventIndex == 0 ){
					setImageX( 0 );
					setImageY( 0 );
				}
				updateEvent( (AnimationEvent) events.get( eventIndex ) );
			}
		}
	}
	
	/* can be called to step foreward through the animation */
	public void nextEvent(){
		synchronized( events ){
			if ( ! events.isEmpty() ){
				eventIndex = next( eventIndex, events.size() );
				if ( eventIndex == 0 ){
					setImageX( 0 );
					setImageY( 0 );
				}
				updateEvent( (AnimationEvent) events.get( eventIndex ) );
			}
		}
	}

        private void checkIndex(){
            if (eventIndex < 0){
                eventIndex = 0;
            }
            if (eventIndex >= events.size()){
                eventIndex = events.size() - 1;
            }
        }

        public void nextEvent( AnimationEvent event ){
            synchronized( events ){
                checkIndex();
                if (events.contains(event)){
                    while ((AnimationEvent) events.get(eventIndex) != event){
                        nextEvent();
                    }
                }
            }
        }

	public void setDelay( int delay ){
		this.delay = delay;
	}

	public int getDelay(){
		return delay;
	}

	public int getDelayTime(){
		return delayTime;
	}

	public void setDelayTime( int i ){
		delayTime = i;
	}

	public void delay(){
		delayTime = (int)(getDelay() * getAnimationSpeed());
	}

	/* if the animation doesnt contain something that will cause delay this
	 * thread will just chew up cpu time
	 */
	public void run(){
		while ( isAlive() ){
			if ( isRunning() && ! events.isEmpty() ){
				nextEvent();
				if ( getDelayTime() != 0 ){
					rest( (int)(getDelayTime() * 1000.0 / 90.0) );
				}
				setDelayTime( 0 );
			} else {
				rest(200);
			}
		}
	}

	public void loadData(Token token) throws LoadException {
		if ( ! token.getName().equals( "anim" ) ){
			throw new LoadException( "Starting token is not 'anim'" );
		}
		
		Token nameToken = token.findToken( "name" );
		if ( nameToken != null ){
			setName( nameToken.readString(0) );
		}

		Token sequenceToken = token.findToken( "sequence" );
		if ( sequenceToken != null ){
			setSequence( sequenceToken.readString( 0 ) );
		}
		
		Token typeToken = token.findToken( "type" );
		if ( typeToken != null ){
			setType( typeToken.readString( 0 ) );
		}
		
		Token keyToken = token.findToken( "keys" );
		if ( keyToken != null ){
                    for (Iterator it = keyToken.iterator(); it.hasNext(); ){
                        String key = ((Token)it.next()).toString();
                        keys.add(key);
                    }
                    /*
			try{
				for(int i = 0; ; i += 1 ){
					String temp = keyToken.readString(i);
					if ( temp != null ){
						keys.add( temp );
					} else {
						break;
					}
				}
			} catch(Exception e) {
				e.printStackTrace();
			}
                        */
		}
		
		Token rangeToken = token.findToken( "range" );
		if ( rangeToken != null ){
			range = rangeToken.readInt(0);
		}
		
		Token basedirToken = token.findToken( "basedir" );
		if ( basedirToken != null ){
			setBaseDirectory( basedirToken.readString( 0 ) );
		}
		
		events.clear();
		for ( Iterator it = token.getTokens().iterator(); it.hasNext(); ){
			Token t = (Token) it.next();
			AnimationEvent ae = EventFactory.getEvent(t.getName());
			if( ae != null ){
				ae.loadToken(t);
				events.add( ae );
			}
		}
	}

	public Token getToken(){
		Token token = new Token();
		token.addToken( new Token( "anim" ) );
		
		token.addToken(new String[]{"name", "\"" + name + "\""});
		if ( ! type.equals("none") ){
			token.addToken(new String[]{"type", type});
		}

		if ( ! sequence.equals( "none" ) ){
			token.addToken( new String[]{ "sequence", "\"" + sequence + "\"", "junk" } );
		}

		if ( ! keys.isEmpty() ){
			Token keyToken = new Token( "keys" );
			keyToken.addToken( new Token( "keys"));
			for ( Iterator it = keys.iterator(); it.hasNext(); ){
				String key = (String) it.next();
				keyToken.addToken( new Token( key ) );
			}
			token.addToken( keyToken );
		}

		if ( range != 0 ){
			token.addToken(new String[]{"range", Integer.toString(range)});
		}

		if( ! baseDirectory.equals( "" ) ){
			token.addToken(new String[]{"basedir", baseDirectory});
		}

		for ( Iterator it = events.iterator(); it.hasNext(); ){
			AnimationEvent event = (AnimationEvent) it.next();
			token.addToken( event.getToken() );
		}
		
		return token;
	}
}
