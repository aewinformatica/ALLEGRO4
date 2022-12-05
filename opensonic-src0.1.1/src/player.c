/*
 * player.c - player module
 * Copyright (C) 2008-2009  Alexandre Martins < alemartf [.a.t.] gmail [.d.o.t.] com >
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <math.h>
#include "global.h"
#include "player.h"
#include "audio.h"
#include "actor.h"
#include "brick.h"
#include "enemy.h"
#include "item.h"
#include "util.h"
#include "input.h"
#include "sprite.h"
#include "level.h"



/* private data */
#define LOCKACCEL_NONE      0
#define LOCKACCEL_LEFT      1
#define LOCKACCEL_RIGHT     2
static int rings, hundred_rings;
static int lives;
static int score;
static char *get_sprite_id(int player_type); /* returns a static pointer */
static void update_shield(player_t *p);
static void update_glasses(player_t *p);
static void drop_glasses(player_t *p);
static void forget_bricks(int top, int right, int bottom, int left, brick_t **brick_up, brick_t **brick_upright, brick_t **brick_right, brick_t **brick_downright, brick_t **brick_down, brick_t **brick_downleft, brick_t **brick_left, brick_t **brick_upleft);
static int inside_loop(player_t *p);
static double old_school_angle(double ang);


/*
 * player_create()
 * Creates a player
 */
player_t *player_create(int type)
{
    int i;
    player_t *p = mallocx(sizeof *p);

    logfile_message("player_create(%d)", type);
    p->type = type;
    p->actor = actor_create();
    p->spin = p->spin_dash = p->braking = p->flying = p->climbing = p->landing = p->spring = FALSE;
    p->getting_hit = p->dieing = p->dead = p->blinking = FALSE;
    p->on_moveable_platform = FALSE;
    p->lock_accel = LOCKACCEL_NONE;
    p->flight_timer = p->blink_timer = p->death_timer = 0;

    p->glasses = actor_create();
    p->got_glasses = FALSE;

    p->shield = actor_create();
    p->shield_type = SH_NONE;

    p->invencible = FALSE;
    p->invtimer = 0;
    for(i=0; i<PLAYER_MAX_INVSTAR; i++) {
        p->invstar[i] = actor_create();
        actor_change_animation(p->invstar[i], sprite_get_animation("SD_INVSTAR", 0));
    }

    p->got_speedshoes = FALSE;
    p->speedshoes_timer = 0;

    p->disable_wall = PLAYER_WALL_NONE;
    p->entering_loop = FALSE;
    p->at_loopfloortop = FALSE;
    p->bring_to_back = FALSE;

    switch(p->type) {
        case PL_SONIC:
            p->actor->acceleration = 350;
            p->actor->maxspeed = 700;
            p->actor->jump_strength = 400;
            p->actor->input = input_create_keyboard(NULL);
            actor_change_animation( p->actor , sprite_get_animation("SD_SONIC", 0) );
            break;

        case PL_TAILS:
            p->actor->acceleration = 300;
            p->actor->maxspeed = 600;
            p->actor->jump_strength = 350;
            p->actor->input = input_create_keyboard(NULL);
            actor_change_animation( p->actor , sprite_get_animation("SD_TAILS", 0) );
            break;

        case PL_KNUCKLES:
            p->actor->acceleration = 300;
            p->actor->maxspeed = 600;
            p->actor->jump_strength = 350;
            p->actor->input = input_create_keyboard(NULL);
            actor_change_animation( p->actor , sprite_get_animation("SD_KNUCKLES", 0) );
            break;
    }

    hundred_rings = rings = 0;
    logfile_message("player_create() ok");
    return p;
}


/*
 * player_destroy()
 * Destroys a player
 */
void player_destroy(player_t *player)
{
    int i;

    for(i=0; i<PLAYER_MAX_INVSTAR; i++)
        actor_destroy(player->invstar[i]);

    actor_destroy(player->glasses);
    actor_destroy(player->actor);
    free(player);
}



/*
 * player_update()
 * Updates the player
 */
void player_update(player_t *player, brick_list_t *brick_list)
{
    if(player->blinking) {
        player->blink_timer += game_delta();
        player->actor->visible = (game_timer() % 250) < 125;
        if(player->blink_timer >= PLAYER_MAX_BLINK) {
            player->getting_hit = player->blinking = FALSE;
            player->actor->visible = TRUE;
        }
    }

    actor_move(player->actor, player_platform_movement(player, brick_list, level_gravity()));
}


/*
 * player_render()
 * Rendering function
 */
void player_render(player_t *player, v2d_t camera_position)
{
    actor_t *act = player->actor;
    v2d_t hot_spot = act->hot_spot;
    v2d_t position = act->position;
    v2d_t s_hot_spot = v2d_new(0,0);
    v2d_t starpos;
    int i, invangle[PLAYER_MAX_INVSTAR];
    double x, angoff, ang = act->angle, s_ang = 0;



    /* invencibility stars */
    if(player->invencible) {
        int maxf = sprite_get_animation("SD_INVSTAR", 0)->frame_count;
        player->invtimer += game_delta();

        for(i=0; i<PLAYER_MAX_INVSTAR; i++) {
            invangle[i] = (180*4) * game_timer()*0.001 + (i+1)*(360/PLAYER_MAX_INVSTAR);
            starpos.x = 30*cos(invangle[i]*PI/180);
            starpos.y = ((game_timer()+i*400)%2000)/40;
            starpos = v2d_rotate(starpos,ang);
            player->invstar[i]->position.x = act->position.x + starpos.x;
            player->invstar[i]->position.y = act->position.y - starpos.y + 5;
            actor_change_animation_frame(player->invstar[i], random(maxf));
        }

        if(player->invtimer >= PLAYER_MAX_INVENCIBILITY)
            player->invencible = FALSE;
    }


    /* shields and glasses */
    if(player->got_glasses)
        update_glasses(player);

    if(player->shield_type != SH_NONE)
        update_shield(player);



    /* player's specific routines (before rendering) */
    switch(player->type) {
        case PL_SONIC:
            break;

        case PL_TAILS:
            /* tails' jump hack */
            if(act->is_jumping && act->animation == sprite_get_animation("SD_TAILS", 3)) {
                int rotate = ((fabs(act->speed.x)>100) || input_button_down(act->input,IB_RIGHT) || input_button_down(act->input,IB_LEFT));
                int left = (act->mirror & IF_HFLIP);
                act->hot_spot = v2d_new(actor_image(act)->w*0.5, actor_image(act)->h*0.9);
                if(act->speed.y < 0) {
                    angoff = left ? 3*PI/2 : PI/2;
                    act->angle = ang + angoff;
                    if(rotate)
                        act->angle -= (left?-1:1) * (PI/2) * (act->jump_strength+act->speed.y)/act->jump_strength;
                    else
                        act->position.x -= actor_image(act)->h*(left?0.5:0.0);
                }
                else {
                    angoff = left ? PI/2 : 3*PI/2;
                    act->angle = ang + angoff;
                    if(rotate) {
                        if(act->speed.y < act->jump_strength)
                            act->angle += (left?-1:1) * (PI/2) * (act->jump_strength-act->speed.y)/act->jump_strength;
                    }
                    else
                        act->position.x += actor_image(act)->h*(left?0.1:-0.2);
                }

                /* fix shield position */
                if(player->shield_type != SH_NONE) {
                    v2d_t voff = v2d_rotate(v2d_new(left?-13:13,-13), -act->angle);
                    s_ang = player->shield->angle;
                    s_hot_spot = player->shield->hot_spot;
                    player->shield->position = v2d_add(act->position, voff);
                    /*player->shield->hot_spot = voff;*/
                    /*player->shield->angle = act->angle;*/
                }
            }
            break;

        case PL_KNUCKLES:
            break;
    }


    /* rendering */
    for(i=0;i<PLAYER_MAX_INVSTAR && player->invencible;i++) {
        if(invangle[i]%360 >= 180)
            actor_render(player->invstar[i], camera_position);
    }

    x = act->angle;
    act->angle = act->is_jumping ? x : old_school_angle(x);
    actor_render(act, camera_position);
    act->angle = x;

    if(player->got_glasses)
        actor_render(player->glasses, camera_position);
    if(player->shield_type != SH_NONE)
        actor_render(player->shield, camera_position);
    for(i=0;i<PLAYER_MAX_INVSTAR && player->invencible;i++) {
        if(invangle[i]%360 < 180)
            actor_render(player->invstar[i], camera_position);
    }


    /* player's specific routines (after rendering) */
    switch(player->type) {
        case PL_SONIC:
            break;

        case PL_TAILS:
            if(act->is_jumping && act->animation == sprite_get_animation("SD_TAILS", 3)) {
                act->position = position;
                act->angle = ang;
                act->hot_spot = hot_spot;

                if(player->shield_type != SH_NONE) {
                    player->shield->angle = s_ang;
                    player->shield->hot_spot = s_hot_spot;
                }
            }
            break;

        case PL_KNUCKLES:
            break;
    }
}



