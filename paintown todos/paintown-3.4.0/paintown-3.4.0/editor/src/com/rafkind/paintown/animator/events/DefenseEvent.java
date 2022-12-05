package com.rafkind.paintown.animator.events;

import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import com.rafkind.paintown.animator.Animation;
import com.rafkind.paintown.animator.DrawArea;
import com.rafkind.paintown.animator.BoundingBox;
import com.rafkind.paintown.Token;
import com.rafkind.paintown.Lambda0;
import com.rafkind.paintown.animator.events.AnimationEvent;
import org.swixml.SwingEngine;

public class DefenseEvent implements AnimationEvent {
    private class Defense{
        public Defense(){
        }

        public int x1;
        public int y1;
        public int x2;
        public int y2;
    }

    java.util.List<Defense> boxes;

    private Lambda0 onDestroy = new Lambda0(){
        public Object invoke(){
            return null;
        }
    };

    public DefenseEvent(){
        this.boxes = new ArrayList();
    }

    public Defense parse(Token token){
        Defense defense = new Defense();

        Token x1_token = token.findToken("x1");
        if (x1_token != null){
            defense.x1 = x1_token.readInt(0);
        }

        Token y1_token = token.findToken("y1");
        if (y1_token != null){
            defense.y1 = y1_token.readInt(0);
        }

        Token x2_token = token.findToken("x2");
        if (x2_token != null){
            defense.x2 = x2_token.readInt(0);
        }

        Token y2_token = token.findToken("y2");
        if (y2_token != null){
            defense.y2 = y2_token.readInt(0);
        }

        return defense;
    }

    public void loadToken(Token token){
        this.boxes = new ArrayList();
        Defense defense = new Defense();

        Token x1_token = token.findToken("x1");
        if (x1_token != null){
            defense.x1 = x1_token.readInt(0);
        }

        Token y1_token = token.findToken("y1");
        if (y1_token != null){
            defense.y1 = y1_token.readInt(0);
        }

        Token x2_token = token.findToken("x2");
        if (x2_token != null){
            defense.x2 = x2_token.readInt(0);
        }

        Token y2_token = token.findToken("y2");
        if (y2_token != null){
            defense.y2 = y2_token.readInt(0);
        }

        if (defense.x1 != 0 || defense.y1 != 0 ||
            defense.x2 != 0 || defense.y2 != 0){
            boxes.add(defense);
            }

        for (Token box : token.findTokens("box")){
            boxes.add(parse(box));
        }
    }

    public void interact(Animation animation){
        if (boxes.isEmpty()){
            animation.setDefense(new BoundingBox(0, 0, 0, 0));
        } else {
            Defense defense = boxes.get(0);
            animation.setDefense(new BoundingBox(defense.x1, defense.y1, defense.x2, defense.y2));
        }
    }

    public String getName(){
        return getToken().toString();
    }

    private JPanel getEditor(final Animation animation, final DrawArea area, final Defense defense){
        SwingEngine engine = new SwingEngine("animator/event-defense.xml");
        // ((JPanel)engine.getRootComponent()).setSize(200,150);

        final JSpinner x1spin = (JSpinner) engine.find( "x1" );
        x1spin.setValue(new Integer(defense.x1));
        x1spin.addChangeListener( new ChangeListener(){
            public void stateChanged(ChangeEvent changeEvent){
                defense.x1 = ((Integer)x1spin.getValue()).intValue();
                interact(animation);
                animation.forceRedraw();
            }
        });
        final JSpinner y1spin = (JSpinner) engine.find( "y1" );
        y1spin.setValue(new Integer(defense.y1));
        y1spin.addChangeListener( new ChangeListener() {
            public void stateChanged(ChangeEvent changeEvent){
                defense.y1 = ((Integer)y1spin.getValue()).intValue();
                interact(animation);
                animation.forceRedraw();
            }
        });
        final JSpinner x2spin = (JSpinner) engine.find( "x2" );
        x2spin.setValue(new Integer(defense.x2));
        x2spin.addChangeListener( new ChangeListener(){
            public void stateChanged(ChangeEvent changeEvent){
                defense.x2 = ((Integer)x2spin.getValue()).intValue();
                interact( animation );
                animation.forceRedraw();
            }
        });
        final JSpinner y2spin = (JSpinner) engine.find( "y2" );
        y2spin.setValue(new Integer(defense.y2));
        y2spin.addChangeListener( new ChangeListener(){
            public void stateChanged(ChangeEvent changeEvent){
                defense.y2 = ((Integer)y2spin.getValue()).intValue();
                interact(animation);
                animation.forceRedraw();
            }
        });

        final JButton toggle = (JButton) engine.find("toggle");
        toggle.addActionListener(new AbstractAction(){
            boolean toggled = false;
            MouseInputAdapter listener = new MouseInputAdapter(){
                public void mousePressed(MouseEvent e){
                    defense.x1 = (int)(e.getX() / area.getScale() - area.getCenterX() + animation.getWidth() / 2 - animation.getOffsetX());
                    defense.y1 = (int)(e.getY() / area.getScale() - area.getCenterY() + animation.getHeight() - animation.getOffsetY());
                    x1spin.setValue(new Integer(defense.x1));
                    y1spin.setValue(new Integer(defense.y1));
                    interact(animation);
                    animation.forceRedraw();
                }

                public void mouseDragged(MouseEvent e){
                    defense.x2 = (int)(e.getX() / area.getScale() - area.getCenterX() + animation.getWidth() / 2 - animation.getOffsetX());
                    defense.y2 = (int)(e.getY() / area.getScale() - area.getCenterY() + animation.getHeight() - animation.getOffsetY());
                    x2spin.setValue(new Integer(defense.x2));
                    y2spin.setValue(new Integer(defense.y2));
                    interact(animation);
                    animation.forceRedraw();
                }
            };

            public void actionPerformed(ActionEvent event){
                if (toggled){
                    toggle.setText("Draw defense box");
                    area.enableMovement();
                    area.removeMouseListener(listener);
                    area.removeMouseMotionListener(listener);
                    onDestroy = new Lambda0(){
                        public Object invoke(){
                            return null;
                        }
                    };
                } else {
                    toggle.setText("Stop drawing");
                    area.disableMovement();
                    area.addMouseListener(listener);
                    area.addMouseMotionListener(listener);
                    onDestroy = new Lambda0(){
                        public Object invoke(){
                            area.removeMouseListener(listener);
                            area.removeMouseMotionListener(listener);
                            area.enableMovement();
                            return null;
                        }
                    };
                }

                toggled = ! toggled;
            }
        });

        return (JPanel)engine.getRootComponent();

    }

    public JPanel getEditor(final Animation animation, final DrawArea area){
        if (boxes.isEmpty()){
            boxes.add(new Defense());
        }

        return getEditor(animation, area, boxes.get(0));
    }
    
    public void destroy(){
        onDestroy.invoke_();
    }

    public Token getToken(){
        Token temp = new Token();
        temp.addToken(new Token("defense"));
        for (Defense defense : boxes){
            if (defense.x1 == 0 && defense.y1 == 0 && defense.x2 == 0 && defense.y2 == 0) {
                // temp.addToken(new Token("box"));
            } else {
                Token box = new Token();
                temp.addToken(box);
                box.addToken(new Token("box"));
                box.addToken(new String[]{"x1",Integer.toString(defense.x1)});
                box.addToken(new String[]{"y1",Integer.toString(defense.y1)});
                box.addToken(new String[]{"x2",Integer.toString(defense.x2)});
                box.addToken(new String[]{"y2",Integer.toString(defense.y2)});
            }
        }

        return temp;
    }
}
