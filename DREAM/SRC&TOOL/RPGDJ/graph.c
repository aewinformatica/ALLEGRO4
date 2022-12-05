/******************************************/
/*                graph.c                 */
/*                                        */
/*          ˮ��֮Լ�е�ͼ�����          */
/*                                        */
/*             ���뻷��DJGPP              */
/*                                        */
/*        ������allegro V4.1.13֧��       */
/*                                        */
/*           Cat Tom 12.5.2003            */
/******************************************/


#define BLACK     makecol(0,0,0)         /*��ɫ*/
#define WHITE     makecol(255,255,255)   /*��ɫ*/
#define RED       makecol(255,0,0)       /*��ɫ*/
#define GREEN     makecol(0,255,0)       /*��ɫ*/
#define BLUE      makecol(0,0,255)       /*��ɫ*/
#define LIGHTGRAY makecol(180,180,180)   /*ǳ��ɫ*/
#define DARKGRAY  makecol(128,128,128)   /*���ɫ*/
#define YELLOW    makecol(255,255,0)     /*��ɫ*/
#define DARKGREEN makecol(0,128,0)       /*����ɫ*/
#define LIGHTBLUE makecol(0,128,255)     /*ǳ��ɫ*/
#define CYAN      makecol(0,255,255)     /*��ɫ*/
#define PINK      makecol(255,128,192)   /*�ۺ�ɫ*/
#define ORANGE    makecol(255,128,0)     /*��ɫ*/
#define MAGENTA   makecol(255,0,255)     /*��ɫ*/
#define BROWN     makecol(128,0,64)      /*��ɫ*/

/******************************************************************/

RGB Pal[256];
BITMAP *V_Buffer;
char HZK16[267616],ASC16[4096];

/******************************************************************/

/*�����ֿ�*/
void LoadCharLib()
{
 FILE *fp;

 fp=fopen("font/HZK16","rb");
 fread(HZK16,267616,1,fp);
 fclose(fp);

 fp=fopen("font/ASC16","rb");
 fread(ASC16,4096,1,fp);
 fclose(fp);
}


void init_graph()
{
 set_color_depth(16);
 set_gfx_mode(GFX_AUTODETECT_WINDOWED , 640, 480, 0, 0);

 /*������ʾ������*/
 V_Buffer=create_bitmap(640,480);

 /*�����ֿ�*/
 LoadCharLib();
}


void vpost()
{
 blit(V_Buffer,screen,0,0,0,0,640,480);
}


void clrscreen(int color)
{
 drawing_mode(DRAW_MODE_SOLID,V_Buffer,640,480);
 rectfill(V_Buffer,0,0,639,479,color);
}


/*����ͼƬ*/
BITMAP *LoadBMP(char *filename)
{
 return load_bitmap(filename,Pal);
}


/*������ʾ��λͼ*/
void QPutImage(int xx,int yy,BITMAP *bmp)
{
  blit(bmp,V_Buffer,-xx,-yy,0,0,640,480);
}


/*��͸��ɫ��ʾ��λͼ*/
void PutImage(int xx,int yy,BITMAP *bmp)
{
 masked_blit(bmp,V_Buffer,-xx,-yy,0,0,640,480);
}


typedef struct
{
 int width;          /*�ӻ����*/
 int height;         /*�ӻ����*/
 BITMAP *image;      /*ͼ��*/
}Sprlib;


/*���ؾ����*/
Sprlib loadsprlib(int width,int height,char *filename)
{
 Sprlib slib;

 /*����bmpͼ��*/
 slib.image=LoadBMP(filename);

 slib.width=width;
 slib.height=height;

 return slib;
}


/*��ʾ�����*/
void putspr(int x,int y,int xx,int yy,Sprlib slib)
{
 masked_blit(slib.image,V_Buffer,xx*slib.width,yy*slib.height,x,y,slib.width,slib.height);
}


/*��ʾ͸�������*/
void alphaputspr(int x,int y,int xx,int yy,Sprlib slib)
{
 BITMAP *temp;

 temp=create_bitmap(slib.width,slib.height);
 blit(slib.image,temp,xx*slib.width,yy*slib.height,0,0,slib.width,slib.height);
 set_trans_blender(0,0,0,127);
 draw_trans_sprite(V_Buffer,temp,x,y);
 destroy_bitmap(temp);
}


typedef struct
{
 int timedelay;       /*֡����*/
 int frame;           /*��֡��*/
 int timetick;        /*��ʱ��*/
 int counter;         /*��ʾ֡��������*/
 Sprlib slib;         /*�����*/
}Animate;