/*
 * player_platform_movement()
 * Platform movement. Returns
 * a delta_space vector.
 *
 * Note: the actor's hot spot must
 * be defined on its feet.
 */
v2d_t player_platform_movement(player_t *player, brick_list_t *brick_list, double gravity)
{
    actor_t *act = player->actor;
    double dt = game_delta();
    double max_y_speed = 480, friction = 0, gravity_factor = 1.0;
    double maxspeed = act->maxspeed;
    double desired_angle = 0;
    v2d_t ds = v2d_new(0,0);
    char *sprite_id = get_sprite_id(player->type);
    int pushing_a_wall;
    int angle_question;
    int was_jumping = FALSE;
    int is_walking = (player->actor->animation == sprite_get_animation(get_sprite_id(player->type), 1));
    int at_right_border = FALSE, at_left_border = FALSE;
    animation_t *animation = NULL;

    /* actor's collision detectors */
    int frame_width = actor_image(act)->w, frame_height = actor_image(act)->h;
    int slope = !((fabs(act->angle)<EPSILON)||(fabs(act->angle-PI/2)<EPSILON)||(fabs(act->angle-PI)<EPSILON)||(fabs(act->angle-3*PI/2)<EPSILON));
    double diff = -2, sqrsize = 2, top=0, middle=0, lateral=0;
    brick_t *brick_up, *brick_down, *brick_right, *brick_left;
    brick_t *brick_upright, *brick_downright, *brick_downleft, *brick_upleft;
    brick_t *brick_tmp;
    v2d_t up, upright, right, downright, down, downleft, left, upleft;
    v2d_t feet = act->position;
    switch(player->type) {
        case PL_SONIC:
            if(!slope) { top = 0.7; middle = 0.5; lateral = 0.4; }
            else       { top = 1.0; middle = 0.8; lateral = 0.5; }
            break;

        case PL_TAILS:
            if(!slope) { top = 0.7; middle = 0.5; lateral = 0.25; }
            else       { top = 1.0; middle = 0.7; lateral = 0.25; }
            break;

        case PL_KNUCKLES:
            if(!slope) { top = 0.7; middle = 0.5; lateral = 0.25; }
            else       { top = 1.0; middle = 0.7; lateral = 0.25; }
            break;
    }
    up        = v2d_add ( feet , v2d_rotate( v2d_new(0, -frame_height*top+diff), -act->angle) );
    down      = v2d_add ( feet , v2d_rotate( v2d_new(0, -diff), -act->angle) ); 
    left      = v2d_add ( feet , v2d_rotate( v2d_new(-frame_width*lateral+diff, -frame_height*middle), -act->angle) );
    right     = v2d_add ( feet , v2d_rotate( v2d_new(frame_width*lateral-diff, -frame_height*middle), -act->angle) );
    upleft    = v2d_add ( feet , v2d_rotate( v2d_new(-frame_width*lateral+diff, -frame_height*top+diff), -act->angle) );
    upright   = v2d_add ( feet , v2d_rotate( v2d_new(frame_width*lateral-diff, -frame_height*top+diff), -act->angle) );
    downleft  = v2d_add ( feet , v2d_rotate( v2d_new(-frame_width*lateral+diff, -diff), -act->angle) );
    downright = v2d_add ( feet , v2d_rotate( v2d_new(frame_width*lateral-diff, -diff), -act->angle) );
    actor_corners_set_floor_priority( (player->disable_wall & PLAYER_WALL_BOTTOM) ? FALSE : TRUE );
    actor_corners_ex(act, sqrsize, up, upright, right, downright, down, downleft, left, upleft, brick_list, &brick_up, &brick_upright, &brick_right, &brick_downright, &brick_down, &brick_downleft, &brick_left, &brick_upleft);
    actor_corners_restore_floor_priority();


    /* is the player dieing? */
    if(player->dieing) {
        act->speed.x = 0;
        act->speed.y = min(max_y_speed, act->speed.y+gravity*dt);
        act->mirror = IF_NONE;
        act->angle = 0;
        act->visible = TRUE;
        player->blinking = FALSE;
        player->death_timer += dt;
        player->dead = (player->death_timer >= 2.5);
        actor_change_animation(act, sprite_get_animation(sprite_id, 8));
        return v2d_new(0, act->speed.y*dt + 0.5*gravity*dt*dt);
    }
    else if(player->dead)
        return v2d_new(0,0);



    /* speed shoes */
    if(player->got_speedshoes) {
        if(player->speedshoes_timer > PLAYER_MAX_SPEEDSHOES)
            player->got_speedshoes = FALSE;
        else {
            maxspeed *= 1.5;
            player->speedshoes_timer += dt;
        }
    }


    /* disable walls? - useful on sonic loops */
    if(inside_loop(player)) {
        if(act->is_jumping)
            player->disable_wall &= ~PLAYER_WALL_BOTTOM;

        forget_bricks(player->disable_wall & PLAYER_WALL_TOP, player->disable_wall & PLAYER_WALL_RIGHT, player->disable_wall & PLAYER_WALL_BOTTOM, player->disable_wall & PLAYER_WALL_LEFT, &brick_up, &brick_upright, &brick_right, &brick_downright, &brick_down, &brick_downleft, &brick_left, &brick_upleft);

        /* loop floor workaround */
        /*if(0 && (player->disable_wall & PLAYER_WALL_BOTTOM) && fabs(act->speed.x) > EPSILON && fabs(act->speed.y) < EPSILON) {
            if(act->speed.x < 0) {
                if(brick_left && brick_left->brick_ref->angle == 0)
                    brick_left = NULL;
            }
            else {
                if(brick_right && brick_right->brick_ref->angle == 0)
                    brick_right = NULL;
            }
        }*/

        /* restoring brick_down if needed */
        if(!brick_down && fabs(act->speed.y)<EPSILON && !(player->disable_wall & PLAYER_WALL_BOTTOM) && (player->disable_wall & PLAYER_WALL_TOP))
            actor_corners(act, 1, 2*diff, brick_list, NULL, NULL, NULL, NULL, &brick_down, NULL, NULL, NULL);

        if(!brick_down && !act->is_jumping && fabs(act->speed.y)<EPSILON && (fabs(act->speed.x)<EPSILON || (!input_button_down(act->input, IB_RIGHT) && !input_button_down(act->input, IB_LEFT)))) {
            actor_corners_set_slope_priority(FALSE);
            actor_corners_ex(act, sqrsize, up, upright, right, downright, down, downleft, left, upleft, brick_list, NULL, NULL, NULL, &brick_downright, &brick_down, &brick_downleft, NULL, NULL);
            actor_corners_restore_slope_priority();
        }
    }


    /* disable spring mode */
    if(player->spring) {
        if((brick_down && (int)act->speed.y >= 0) || player->flying || player->climbing)
            player->spring = FALSE;
    }


    /* useful flags */
    pushing_a_wall = ((brick_right && input_button_down(act->input, IB_RIGHT)) || (brick_left && input_button_down(act->input, IB_LEFT))) && brick_down;
    player->on_moveable_platform = (v2d_magnitude(level_brick_move_actor(brick_down,act)) > EPSILON);


    /* wall collision - TODO: more testing? */
    if((fabs(act->angle) < EPSILON || fabs(act->angle-PI) < EPSILON)){ /* <-- test me */
        if(brick_right) {
            if(brick_right->brick_ref->angle % 90 == 0 && (act->speed.x > EPSILON || right.x > brick_right->x)) {
                act->speed.x = 0;
                act->position.x = brick_right->x + (feet.x-right.x);
                if(!act->is_jumping && !player->flying && !player->climbing && fabs(act->speed.y)<EPSILON)
                    animation = sprite_get_animation(sprite_id, pushing_a_wall ? 14 : 0);
            }
        }

        if(brick_left) {
            if(brick_left->brick_ref->angle % 90 == 0 && (act->speed.x < -EPSILON || left.x < brick_left->x+brick_left->brick_ref->image->w)) {
                act->speed.x = 0;
                act->position.x = (brick_left->x+brick_left->brick_ref->image->w) + (feet.x-left.x);
                if(!act->is_jumping && !player->flying && !player->climbing && fabs(act->speed.y)<EPSILON)
                    animation = sprite_get_animation(sprite_id, pushing_a_wall ? 14 : 0);
            }
        }

        if(act->position.x <= act->hot_spot.x) {
            player->spin = FALSE;
            at_left_border = TRUE;

            if(act->position.x < act->hot_spot.x) {
                act->speed.x = 0;
                act->position.x = act->hot_spot.x;
                if(brick_down) {
                    pushing_a_wall = TRUE;
                    animation = sprite_get_animation(sprite_id, 1);
                }
            }
        }

        if(act->position.x >= level_size().x - (actor_image(act)->w - act->hot_spot.x)) {
            player->spin = FALSE;
            at_right_border = TRUE;

            if(act->position.x > level_size().x - (actor_image(act)->w - act->hot_spot.x)) {
                act->speed.x = 0;
                act->position.x = level_size().x - (actor_image(act)->w - act->hot_spot.x);
                if(brick_down) {
                    pushing_a_wall = TRUE;
                    animation = sprite_get_animation(sprite_id, 1);
                }
            }
        }

        /*act->position.x = clip(act->position.x, act->hot_spot.x, level_size().x-(actor_image(act)->w - act->hot_spot.x));*/
    }



    /* y-axis */
    if(!player->climbing) {
        if(brick_down) {
            int ang = brick_down->brick_ref->angle;
            int spin_block;
            double factor;
            was_jumping = TRUE;
            act->is_jumping = FALSE;
            act->ignore_horizontal = FALSE;

            /* falling bricks? */
            if(brick_down->brick_ref && brick_down->brick_ref->behavior == BRB_FALL && brick_down->state == BRS_IDLE)
                brick_down->state = BRS_ACTIVE;

            /* stopped, walking, running, spinning... */
            if(fabs(act->speed.x) < EPSILON) {
                if(ang%180==0) player->spin = FALSE;

                /* look down */
                if(input_button_down(act->input, IB_DOWN)) {
                    /* crouch down */
                    if(!player->spin_dash)
                        animation = sprite_get_animation(sprite_id, 4);

                    /* spin dash - start */
                    if(input_button_pressed(act->input, IB_FIRE1)) {
                        animation = sprite_get_animation(sprite_id, 6);
                        player->spin_dash = TRUE;
                        sound_play( sound_load("samples/spindash1.wav") );
                    }
                }
                else if(!pushing_a_wall) {
                    if(input_button_down(act->input, IB_UP)) /* look up */
                        animation = sprite_get_animation(sprite_id, 5);
                    else if(!inside_loop(player)) {
                        /* stopped / ledge */
                        brick_t *minileft, *miniright;
                        v2d_t vminileft  = v2d_add ( feet , v2d_rotate( v2d_new(-8, 0), -act->angle) );
                        v2d_t vminiright = v2d_add ( feet , v2d_rotate( v2d_new(5, 0), -act->angle) );
                        v2d_t v = v2d_new(0,0);
                        actor_corners_ex(act, sqrsize, v, v, v, vminiright, v, vminileft, v, v, brick_list, NULL, NULL, NULL, &miniright, NULL, &minileft, NULL, NULL);
                        if(((!miniright && !(act->mirror&IF_HFLIP)) || (!minileft && (act->mirror&IF_HFLIP))) && !player->on_moveable_platform)
                            animation = sprite_get_animation(sprite_id, 10);
                        else {
                            if( !((input_button_down(act->input, IB_LEFT) && at_left_border) || (input_button_down(act->input, IB_RIGHT) && at_right_border)) )
                                animation = sprite_get_animation(sprite_id, 0);
                            else {
                                act->mirror = at_left_border ? IF_HFLIP : IF_NONE;
                                animation = sprite_get_animation(sprite_id, 1);
                            }
                        }
                    }
                    else /* stopped */
                        animation = sprite_get_animation(sprite_id, 0);
                }
               
                /* spin dash */
                if(player->spin_dash) {

                    /* particles */
                    int a, sd_sig = act->mirror&IF_HFLIP ? 1 : -1, r;
                    v2d_t sd_relativepos, sd_speed;
                    image_t *pixel;

                    for(a=0; a<3; a++) {
                        r = 128+random(128);
                        pixel = image_create(1,1);
                        image_clear(pixel, image_rgb(r,r,r));

                        sd_relativepos = v2d_new(sd_sig*(7+random(7)), 2);
                        sd_speed = v2d_new(sd_sig * (50+random(200)), -random(200));

                        level_create_particle(pixel, v2d_add(act->position,sd_relativepos), sd_speed, TRUE);
                    }

                    /* end */
                    if(input_button_up(act->input, IB_DOWN) || level_editmode()) {
                        player->spin = TRUE;
                        player->spin_dash = FALSE;
                        if( ((act->mirror&IF_HFLIP)&&!brick_left&&!at_left_border) || (!(act->mirror&IF_HFLIP)&&!brick_right&&!at_right_border) )
                            act->speed.x = ( act->mirror & IF_HFLIP ? -1 : 1 )*maxspeed*1.35;
                        sound_play( sound_load("samples/spindash2.wav") );
                    }
                }


            }
            else {
                if(input_button_down(act->input, IB_DOWN)) {
                    if(!player->spin) sound_play( sound_load("samples/spin.wav") );
                    player->spin = TRUE;
                }


                if(!player->spin && !player->braking) {

                    /* animation */
                    if(fabs(act->speed.x) < maxspeed*0.75) {
                        if(!pushing_a_wall && act->speed.y >= 0)
                               animation = sprite_get_animation(sprite_id, 1); /* walking */
                    }
                    else
                        animation = sprite_get_animation(sprite_id, 2); /* running */

                    /* brake */
                    if(fabs(act->speed.x) >= maxspeed*0.35) {
                        if( (input_button_down(act->input, IB_RIGHT)&&(act->speed.x<0)) || (input_button_down(act->input, IB_LEFT)&&(act->speed.x>0)) ) {
                            sound_play( sound_load("samples/brake.wav") );
                            player->braking = TRUE;
                        }
                    }

                }
                else if(player->spin)
                    animation = sprite_get_animation(sprite_id, 3); /* spinning */
                else if(player->braking) {
                    /* particles */
                    int r, sd_sig = act->mirror&IF_HFLIP ? 1 : -1;
                    v2d_t sd_relativepos, sd_speed;
                    image_t *pixel;

                    r = 128+random(128);
                    pixel = image_create(1,1);
                    image_clear(pixel, image_rgb(r,r,r));
                    sd_relativepos = v2d_new(sd_sig*(10-random(21)), 0);
                    sd_speed = v2d_new(sd_sig * (50+random(200)), -random(200));
                    level_create_particle(pixel, v2d_add(act->position,sd_relativepos), sd_speed, TRUE);

                    /* braking */
                    animation = sprite_get_animation(sprite_id, 7);
                    if(fabs(act->speed.x)<10) player->braking = FALSE;
                }
            }

            /* jump */
            /*spin_block = !player->spin_dash && (player->spin ? (fabs(act->speed.x) < maxspeed*1.3) : TRUE);*/
            spin_block = !player->spin_dash;
            if(input_button_down(act->input, IB_FIRE1) && !input_button_down(act->input, IB_DOWN) && !brick_up && !player->landing && spin_block && !act->is_jumping) {
                if(act->speed.y >= 0 && (player->type != PL_KNUCKLES || (player->type == PL_KNUCKLES && !player->flying)))
                    sound_play( sound_load("samples/jump.wav") );
                act->angle = desired_angle;
                act->is_jumping = TRUE;
                player->spin = FALSE;
                animation = sprite_get_animation(sprite_id, 3);
                if(ang == 0) {
                    act->speed.y = -act->jump_strength;
                }
                else if(ang > 0 && ang < 90) {
                    act->speed.x = min(act->speed.x, -0.7*act->jump_strength);
                    act->speed.y = -0.7*act->jump_strength;
                }
                else if(ang == 90) {
                    actor_move(act, v2d_new(20*diff, 0));
                    act->speed.x = min(act->speed.x, -act->jump_strength);
                    act->speed.y = -act->jump_strength/2;
                }
                else if(ang > 90 && ang < 180) {
                    actor_move(act, v2d_new(0, -20*diff));
                    act->speed.x = min(act->speed.x, -0.7*act->jump_strength);
                    act->speed.y = act->jump_strength;
                }
                else if(ang == 180) {
                    actor_move(act, v2d_new(0, -20*diff));
                    act->speed.x *= -1;
                    act->speed.y = act->jump_strength;
                }
                else if(ang > 180 && ang < 270) {
                    actor_move(act, v2d_new(0, -20*diff));
                    act->speed.x = max(act->speed.x, 0.7*act->jump_strength);
                    act->speed.y = act->jump_strength;
                }
                else if(ang == 270) {
                    actor_move(act, v2d_new(-20*diff, 0));
                    act->speed.x = max(act->speed.x, act->jump_strength);
                    act->speed.y = -act->jump_strength/2;
                }
                else if(ang > 270 && ang < 360) {
                    act->speed.x = max(act->speed.x, 0.7*act->jump_strength);
                    act->speed.y = -0.7*act->jump_strength;
                }
            }

            /* slopes / speed issues */
            if(!act->is_jumping) {
                double mytan, super = 1.2, push = 25.0;
                if(ang > 0 && ang < 90) {
                    mytan = min(1, tan( ang*PI/180.0 ));
                    if(fabs(act->speed.y) > EPSILON)
                        act->speed.x = (was_jumping && act->speed.x>0 && ang<45) ? act->speed.x/2 : max(-super*maxspeed, -3*mytan*act->speed.y);
                    else {
                        factor = (!(act->mirror & IF_HFLIP) ? 0.8 : 2.0) * mytan;
                        if(player->braking && ang<45)
                            factor *= 8.0 * (act->speed.x<0 ? -1.0/2.0 : 1.0/0.8);
                        else if(fabs(act->speed.x)<5) {
                            factor *= sin(ang*PI/180.0)*push;
                            player->lock_accel = LOCKACCEL_RIGHT;
                        }
                        act->speed.x = max(act->speed.x - factor*700*dt, -super*maxspeed);
                    }
                }
                else if(ang > 270 && ang < 360) {
                    mytan = min(1, -tan( ang*PI/180.0 ));
                    if(fabs(act->speed.y) > EPSILON)
                        act->speed.x = (was_jumping && act->speed.x<0 && ang>315) ? act->speed.x/2 : min(super*maxspeed, 3*mytan*act->speed.y);
                    else {
                        factor = ((act->mirror & IF_HFLIP) ? 0.8 : 2.0) * mytan;
                        if(player->braking && ang>315)
                            factor *= 8.0 * (act->speed.x>0 ? -1.0/2.0 : 1.0/0.8);
                        else if(fabs(act->speed.x)<5) {
                            factor *= -sin(ang*PI/180.0)*push;
                            player->lock_accel = LOCKACCEL_LEFT;
                        }
                        act->speed.x = min(act->speed.x + factor*700*dt, super*maxspeed);
                    }
                }
            }

            if(ang%90 == 0)
                player->lock_accel = LOCKACCEL_NONE;
        }
        else { /* not brick_down */
            player->braking = FALSE;
            player->lock_accel = LOCKACCEL_NONE;

            if(player->spin_dash) {
                player->spin_dash = FALSE;
                animation = sprite_get_animation(sprite_id, 1);
            }

            if(act->animation == sprite_get_animation(sprite_id, 0) || act->animation == sprite_get_animation(sprite_id, 10) || act->animation == sprite_get_animation(sprite_id, 5))
                animation = sprite_get_animation(sprite_id, 1);

            /*if(!input_button_down(act->input, IB_DOWN) && !inside_loop(player))
                player->spin = FALSE;*/

            if(player->spring || is_walking || act->speed.y < 0)
                player->spin = FALSE;

            if(!inside_loop(player))
                act->angle = desired_angle;
        }

        /* who can fly? */
        if(player->type == PL_TAILS && player->flying)
            gravity_factor = (player->flight_timer < TAILS_MAX_FLIGHT) ? 0.3 : 0.7;
        else
            gravity_factor = 1.0;

        /* y-axis movement */
        ds.y = (fabs(act->speed.y) > EPSILON) ? act->speed.y*dt + 0.5*(gravity*gravity_factor)*(dt*dt) : 0;
        if(!(player->type == PL_KNUCKLES && player->flying))
            act->speed.y = min(act->speed.y + (gravity*gravity_factor)*dt, max_y_speed);




        /* ceil collision */
        angle_question = (brick_up && brick_up->brick_ref->angle%90!=0) && fabs(act->angle)<EPSILON;
        if(brick_up && (brick_up->brick_ref->angle % 90 == 0 || angle_question) && act->speed.y < -EPSILON) {
            act->position.y = (brick_up->y+brick_up->brick_ref->image->h) + (feet.y-up.y);
            act->speed.y = 10;
        }



        /* floor collision */
        brick_tmp = brick_down;
        if(brick_tmp && !act->is_jumping) {
            int ang = brick_tmp->brick_ref->angle;
            act->speed.y = ds.y = 0;
            act->angle = ang * PI / 180.0;

            /* 0 floor */
            if(ang == 0) {
                v2d_t mov = level_brick_move_actor(brick_down, act); /* moveable platforms I */
                feet.y = brick_tmp->y;
                friction = 0;
                if(mov.y > EPSILON) /* if the moveable brick is going down... */
                    ds.y += mov.y*dt;
                else
                    act->position.y = feet.y+diff+1;
            }

            /* (0-90) slope */
            else if(ang > 0 && ang < 90) {
                feet.y = brick_tmp->y + brick_tmp->brick_ref->image->h - (act->position.x-brick_tmp->x)*tan(act->angle);
                if(act->speed.x<0) feet.y += 2.0;
                act->position.y = feet.y+diff;
                if(!(act->mirror & IF_HFLIP)) friction = 0.2;
            }

            /* 90 wall */
            else if(ang == 90) {
                if(fabs(act->speed.x) > 5) {
                    int myang = brick_downright ? brick_downright->brick_ref->angle : -1;
                    if(brick_downright && (myang >= ang && myang < ang+90)) {
                        feet.y = brick_tmp->x;
                        act->position.x = feet.y+diff;
                    }
                    else {
                        act->angle = desired_angle;
                        act->is_jumping = TRUE;
                        if(!player->spin) animation = sprite_get_animation(sprite_id, 1);
                        if(!inside_loop(player)) {
                            actor_move(act, v2d_new(6.5*diff, 0));
                            act->speed = v2d_new(0, -0.9*fabs(act->speed.x));
                        }
                    }
                }
                else {
                    act->angle = desired_angle;
                    actor_move(act, v2d_new(5*diff, 0));
                    act->is_jumping = TRUE;
                    act->ignore_horizontal = FALSE;
                }
                if(!(act->mirror & IF_HFLIP)) friction = 1.5;
            }

            /* (90-180) slope */
            else if(ang > 90 && ang < 180) {
                if(fabs(act->speed.x) > 5) {
                    feet.y = brick_tmp->y - (act->position.x-brick_tmp->x)*tan(act->angle);
                    act->position.y = feet.y-diff;
                }
                else {
                    act->angle = desired_angle;
                    actor_move(act, v2d_new(0, -15*diff));
                    act->is_jumping = TRUE;
                }
                friction = 1.5;
            }

            /* 180 ceil */
            else if(ang == 180) {
                /*if( (act->speed.x > 5 && !(act->mirror & IF_HFLIP)) || (act->speed.x < -5 && act->mirror & IF_HFLIP) ) {*/
                if(fabs(act->speed.x) > 5) {
                    feet.y = brick_tmp->y + brick_tmp->brick_ref->image->h;
                    act->position.y = feet.y-diff;

                    /* end of ceil */
                    if( (act->speed.x > 0 && !brick_downright) || (act->speed.x < 0 && !brick_downleft) ) {
                        actor_move(act, v2d_new(0, 15*diff));
                        act->is_jumping = TRUE;
                        act->speed.x *= -1;
                        act->mirror = act->speed.x<0 ? IF_HFLIP : IF_NONE;
                        act->angle = desired_angle;
                    }
                }
                else {
                    act->angle = desired_angle;
                    actor_move(act, v2d_new(0, -20*diff));
                    act->is_jumping = TRUE;
                    act->speed.x = 0;
                }
                friction = 1.2;
            }

            /* (180-270) slope */
            else if(ang > 180 && ang < 270) {
                if(fabs(act->speed.x) > 5) {
                    feet.y = brick_tmp->y + brick_tmp->brick_ref->image->h - (act->position.x-brick_tmp->x)*tan(act->angle);
                    act->position.y = feet.y-diff;
                }
                else {
                    act->angle = desired_angle;
                    actor_move(act, v2d_new(0, -15*diff));
                    act->is_jumping = TRUE;
                }
                friction = 1.5;
            }

            /* 270 wall */
            else if(ang == 270) {
                if(fabs(act->speed.x) > 5) {
                    int myang = brick_downleft ? brick_downleft->brick_ref->angle : -1;
                    if(brick_downleft && (myang > ang-90 && myang <= ang)) {
                        feet.y = brick_tmp->x + brick_tmp->brick_ref->image->w;
                        act->position.x = feet.y-diff;
                    }
                    else {
                        act->angle = desired_angle;
                        act->is_jumping = TRUE;
                        if(!player->spin) animation = sprite_get_animation(sprite_id, 1);
                        if(!inside_loop(player)) {
                            actor_move(act, v2d_new(-6.5*diff, 0));
                            act->speed = v2d_new(0, -0.9*fabs(act->speed.x));
                        }
                    }

                }
                else {
                    act->angle = desired_angle;
                    actor_move(act, v2d_new(-5*diff, 0));
                    act->is_jumping = TRUE;
                    act->ignore_horizontal = FALSE;
                }
                if(act->mirror & IF_HFLIP) friction = 1.5;
            }

            /* (270-360) slope */
            else if(ang > 270 && ang < 360) {
                feet.y = brick_tmp->y - (act->position.x-brick_tmp->x)*tan(act->angle);
                if(act->speed.x>0) feet.y += 2.0;
                act->position.y = feet.y+diff;
                if(act->mirror & IF_HFLIP) friction = 0.2;
            }
        }


        /* x-axis */
        ds.x = (fabs(act->speed.x) > EPSILON) ? act->speed.x*dt + 0.5*((1.0-friction)*act->acceleration)*(dt*dt) : 0;
        if(input_button_down(act->input, IB_LEFT) && !input_button_down(act->input, IB_RIGHT) && !player->spin && !player->braking && !player->landing && !player->getting_hit && player->lock_accel != LOCKACCEL_LEFT && !at_left_border) {
            if(!act->ignore_horizontal && (act->is_jumping || player->spring || is_walking || !input_button_down(act->input, IB_DOWN))) {
                act->mirror = IF_HFLIP;
                friction = (act->speed.x > 0) ? -1.0 : friction;
                if(act->speed.x >= -maxspeed*1.1)
                    act->speed.x = max(act->speed.x - (1.0-friction)*act->acceleration*dt, -maxspeed);
            }
        }
        else if(input_button_down(act->input, IB_RIGHT) && !input_button_down(act->input, IB_LEFT) && !player->spin && !player->braking && !player->landing && !player->getting_hit && player->lock_accel != LOCKACCEL_RIGHT && !at_right_border) {
            if(!act->ignore_horizontal && (act->is_jumping || player->spring || is_walking || !input_button_down(act->input, IB_DOWN))) {
                act->mirror = IF_NONE;
                friction = (act->speed.x < 0) ? -1.0 : friction;
                if(act->speed.x <= maxspeed*1.1)
                    act->speed.x = min(act->speed.x + (1.0-friction)*act->acceleration*dt, maxspeed);
            }
        }
        else if(brick_down) {
            int signal = 0;
            int ang = brick_down->brick_ref->angle;
            double factor;
            
            /* deceleration factor */
            if(player->spin)
                factor = 0.65;
            else if(player->braking)
                factor = 4.5;
            else if(player->landing)
                factor = 0.6;
            else
                factor = 1.0;

            /* deceleration */
            if(ang % 90 == 0) {
                if( (ang==90 && (act->mirror&IF_HFLIP) && act->speed.x < 0) || ((ang==270) && !(act->mirror&IF_HFLIP) && act->speed.x > 0) ) {
                    if(act->speed.x > EPSILON) signal = 1;
                    else if(-act->speed.x > EPSILON) signal = -1;
                    else signal = 0;
                }
                else {
                    if(act->speed.x > EPSILON) signal = -1;
                    else if(-act->speed.x > EPSILON) signal = 1;
                    else signal = 0;
                }
            }
            else if((ang > 90 && ang < 180) || (ang > 180 && ang < 270)){
                if(act->speed.x > EPSILON) signal = -1;
                else if(-act->speed.x > EPSILON) signal = 1;
                else signal = 0;
            }

            act->speed.x += signal*factor*act->acceleration*dt;
        }
    }


    /* spring mode */
    if(player->spring) {
        animation = sprite_get_animation(sprite_id, act->speed.y <= 0 ? 13 : 1);
        if(act->speed.y > 0) {
            player->spring = FALSE;
            act->is_jumping = FALSE;
        }
    }


    /* got hurt? */
    if(player->getting_hit) {
        if(!brick_down)
            animation = sprite_get_animation(sprite_id, 11);
        else
            player->getting_hit = FALSE;
    }



    /* character's specific routines */
    switch(player->type) {
        case PL_SONIC:
            break;

        case PL_TAILS:
            /* tails can fly */
            player->flight_timer += dt;
            if(brick_down){ player->flying = FALSE; player->flight_timer = 0; }
            if(((act->is_jumping && act->speed.y>-act->jump_strength/3 && !player->getting_hit) || player->flying) && input_button_pressed(act->input, IB_FIRE1) && !player->getting_hit) {
                if(player->flight_timer < TAILS_MAX_FLIGHT) {
                    if(!player->flying) player->flight_timer = 0;
                    act->speed.y = -level_gravity()*0.15;
                    player->flying = TRUE;
                    act->is_jumping = FALSE;
                    animation = sprite_get_animation(sprite_id, 20);
                }
            }
            if(player->flying) {
                if(player->flight_timer >= TAILS_MAX_FLIGHT) { 
                    /* i'm tired of flying... */
                    sound_t *smp = sound_load("samples/tfly2.wav");
                    if(!sound_is_playing(smp)) sound_play(smp);
                    animation = sprite_get_animation(sprite_id, 19);
                }
                else {
                    /* i'm flying! :) */
                    sound_t *smp = sound_load("samples/tfly1.wav");
                    if(!sound_is_playing(smp)) sound_play(smp);
                }
            }
            break;

        case PL_KNUCKLES:
            /* knuckles can fly too! */
            if(((act->is_jumping && act->speed.y>-0.7*act->jump_strength) || player->flying) && input_button_pressed(act->input, IB_FIRE1) && !brick_down && !player->getting_hit) {
                act->speed.y = 50;
                player->flying = TRUE;
                act->is_jumping = FALSE;
                act->speed.x = (act->mirror & IF_HFLIP) ? min(-100, act->speed.x) : max(100, act->speed.x);
            }


            /* fly? */
            if(player->flying) {
                int turning = (input_button_down(act->input, IB_LEFT) && act->speed.x > 0) || (input_button_down(act->input, IB_RIGHT) && act->speed.x < 0);
                int floor = (brick_down && fabs(brick_down->brick_ref->angle*PI/180.0 - desired_angle) < EPSILON);
                turning += (act->animation == sprite_get_animation(sprite_id, 21)) && !actor_animation_finished(act);

                /* i'm flying... */
                if(!floor && act->animation != sprite_get_animation(sprite_id, 19) && !player->landing) {
                    if(!(act->mirror & IF_HFLIP)) {
                        animation = sprite_get_animation(sprite_id, turning ? 21 : 20);
                        act->speed.x = min(act->speed.x + (0.5*act->acceleration)*dt, maxspeed/2);
                    }
                    else {
                        animation = sprite_get_animation(sprite_id, turning ? 21 : 20);
                        act->speed.x = max(act->speed.x - (0.5*act->acceleration)*dt, -maxspeed/2);
                    }
                }

                /* end of flight */
                if(floor) {
                    /* collided with the floor */
                    player->landing = TRUE;
                    act->is_jumping = FALSE;
                    animation = sprite_get_animation(sprite_id, 19);
                    act->speed.y = 0; ds.y = 0;
                    player->climbing = FALSE;
                }
                else if(input_button_up(act->input, IB_FIRE1)) {
                    /* knuckles doesn't want to fly anymore */
                    player->flying = FALSE;
                    animation = sprite_get_animation(sprite_id, 18);
                }
                else {
                    int t;
                    brick_t *try_me[5] = { brick_left , brick_downleft , brick_right , brick_downright , brick_down };
                    for(t=0; t<5; t++) {
                        brick_tmp = try_me[t];
                        if(brick_tmp && brick_tmp->brick_ref->angle%90!=0) {
                            /* collided with a slope while flying? */
                            player->flying = FALSE;
                            player->landing = FALSE;
                        }
                    }
                }

                /* wall climbing - begin */
                if(!floor) {
                    if((brick_left && brick_left->brick_ref->angle%90==0) || (brick_right && brick_right->brick_ref->angle%90==0)) {
                        player->climbing = TRUE;                        
                        player->flying = FALSE;
                        sound_play( sound_load("samples/touch2.wav") );
                    }
                }
            }


            /* no more landing */
            if(player->landing) {
                if(fabs(act->speed.x) < EPSILON || !brick_down)
                    player->flying = player->landing = FALSE;
            }


            /* wall climbing */
            if(player->climbing) {
                act->speed.x = ds.x = 0;
                if(brick_left && !brick_right) act->mirror |= IF_HFLIP;
                if(brick_right && !brick_left) act->mirror &= ~IF_HFLIP;

                /* climbing... */
                if(brick_left || brick_right) {

                    /* knuckles doesn't want to climb the wall anymore */
                    if(input_button_pressed(act->input, IB_FIRE1)) {
                        animation_t *an_a = sprite_get_animation(sprite_id, 17);
                        animation_t *an_b = sprite_get_animation(sprite_id, 22);
                        if(act->animation == an_a || act->animation == an_b) { /* no wall kicking */
                            player->climbing = FALSE; 
                            act->is_jumping = TRUE;
                            act->speed.x = ((act->mirror&IF_HFLIP)?1:-1)*0.7*act->jump_strength;
                            act->speed.y = -0.5*act->jump_strength;
                            animation = sprite_get_animation(sprite_id, 3);
                            sound_play( sound_load("samples/jump.wav") );
                        }
                    }
                    else {
                        /* up or down? */
                        if(input_button_down(act->input, IB_UP)) {
                            if(!brick_up) {
                                ds.y = (-maxspeed*0.1) * dt;
                                animation = sprite_get_animation(sprite_id, 17);
                            }
                        }
                        else if(input_button_down(act->input, IB_DOWN)) {
                            if(!brick_down) {
                                ds.y = (maxspeed*0.1) * dt;
                                animation = sprite_get_animation(sprite_id, 17);
                            }
                            else
                                player->climbing = FALSE; /* reached the ground */
                        }
                        else
                            animation = sprite_get_animation(sprite_id, 22);
                    }
                }

                /* end of wall climbing */
                else {
                    brick_tmp = (act->mirror&IF_HFLIP) ? brick_downleft : brick_downright;
                    if(brick_tmp) {
                        animation = sprite_get_animation(sprite_id, 23);
                        act->ignore_horizontal = TRUE;
                        if(actor_animation_finished(act)) {
                            player->climbing = FALSE;
                            act->ignore_horizontal = FALSE;
                            act->speed = v2d_new(((act->mirror&IF_HFLIP)?-1:1)*maxspeed*0.15, -level_gravity()/12.5);
                            ds.x = ((act->mirror&IF_HFLIP)?-1:1)*5;
                        }
                    }
                    else {
                        player->climbing = FALSE;
                        act->is_jumping = TRUE;
                        animation = sprite_get_animation(sprite_id, 3);
                    }
                }


            }
            break;
    }


    /* almost done... */
    if(animation) actor_change_animation(act, animation);
    if(fabs(act->speed.x) < 4) {
        player->braking = FALSE;
        if( (!input_button_down(act->input, IB_RIGHT) && !input_button_down(act->input, IB_LEFT)) || (input_button_down(act->input, IB_RIGHT) && input_button_down(act->input, IB_LEFT)) || player->spin || player->landing ) {
            ds.x = 0;
            act->speed.x = 0;
        }
    }
    ds.x += level_brick_move_actor(brick_down,act).x*dt; /* moveable platforms II */
    ds.x = ((act->position.x<=act->hot_spot.x) && act->speed.x<0) ? 0 : ds.x;
    ds.x = ((act->position.x>=level_size().x - (actor_image(act)->w-act->hot_spot.x)) && act->speed.x>0) ? 0 : ds.x;
    return ds;
}


