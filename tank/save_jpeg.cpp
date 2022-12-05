#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro.h>
#include "scorch.h"
#include "TonyJpegEncoder.h"

// defined in dialogs.h but it includes to much junk!!
extern	int 	screenshotJPG;


//	should not be global
unsigned char InBuf[800*600*3];		// source data, bgr format, 3 bytes per pixel
unsigned char OutBuf[800*600*3];	// destination buffer, in jpg format


void save_jpeg(char *filename, BITMAP * shotbuf)
{
char msg[64];
BITMAP *bmp = shotbuf;
int						nOutputBytes;				// return number of bytes being written
unsigned char *pInBuf;						// pointer to source data
unsigned char pOutBuf;						// pointer to destination buffer
int						nWidthPix=800;			// image width in pixels
int						nHeight=600;				// height
int		x,y;
int		color;

//	Set the qality
switch(screenshotJPG)
	{
	case 1:	x=50; break; // low quality
	case 2:	x=64; break; // medium
	case 3:	x=91; break; // high
	default: return; // do nothing
	}
#ifdef	_DEBUG
Printf("JPEG quality: %d\n", x);
#endif

pInBuf=InBuf;

class CTonyJpegEncoder jp( x );

FILE *f = fopen (filename, "wb");
if (!f)
	{
	sprintf(msg, "Couldn't create JPEG file '%s'", filename);
	G_Printf(msg);
	return;
	}

StartSound(MOUSE_SELECT);

for (y=nHeight-1; y>=0; y--)
	{
	for (x=0; x<nWidthPix; x++)
		{			// Get the color
		color = getpixel(bmp, x,y);
		*pInBuf++ = (unsigned char)getb16(color);// blue
		*pInBuf++ = (unsigned char)getg16(color);// green
		*pInBuf++ = (unsigned char)getr16(color);// red
		}
	}

nOutputBytes = jp.CompressImage(	//return number of bytes being written
		InBuf,						//source data, bgr format, 3 bytes per pixel
		OutBuf,						//destination buffer, in jpg format
		nWidthPix,				//image width in pixels
		nHeight						//height
		);					

#ifdef	_DEBUG
Printf("CompressImage '%d'\n", nOutputBytes);
#endif

if(nOutputBytes)		// was anything produced
	fwrite(OutBuf, nOutputBytes, 1, f);
else
	Printf("JPEG Compression went BOOM!! Empty File Produced.");

fclose (f);

StartSound(MOUSE_SELECT);

#ifdef	_DEBUG
Printf("Finished JPEG\n");
#endif
}
