package com.rafkind.paintown;

import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;

import java.io.*;

import java.util.Iterator;
import java.util.List;
import java.util.ArrayList;
import java.util.HashMap;

import com.rafkind.paintown.level.Character;
import com.rafkind.paintown.level.Level;
import com.rafkind.paintown.level.Block;

import org.swixml.SwingEngine;

public class CharacterEditor implements PropertyEditor {
	private static HashMap files = new HashMap();
	private Character character;

	public CharacterEditor( Character i ){
		this.character = i;
	}

	private int findBlock( Level level ){
		int i = 1;
		for ( Iterator it = level.getBlocks().iterator(); it.hasNext(); ){
			Block b = (Block) it.next();
			if ( b.hasThing( character ) ){
				return i;
			}
			i += 1;
		}
		return i;
	}

	private Block getBlock( int num, Level level ){
		return (Block) level.getBlocks().get( num - 1 );
	}

	private String convertAggressionLevel( int level ){

		if ( level == -1 ){
			return "default";
		}
		if ( level > 90 ){
			return "calm";
		}
		if ( level > 80 ){
			return "unpleasant";
		}
		if ( level > 70 ){
			return "mild";
		}
		if ( level > 60 ){
			return "angry";
		}
		if ( level > 50 ){
			return "hot headed";
		}
		if ( level > 40 ){
			return "wild";
		}
		if ( level > 30 ){
			return "furious";
		}
		if ( level > 20 ){
			return "temper tantrum";
		}
		return "insane";
	}

	private abstract class RandomNameAction extends AbstractAction {
		private List names;

		private List readFile( BufferedReader reader ) throws IOException {
			List l = new ArrayList();
			while ( reader.ready() ){
				l.add( reader.readLine() );
			}
			return l;
		}

		public RandomNameAction( String file ){
			if ( files.get( file ) == null ){
				try{
					files.put( file, readFile( new BufferedReader( new InputStreamReader( this.getClass().getResourceAsStream( "/" + file ) ) ) ) );
					names = (List) files.get( file );
				} catch ( IOException ie ){
					ie.printStackTrace();
					names = new ArrayList();
					names.add( "TempName" );
				}
			} else {
				names = (List) files.get( file );
			}
		}

		public String generateName(){
			return (String) names.get( (int)(Math.random() * (names.size() - 1)) );
		}
	}

	public JComponent createPane( final Level level, final Lambda0 closeProc ){
		final SwingEngine engine = new SwingEngine( "character.xml" );
		final JTextField name = (JTextField) engine.find( "name" );
		name.setText( character.getName() );
		final JTextField health = (JTextField) engine.find( "health" );
		health.setText( String.valueOf( character.getHealth() ) );
		final JTextField x = (JTextField) engine.find( "x" );
		x.setText( String.valueOf( character.getX() ) );
		final JTextField y = (JTextField) engine.find( "y" );
		y.setText( String.valueOf( character.getY() ) );
		final JTextField path = (JTextField) engine.find( "path" );
		path.setText( character.getPath() );
		path.setEditable( false );
		final JTextField aggression = (JTextField) engine.find( "aggression" );
		aggression.setText( String.valueOf( character.getAggression() ) );
		final JSpinner block = (JSpinner) engine.find( "block" );
		final JSlider aggressionScroll = (JSlider) engine.find( "aggression-slider" );
		final JLabel aggressionLevel = (JLabel) engine.find( "aggression-level" );
		final JButton boysName = (JButton) engine.find( "boys-name" );
		final JButton girlsName = (JButton) engine.find( "girls-name" );

		boysName.addActionListener( new RandomNameAction( "boys.txt" ){
			public void actionPerformed( ActionEvent event ){
				name.setText( generateName() );
			}
		});
		
		girlsName.addActionListener( new RandomNameAction( "girls.txt" ){
			public void actionPerformed( ActionEvent event ){
				name.setText( generateName() );
			}
		});

		if ( character.getAggression() == -1 ){
			aggressionScroll.setValue( aggressionScroll.getMaximum() );
		} else {
			aggressionScroll.setValue( character.getAggression() );
		}

		aggressionLevel.setText( "Aggression level: " + convertAggressionLevel( aggressionScroll.getValue() ) );

		aggressionScroll.addChangeListener( new ChangeListener(){
			public void stateChanged( ChangeEvent e ){
				int level = aggressionScroll.getValue();
				if ( level == 100 ){
					level = -1;
				}
				aggressionLevel.setText( "Aggression level: " + convertAggressionLevel( level ) );
				aggression.setText( String.valueOf( level ) );
			}
		});
		
		final Lambda1 update = new Lambda1(){
			public Object invoke( Object c ){
				Character guy = (Character) c;
				x.setText( String.valueOf( guy.getX() ) );
				y.setText( String.valueOf( guy.getY() ) );
				return null;
			}
		};

		character.addListener( update );

		
		block.setModel( new MinMaxSpinnerModel( findBlock( level ), 1, level.getBlocks().size() ) );
		final JSpinner map = (JSpinner) engine.find( "map" );
		map.setModel( new MinMaxSpinnerModel( character.getMap(), 0, character.getMaxMaps() ) );

		final JButton save = (JButton) engine.find( "save" );
		final JButton close = (JButton) engine.find( "close" );

		save.addActionListener( new AbstractAction(){
			public void actionPerformed( ActionEvent event ){
				int xInt = Integer.parseInt( x.getText() );
				int yInt = Integer.parseInt( y.getText() );
				character.setName( name.getText() );
				character.setHealth( Integer.parseInt( health.getText() ) );
				character.setX( xInt );
				character.setY( yInt );
				character.setMap( ((Integer) map.getValue()).intValue() );
				int a = Integer.parseInt( aggression.getText() );
				character.setAggression( a );
				Block b = getBlock( ((Integer) block.getValue()).intValue(), level );
				Block old = level.findBlock( character );
				if ( b != null && old != null && b != old ){
					old.removeThing( character );
					b.addThing( character );
				}

				character.removeListener( update );
				closeProc.invoke_();
			}
		});

		close.addActionListener( new AbstractAction(){
			public void actionPerformed( ActionEvent event ){
				character.removeListener( update );
				closeProc.invoke_();		
			}
		});

		return (JPanel) engine.getRootComponent();
	}
}