/*
 * player_bounce()
 * Bounces
 */
void player_bounce(player_t *player)
{
    input_simulate_button_down(player->actor->input, IB_FIRE1);
    player->spring = FALSE;
    player->actor->speed.y = -player->actor->jump_strength;
    player->actor->is_jumping = TRUE;
    player->flying = FALSE;
}


/*
 * player_get_rings()
 * Returns the amount of rings
 * the player has got so far
 */
int player_get_rings()
{
    return rings;
}



/*
 * player_set_rings()
 * Sets a new amount of rings
 */
void player_set_rings(int r)
{
    rings = r;

    /* 100+ * k rings (k integer) = new life! */
    if(r/100 > hundred_rings) {
        hundred_rings = r/100;
        player_set_lives( player_get_lives()+1 );
        level_override_music( sound_load("samples/1up.wav") );
    }
}



/*
 * player_get_lives()
 * How many lives does the player have?
 */
int player_get_lives()
{
    return lives;
}



/*
 * player_set_lives()
 * Sets the number of lives
 */
void player_set_lives(int l)
{
    lives = max(0, l);
}



/*
 * player_get_score()
 * Returns the score
 */
int player_get_score()
{
    return score;
}



/*
 * player_set_score()
 * Sets the score
 */
void player_set_score(int s)
{
    score = max(0, s);
}