Animate init_animate(int width,int height,int frame,int timedelay,char *filename)
{
 Animate ani;

 ani.timedelay=timedelay;
 ani.timetick=timedelay;
 ani.frame=frame;
 ani.counter=0;

 ani.slib=loadsprlib(width,height,filename);

 return ani;
}


void showanimate(int x,int y,Animate *animate)
{
 animate->timetick--;
 if(animate->timetick<0)
 {
  animate->timetick=animate->timedelay;
  animate->counter++;
  if(animate->counter==animate->frame)animate->counter=0;
 }
 
 putspr(x,y,animate->counter,0,animate->slib);
}


void alphabar(int x,int y,unsigned int width,unsigned int height,int color)
{
 set_trans_blender(0,0,0,127);
 drawing_mode(DRAW_MODE_TRANS,V_Buffer,640,480);
 rectfill(V_Buffer,x,y,x+width,y+height,color);
}


void alphaimage(int x,int y,BITMAP *bmp)
{
 set_trans_blender(0,0,0,127);
 draw_trans_sprite(V_Buffer,bmp,x,y);
}


void rectangle(int x0,int y0,int x1,int y1,int color)
{
 drawing_mode(DRAW_MODE_SOLID,V_Buffer,640,480);
 rect(V_Buffer,x0,y0,x1,y1,color);
}

////////////////////////////////////////////////////////////////////
/******************************************************************/

#define SL_NONE 0    /*���ֲ���б*/
#define SL_LEFT 1    /*��������*/
#define SL_RIGHT -1    /*��������*/

/******************************************************************/

char TEXT_DIRECTION=1;        /*���ֱ�־��1Ϊ���ţ�0Ϊ����*/
char TEXT_SLANT=SL_NONE;    /*������б:����б*/

/******************************************************************/

void puts16b(unsigned char *s,int x,int y,int color)
{char text_flag,fix_length=0,th,tw,i,j,mat[32]={0},canput,text_h=16,slant;
 long position;
 unsigned char qh,wh,*p;

 drawing_mode(DRAW_MODE_SOLID,V_Buffer,640,480);

 while(*s!=0)
 {if(*s>0xa0&&*(s+1)>0xa0)
  {
   tw=16;th=16;
   qh=*s-0xa0;
   wh=*(s+1)-0xa0;
   position=(94*(qh-1)+(wh-1))*2l*th;
   memcpy(mat,HZK16+position,th*2);
   text_flag=1;
  }
  else
  {tw=8;th=text_h;
   memcpy(mat,ASC16+(*s)*text_h,text_h);
   text_flag=0;
  }
  slant=0;
  for(j=0;j<th;j++)
  {if(!(j%4))slant+=TEXT_SLANT;
   for(i=0;i<tw;i++)
   {if(text_flag)canput=((0x80>>i%8)&mat[2*j+i/8]);
    else canput=((mat[j]>>(7-i))&1);
    if(canput)
    {fix_length=(text_flag&&text_h==14);
     putpixel(V_Buffer,i+x+fix_length+slant,y+j,color);
    }
   }
  }
  if(text_flag)
  {s+=2;
   if(TEXT_DIRECTION)x+=text_h+2;
   else y+=text_h+2;
  }
  else
  {s++;
   if(TEXT_DIRECTION)x+=9;
  }
 }
}

void boxputs16b(char *s,int x0,int y0,char texts_width,char texts_height,int color)
{unsigned char workline[81]={0},flag,i,h,count=0;
 if(TEXT_DIRECTION==0)return;

 h=24;

 while(1)
 {if(count+1>texts_height)return;
  memcpy(workline,s,texts_width);
  for(i=0;i<texts_width;i++)
   if(workline[texts_width-i-1]<128)break;
  if(i%2)
  {s+=texts_width-1;
   workline[texts_width-1]=0;
  }
  else s+=texts_width;
  puts16b(workline,x0,y0+count*h,color);
  count++;
  for(i=0;i<texts_width-1;i++)if(workline[i]==0)return;
 }
}

unsigned char boxput_height(unsigned char *s,char texts_width)
{char i,temp=0;
 while(1)
 {if(*s==0)return(temp);
  temp++;
  for(i=0;i<texts_width;i++)if(s[i]==0)return(temp);
  for(i=0;i<texts_width-1;i++)
   if(s[texts_width-i-1]<128)break;
  if(i%2)s+=texts_width-1;
  else s+=texts_width;
 }
}

void shadow_puts(char *str,int xx,int yy,int color)
{
 puts16b(str,xx+1,yy+1,BLACK);
 puts16b(str,xx,yy,color);
}
