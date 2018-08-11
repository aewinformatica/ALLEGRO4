#include "sprite.h"

Sprite::Sprite(){

};

/* get one animation from one datafile. This function is called for every datafile entry loaded */
void Sprite::getAnim(DATAFILE *d) {
     
	  const char *data_prop;
	  
	  struct anim_t animation;  
	 
	  data_prop = get_datafile_property(d, DAT_NAME);
      	  if(data_prop != empty_string)
	    ustrcpy(animation.name, data_prop);
	 
	  // if we've hit an end of file
	  if(ustrcmp("GrabberInfo", animation.name) == 0) 
	    return;
	 
	  //logstr("Loading %s...", animation.name); // dump
	  printf("Loading %s...", animation.name); // dump
      
      //descobrir como pegar a tela sem passar como parametro
      textprintf_ex(screen, font, 0, 70, palette_color[15],-1,"Loading: %s...",animation.name);

	  
	 
	  data_prop = get_datafile_property(d, DAT_ID('X','S','I','Z')); // frame width
	  if(data_prop != empty_string)
	    animation.w = atoi(data_prop);
	 
	  data_prop = get_datafile_property(d, DAT_ID('Y','S','I','Z')); // frame height
	  if(data_prop != empty_string)
	    animation.h = atoi(data_prop);
	 
	  data_prop = get_datafile_property(d, DAT_ID('F','C',' ',' ')); // frame count
	  if(data_prop != empty_string){
	    animation.frame_count = atoi(data_prop);
          textprintf_ex(screen, font, 0, 110, palette_color[15],-1, "frame_count %s", data_prop);
    while (!keypressed()) ;
        }
       

  
	 
	  data_prop = get_datafile_property(d, DAT_ID('T','B','F','A')); // speed or Time Before Frame Advance (in one-sevenths of a second)
	  if(data_prop != empty_string)
	    animation.frame_speed = atoi(data_prop);
	 
	  data_prop = get_datafile_property(d, DAT_ID('X','P','O','S')); // x-origin
	  if(data_prop != empty_string)
	    animation.xorigin = atoi(data_prop);
	 
	  data_prop = get_datafile_property(d, DAT_ID('Y','P','O','S')); // y-origin
	  if(data_prop != empty_string)
	    animation.yorigin = atoi(data_prop);
	 
	 
      for(int x=0;x<=animation.frame_count;x++) {
	    /* create the rle sprite from a sub-bitmap of a image containing
	    all the animations in a film strip */
	    textprintf_ex(screen, font, 0, 210, palette_color[15],-1, "nome %s", animation.name);
	    animation.anim_data.push_back( get_rle_sprite( create_sub_bitmap( (BITMAP*)d->dat, , 0, animation.w, animation.h ) ) ); 
	  }	 
	  // store the pointer to the animation data in the vector
	  animations.push_back(&animation);
	 
	  return;
};

void Sprite::draw(BITMAP *pBuffer){
     
     // draw_rle_sprite(pBuffer, this->animations[0]->anim_data[0], 0, 0);
};