/*
 * player_hit()
 * Hits a player. If it has no rings, then
 * it must die
 */
void player_hit(player_t *player)
{
    actor_t *act = player->actor;
    item_t *ring;
    int i;
    int get_hit = FALSE;

    if(!player->blinking && !player->dieing && !player->invencible) {
        drop_glasses(player);
        if(player->shield_type != SH_NONE) {
            /* lose shield */
            get_hit = TRUE;
            player->shield_type = SH_NONE;
            sound_play( sound_load("samples/death.wav") );
        }
        else if(rings > 0) {
            /* lose rings */
            get_hit = TRUE;
            for(i=0; i<min(player_get_rings(), 30); i++) {
                ring = level_create_item(IT_RING, act->position);
                ring->state = IS_ACTIVE;
                ring->actor->speed.x = ring->actor->maxspeed * (random(100)-50)/100;
                ring->actor->speed.y = -ring->actor->jump_strength+random(ring->actor->jump_strength);
            }
            player_set_rings(0);
            sound_play( sound_load("samples/ringless.wav") );
        }
        else {
            /* death */
            player_kill(player);
        }
    }

    if(get_hit) {
        player->getting_hit = TRUE;
        player->flying = player->landing = player->climbing = player->spring = FALSE;
        player->spin_dash = player->spin = FALSE;
        player->blinking = TRUE;
        player->blink_timer = 0;
        act->speed.x = act->mirror&IF_HFLIP ? 200 : -200;
        act->speed.y = -act->jump_strength*0.75;
        actor_move(act, v2d_new(0, -5));
    }
}



/*
 * player_kill()
 * Kills a player
 */
void player_kill(player_t *player)
{
    if(!player->dieing) {
        drop_glasses(player);
        player->shield_type = SH_NONE;
        player->invencible = FALSE;
        player->got_speedshoes = FALSE;
        player->dieing = TRUE;
        player->death_timer = 0;
        player->spring = FALSE;
        player->actor->speed.y = -player->actor->jump_strength*1.2;
        player->flying = player->climbing = player->landing = FALSE;
        player->spin = player->spin_dash = FALSE;
        player->blinking = FALSE;
        sound_play( sound_load("samples/death.wav") );
    }
}



/*
 * player_attacking()
 * Returns TRUE if a given player is attacking;
 * FALSE otherwise
 */
int player_attacking(player_t *player)
{
    animation_t *jump = sprite_get_animation(get_sprite_id(player->type), 3);
    return player->spin || player->spin_dash ||
           (/*player->actor->is_jumping &&*/ player->actor->animation == jump) ||
           (player->type == PL_KNUCKLES && (player->landing || player->flying));
}





/* private functions */
char *get_sprite_id(int player_type)
{
    static char id[32];
    
    switch(player_type) {
        case PL_SONIC:
            strcpy(id, "SD_SONIC");
            break;

        case PL_TAILS:
            strcpy(id, "SD_TAILS");
            break;

        case PL_KNUCKLES:
            strcpy(id, "SD_KNUCKLES");
            break;
    }

    return id;
}


void update_glasses(player_t *p)
{
    int frame_id = 0, hflip = p->actor->mirror & IF_HFLIP;
    int visible = TRUE;
    double ang = old_school_angle(p->actor->angle);
    v2d_t gpos, top = v2d_subtract(p->actor->position,v2d_rotate(v2d_new(0,p->actor->hot_spot.y),-ang));
    animation_t *anim = p->actor->animation;



    switch(p->type) {


        case PL_SONIC:
            if(anim == sprite_get_animation("SD_SONIC", 0)) {
                /* stopped */
                gpos = v2d_new(3,24);
                frame_id = 1;
            }
            else if(anim == sprite_get_animation("SD_SONIC", 1)) {
                /* walking */
                switch((int)p->actor->animation_frame) {
                    case 0: frame_id = 2; gpos = v2d_new(5,23); break;
                    case 1: frame_id = 2; gpos = v2d_new(4,25); break;
                    case 2: frame_id = 1; gpos = v2d_new(7,25); break;
                    case 3: frame_id = 1; gpos = v2d_new(5,23); break;
                    case 4: frame_id = 1; gpos = v2d_new(5,23); break;
                    case 5: frame_id = 1; gpos = v2d_new(4,24); break;
                    case 6: frame_id = 2; gpos = v2d_new(6,24); break;
                    case 7: frame_id = 2; gpos = v2d_new(6,23); break;
                }
            }
            else if(anim == sprite_get_animation("SD_SONIC", 2)) {
                /* running */
                frame_id = 1;
                gpos = v2d_new(8,26);
            }
            else if(anim == sprite_get_animation("SD_SONIC", 5)) {
                /* look up */
                frame_id = 3;
                if((int)p->actor->animation_frame == 0)
                    gpos = v2d_new(0,19);
                else
                    gpos = v2d_new(-1,21);
            }
            else if(anim == sprite_get_animation("SD_SONIC", 7)) {
                /* braking */
                frame_id = 1;
                if((int)p->actor->animation_frame < 2)
                    gpos = v2d_new(8,26);
                else
                    gpos = v2d_new(10,28);
            }
            else if(anim == sprite_get_animation("SD_SONIC", 10)) {
                /* almost falling / ledge */
                frame_id = 1;
                switch((int)p->actor->animation_frame) {
                    case 0: gpos = v2d_new(1,22); break;
                    case 1: gpos = v2d_new(-1,23); break;
                    case 2: gpos = v2d_new(1,23); break;
                }
            }
            else if(anim == sprite_get_animation("SD_SONIC", 11)) {
                /* ringless */
                frame_id = 3;
                gpos = v2d_new(-4,30);
            }
            else if(anim == sprite_get_animation("SD_SONIC", 12)) {
                /* breathing */
                frame_id = 3;
                gpos = v2d_new(1,19);
            }
            else if(anim == sprite_get_animation("SD_SONIC", 13)) {
                /* spring */
                frame_id = 3;
                gpos = v2d_new(4,13);
            }
            else if(anim == sprite_get_animation("SD_SONIC", 14)) {
                /* pushing */
                frame_id = 1;
                gpos = v2d_new(12,31);
            }
            else if(anim == sprite_get_animation("SD_SONIC", 15)) {
                /* waiting */
                frame_id = 0;
                gpos = v2d_new(3,23);
            }
            else
                visible = FALSE;
            break;



        case PL_TAILS:
            if(anim == sprite_get_animation("SD_TAILS", 0)) {
                /* stopped */
                gpos = v2d_new(5,34);
                frame_id = 1;
            }
            else if(anim == sprite_get_animation("SD_TAILS", 1)) {
                /* walking */
                frame_id = 2;
                switch((int)p->actor->animation_frame) {
                    case 0: gpos = v2d_new(2,33); break;
                    case 1: gpos = v2d_new(3,33); break;
                    case 2: gpos = v2d_new(8,33); break;
                    case 3: gpos = v2d_new(3,32); break;
                    case 4: gpos = v2d_new(1,33); break;
                    case 5: gpos = v2d_new(3,33); break;
                    case 6: gpos = v2d_new(7,33); break;
                    case 7: gpos = v2d_new(3,32); break;
                }
            }
            else if(anim == sprite_get_animation("SD_TAILS", 2)) {
                /* running */
                frame_id = 2;
                if((int)p->actor->animation_frame == 0)
                    gpos = v2d_new(7,35);
                else
                    gpos = v2d_new(6,34);
            }
            else if(anim == sprite_get_animation("SD_TAILS", 4)) {
                /* crouch down */
                frame_id = 1;
                gpos = v2d_new(9,44);
            }
            else if(anim == sprite_get_animation("SD_TAILS", 5)) {
                /* look up */
                frame_id = 1;
                gpos = v2d_new(7,32);
            }
            else if(anim == sprite_get_animation("SD_TAILS", 7)) {
                /* braking */
                frame_id = 1;
                if((int)p->actor->animation_frame == 0)
                    gpos = v2d_new(2,33);
                else
                    gpos = v2d_new(4,33);
            }
            else if(anim == sprite_get_animation("SD_TAILS", 10)) {
                /* almost falling / ledge */
                frame_id = 4;
                switch((int)p->actor->animation_frame) {
                    case 0: gpos = v2d_new(5,33); break;
                    case 1: gpos = v2d_new(6,33); break;
                }
            }
            else if(anim == sprite_get_animation("SD_TAILS", 11)) {
                /* ringless */
                frame_id = 1;
                gpos = v2d_new(1,33);
            }
            else if(anim == sprite_get_animation("SD_TAILS", 12)) {
                /* breathing */
                frame_id = 1;
                gpos = v2d_new(6,28);
            }
            else if(anim == sprite_get_animation("SD_TAILS", 13)) {
                /* spring */
                frame_id = 3;
                gpos = v2d_new(2,17);
            }
            else if(anim == sprite_get_animation("SD_TAILS", 14)) {
                /* pushing */
                frame_id = 1;
                gpos = v2d_new(9,35);
            }
            else if(anim == sprite_get_animation("SD_TAILS", 15)) {
                /* waiting */
                frame_id = 4;
                switch((int)p->actor->animation_frame) {
                    case 0: case 8: case 9: case 10: gpos = v2d_new(5,34); break;
                    default: gpos = v2d_new(5,33); break;
                }
            }
            else if(anim == sprite_get_animation("SD_TAILS", 19)) {
                /* tired of flying */
                frame_id = 1;
                if((int)p->actor->animation_frame == 0)
                    gpos = v2d_new(9,39);
                else
                    gpos = v2d_new(9,40);
            }
            else if(anim == sprite_get_animation("SD_TAILS", 20)) {
                /* flying */
                frame_id = 1;
                gpos = v2d_new(8,39);
            }
            else
                visible = FALSE;
            break;


        case PL_KNUCKLES:
            if(anim == sprite_get_animation("SD_KNUCKLES", 0)) {
                /* stopped */
                frame_id = 1;
                gpos = v2d_new(1,24);
            }
            else if(anim == sprite_get_animation("SD_KNUCKLES", 1)) {
                /* walking */
                switch((int)p->actor->animation_frame) {
                    case 0: frame_id = 1; gpos = v2d_new(5,29); break;
                    case 1: frame_id = 2; gpos = v2d_new(5,29); break;
                    case 2: frame_id = 2; gpos = v2d_new(8,29); break;
                    case 3: frame_id = 2; gpos = v2d_new(9,28); break;
                    case 4: frame_id = 1; gpos = v2d_new(6,28); break;
                    case 5: frame_id = 1; gpos = v2d_new(6,29); break;
                    case 6: frame_id = 1; gpos = v2d_new(5,28); break;
                    case 7: frame_id = 1; gpos = v2d_new(4,27); break;
                }
            }
            else if(anim == sprite_get_animation("SD_KNUCKLES", 2)) {
                /* running */
                frame_id = 1;
                gpos = v2d_new(7,29);
            }
            else if(anim == sprite_get_animation("SD_KNUCKLES", 4)) {
                /* crouch down */
                frame_id = 1;
                if((int)p->actor->animation_frame == 0)
                    gpos = v2d_new(0,31);
                else
                    gpos = v2d_new(0,40);
            }
            else if(anim == sprite_get_animation("SD_KNUCKLES", 5)) {
                /* look up */
                frame_id = 1;
                if((int)p->actor->animation_frame == 0)
                    gpos = v2d_new(0,21);
                else
                    gpos = v2d_new(-1,21);
            }
            else if(anim == sprite_get_animation("SD_KNUCKLES", 7)) {
                /* braking */
                frame_id = 0;
                gpos = v2d_new(-2,27);
            }
            else if(anim == sprite_get_animation("SD_KNUCKLES", 10)) {
                /* almost falling / ledge */
                frame_id = 1;
                switch((int)p->actor->animation_frame) {
                    case 0: gpos = v2d_new(9,30); break;
                    case 1: gpos = v2d_new(8,27); break;
                }
            }
            else if(anim == sprite_get_animation("SD_KNUCKLES", 11)) {
                /* ringless */
                frame_id = 1;
                gpos = v2d_new(-3,27);
            }
            else if(anim == sprite_get_animation("SD_KNUCKLES", 12)) {
                /* breathing */
                frame_id = 1;
                gpos = v2d_new(5,24);
            }
            else if(anim == sprite_get_animation("SD_KNUCKLES", 13)) {
                /* spring */
                frame_id = 3;
                gpos = v2d_new(-1,16);
            }
            else if(anim == sprite_get_animation("SD_KNUCKLES", 14)) {
                /* pushing */
                switch((int)p->actor->animation_frame) {
                    case 0: frame_id = 1; gpos = v2d_new(5,29); break;
                    case 1: frame_id = 2; gpos = v2d_new(5,29); break;
                    case 2: frame_id = 2; gpos = v2d_new(8,29); break;
                    case 3: frame_id = 2; gpos = v2d_new(9,28); break;
                    case 4: frame_id = 1; gpos = v2d_new(6,28); break;
                    case 5: frame_id = 1; gpos = v2d_new(6,29); break;
                    case 6: frame_id = 1; gpos = v2d_new(5,28); break;
                    case 7: frame_id = 1; gpos = v2d_new(4,27); break;
                }
            }
            else if(anim == sprite_get_animation("SD_KNUCKLES", 15)) {
                /* waiting */
                frame_id = 0;
                gpos = v2d_new(1,23);
            }
            else if(anim == sprite_get_animation("SD_KNUCKLES", 16)) {
                /* no more climbing */
                frame_id = 1;
                switch((int)p->actor->animation_frame) {
                    case 0: gpos = v2d_new(6,23); break;
                    case 1: gpos = v2d_new(5,20); break;
                    case 2: gpos = v2d_new(0,22); break;
                }
            }
            else if(anim == sprite_get_animation("SD_KNUCKLES", 17)) {
                /* climbing */
                frame_id = 3;
                switch((int)p->actor->animation_frame) {
                    case 0: gpos = v2d_new(-1,22); break;
                    case 1: gpos = v2d_new(-2,20); break;
                    case 2: gpos = v2d_new(0,21); break;
                    case 3: gpos = v2d_new(-1,24); break;
                    case 4: gpos = v2d_new(0,23); break;
                    case 5: gpos = v2d_new(0,22); break;
                }
            }
            else if(anim == sprite_get_animation("SD_KNUCKLES", 18)) {
                /* end of flight */
                frame_id = 1;
                if((int)p->actor->animation_frame == 0)
                    gpos = v2d_new(6,23);
                else
                    gpos = v2d_new(5,20);
            }
            else if(anim == sprite_get_animation("SD_KNUCKLES", 19)) {
                /* flying - ground */
                frame_id = 1;
                gpos = v2d_new(8,44);
            }
            else if(anim == sprite_get_animation("SD_KNUCKLES", 20)) {
                /* flying - air */
                frame_id = 1;
                gpos = v2d_new(8,39);
            }
            else if(anim == sprite_get_animation("SD_KNUCKLES", 21)) {
                /* flying - turn */
                frame_id = 4;
                switch((int)p->actor->animation_frame) {
                    case 0: gpos = v2d_new(-8,41); break;
                    case 1: gpos = v2d_new(0,43); break;
                    case 2: gpos = v2d_new(10,41); break;
                }
            }
            else if(anim == sprite_get_animation("SD_KNUCKLES", 22)) {
                /* climbing - stopped */
                frame_id = 3;
                gpos = v2d_new(0,22);
            }
            else if(anim == sprite_get_animation("SD_KNUCKLES", 23)) {
                /* climbing - reached the top */
                switch((int)p->actor->animation_frame) {
                    case 0: frame_id = 3; gpos = v2d_new(7,17); break;
                    case 1: frame_id = 3; gpos = v2d_new(11,15); break;
                    case 2: frame_id = 0; gpos = v2d_new(12,13); break;
                }
            }
            else
                visible = FALSE;
            break;
    }

    gpos.x *= hflip ? -1 : 1;
    actor_change_animation(p->glasses, sprite_get_animation("SD_GLASSES", frame_id));
    p->glasses->position = v2d_add(top, v2d_rotate(gpos, -ang));
    p->glasses->angle = ang;
    p->glasses->mirror = p->actor->mirror;
    p->glasses->visible = visible && p->actor->visible;
}



void drop_glasses(player_t *p)
{
    if(p->got_glasses) {
        v2d_t pos = v2d_add(p->actor->position, v2d_new(0,-27));
        item_t *item = level_create_item(IT_FALGLASSES, pos);
        item->actor->speed.x = -p->actor->speed.x*0.2;
        item->actor->speed.y = -490;
        p->got_glasses = FALSE;
    }
}



void update_shield(player_t *p)
{
    actor_t *sh = p->shield, *act = p->actor;
    v2d_t off = v2d_new(0,0);

    switch(p->shield_type) {

        /* regular shield */
        case SH_SHIELD:
            off = v2d_new(0,-22);
            sh->position = v2d_add(act->position, v2d_rotate(off, -old_school_angle(act->angle)));
            actor_change_animation(sh, sprite_get_animation("SD_SHIELD", 0));
            break;

    }
}



/* useful on sonic loops */
void forget_bricks(int top, int right, int bottom, int left, brick_t **brick_up, brick_t **brick_upright, brick_t **brick_right, brick_t **brick_downright, brick_t **brick_down, brick_t **brick_downleft, brick_t **brick_left, brick_t **brick_upleft)
{
    int i, ang;
    brick_t **v[8] = { brick_up, brick_upright, brick_right, brick_downright, brick_down, brick_downleft, brick_left, brick_upleft };

    for(i=0; i<8; i++) {
        if(NULL == *(v[i])) continue;
        ang = (*(v[i]))->brick_ref->angle;

        if(ang == 0)
            *(v[i]) = bottom ? NULL : *(v[i]);
        else if(ang > 0 && ang < 180)
            *(v[i]) = right ? NULL : *(v[i]);
        else if(ang == 180)
            *(v[i]) = top ? NULL : *(v[i]);
        else if(ang > 180 && ang < 360)
            *(v[i]) = left ? NULL : *(v[i]);
    }
}


/* is the player inside a loop? */
int inside_loop(player_t *p)
{
    return (p->disable_wall != PLAYER_WALL_NONE);
}


/* old-school angle */
double old_school_angle(double ang)
{
    return PI/4 * (ang <= PI ? floor(ang/(PI/4) + 0.1) : ceil(ang/(PI/4)));
}
