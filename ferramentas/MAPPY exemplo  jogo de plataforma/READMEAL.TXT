MAPPYAL11D

DISCLAIMER:

 This software and associated files are provided 'as is'
with no warranty or guarantees of any kind, you use them
at your own risk and in doing so agree that the author is
in no way liable and cannot be held responsible for any
loss of time/data/hair or anything else that may occur
either directly or indirectly from the use of this
software or associated files.

==============================================
http://www.tilemap.co.uk
http://www.geocities.com/SiliconValley/Vista/7336/robmpy.htm
email: mappy@tilemap.co.uk



Just double click or run buildal.bat to make the examples using DJGPP (DOS)
or buildalm.bat for Mingw32 (DevC++ counts too) (Windows)
(if you are compiling with Visual C you'll have to do it yourself). 
Use cursor keys to scroll on examples and ESC to exit.



C++ notes: MappyAL will work fine with C++, but the file 'mappyal.c'
must be compiled as 'C'. With MSVC there is no problem as it uses
the filename extension (.c/.cpp) to determine how to compile, but
IDE's like DevC++ and VIDE don't do this with C++ projects, so 
instead of adding mappyal.c to your list of sourcefiles (like with 
MSVC) be sure you have mappyal.o by running 'buildal.bat', or 
manually compiling with:

gcc mappyal.c -O2 -Wall -c -o mappyal.o

then adding mappyal.o BEFORE -lalleg in the linking options, with
DevC++ select Project:Project options (Alt+P) and click 'Load object
files' and add mappyal.o, now move the -lalleg text so it is after
the object file, it should look something like:
C:\mapalr11\mappyal.o -lalleg
do not add the file mappyal.c to your sourcefile list

Please report any bugs!





Function reference:
-------------------------------------------------
IMPORTANT: You must have set_gfx_mode BEFORE calling these:

Note: 'mapname' can be any FMP file, or FMP file in an Allegro datafile
such as "MYMAP.FMP" or "mpgame.dat#GAMEMAP"

int MapLoad (char * mapname);

Tries to load the .fmp file specified in mapname, eg.
MapLoad ("MYMAP.FMP");
Returns 0 on success, -1 on failure. You need to call this or MapDecode
before using any of the other map functions. This is the same as the
function MapLoadABM.

int MapLoadVRAM (char * mapname);

Tries to load the .fmp using video bitmaps to take advantage of hardware
acceleration. If it fails, check maperror for the reason. See pbdemo2.c

int MapLoadABM (char * mapname);

Loads the .fmp with the graphics in Allegro memory BITMAPs, you can then
get at the graphics with the abmTiles array, eg. if you want to get at 
the third BITMAP, use abmTiles[2]. All calls to MapDraw functions will
then use the allegro blit and masked_blit functions.

-------------------------------------------------
int MapDecode (unsigned char * mapmempt);

Same as MapLoad, but loads from an area of memory containing an FMP file.
(Useful if your maps are stored compressed, just uncompress to memory and
call MapDecode).

int MapDecodeVRAM (unsigned char * mapmempt);

See MapLoadVRAM

int MapDecodeABM (unsigned char * mapmempt);

See MapLoadABM
-------------------------------------------------
int MapGenerateYLookup (void);

This is optional, call this after MapLoad or MapDecode (or variant) and
it will generate a lookup table that _may_ slightly speed up the functions
MapGetBlock and MapSetBlock, and allows you to use the maparraypt. If you
use this, you _must_ use MapChangeLayer if you want to swap between layers.
Another advantage is you can access block or anim offsets simply with:
maparraypt[y][x];
Where x is the offset from the left in _BLOCKS_, and y is the offset from
the top in _BLOCKS_. Note you can get the actual block structure with the
functions MapGetBlock and MapSetBlock.
The memory allocated is freed when either MapFreeMem is called, or another
map is loaded.

-------------------------------------------------
int MapGetBlockID (int blid, int usernum)

returns the number of the first block that matches 'blid' in the field 
specified with usernum (1 to 7, user1 to user7). If no match, returns -1

-------------------------------------------------
int MapLoadMAR (char * filename, int layer);
int MapDecodeMAR (unsigned char * marmemory, int layer);

Loads a .MAR file into a specified layer. Decode does it from memory, 
you can dealloc that memory afterwards. If you called MapGenerateYLookup,
call it again after this function. Returns -1 on error, 0 on success.

-------------------------------------------------
int MapChangeLayer (int)

This changes to a different map layer, returns -1 if failed, or the new
layer number if success. See mpgame.c

-------------------------------------------------
int MapGetXOffset (int x, int y)
int MapGetYOffset (int x, int y)

These functions are handy for translating a pixel coordinate to block
coordinates, especially for non-rectangular blocks (see mousecrd.c).

-------------------------------------------------
BLKSTR * MapGetBlockInPixels (int x, int y)

Returns a BLKSTR pointer, useful for collision detection and examining a
blockstructure. This is more useful on non-rectangular maps as it is pixel
perfect.

-------------------------------------------------
BLKSTR * MapGetBlock (int x, int y);

Returns a BLKSTR pointer, useful for collision detection and examining a
blockstructure. Note: the x and y paramaters are the offset from the left
and top of the map in _BLOCKS_ NOT pixels. See mpgame.c for an example.

-------------------------------------------------
void MapSetBlockInPixels (int x, int y, int strvalue)

The x and y paramaters are the offset from the left and top of the map
in pixels. If strvalue is positive, the cell is set to that block structure.
If strvalue is negative the cell is set to that anim structure-1 (ie if
you want to put anim 3 in, strvalue would be -4).

-------------------------------------------------
void MapSetBlock (int x, int y, int strvalue);

The x and y paramaters are the offset from the left and top of the map
in _BLOCKS_ NOT pixels. See mpgame.c for an example. If strvalue is
positive, the cell is set to that block structure. If strvalue is
negative the cell is set to that anim structure-1 (ie if you want to
put anim 3 in, strvalue would be -4).

-------------------------------------------------
void MapRestore (void);

Restores the graphics to video bitmaps in hardware rendering.

-------------------------------------------------
void MapCorrectColours (void);

NOTE: This function is now redundant and does nothing. Maps are
colour corrected on loading.
-------------------------------------------------
void MapSetPal8 (void);

Only useful in 8bit (256 colour) mode. Sets the screen palette to the
values contained int the map file.
-------------------------------------------------
void MapFreeMem (void);

When you've had enough of the map, this frees all the memory mappypb
has used.
-------------------------------------------------
void MapInitAnims (void);

Call to reset the animated blocks to their defaults
-------------------------------------------------
void MapUpdateAnims (void);

Animation control. Call from your game loop, moves blocks to next anim
-------------------------------------------------
void MapDrawBG (BITMAP * mapdestpt, int mapxo, int mapyo, int mapx, int mapy,
	int mapw, int maph);

Draws the blocks referenced in bgoff. Will clip the map to the specified area.
Note: the normal clipping on the bitmap is ignored, so trying to draw off the
edges of a bitmap will crash your programme.

BITMAP * mapdestpt  pointer to a MEMORY bitmap to draw on (not screen)
int mapxo           offset, in pixels, from left edge of map, this is
                    the first visible column of pixels you are drawing
int mapyo           offset, in pixels, from top edge of map, this is
                    the first visible row of pixels you are drawing
int mapx            offset, in pixels, from left edge of bitmap
int mapy            offset, in pixels, from top edge of bitmap
int mapw            width of area you want to draw
int maph            height of area you want to draw
-------------------------------------------------
void MapDrawBGT (BITMAP * mapdestpt, int mapxo, int mapyo, int mapx, int mapy,
	int mapw, int maph, int mapfg);

Same as MapDrawBG, except colour 0 pixels are not drawn (8bit), or pink colour
pixels are not drawn (other depths) (transparent).
-------------------------------------------------
void MapDrawFG (BITMAP * mapdestpt, int mapxo, int mapyo, int mapx, int mapy,
	int mapw, int maph, int mapfg);

Same as MapDrawBG, except:

int mapfg           The foreground layer you want to draw, 0, 1, or 2
-------------------------------------------------
void MapDrawRow (BITMAP * mapdestpt, int mapxo, int mapyo, int mapx, int mapy,
	int mapw, int maph, int maprw, void (*cellcall) (int cx, int cy, int dx, int dy))

Same as MapDrawBG, except:

int maprw is the row to draw
cellcall is a callback function for each cell, pass NULL if you are doing your
own depth sorting, or using another system
see isodemo.c for an example of this
-------------------------------------------------
BITMAP * MapMakeParallaxBitmap (BITMAP * sourcebm, int style);

Only on regular rectangular maps.
Pass a bitmap you would like to use for the parallax bitmap. Paramater 2
is where you want the bitmap created, 0 means it is created with
'create_bitmap', 1 means it is created with 'create_video_bitmap'. You
must use this function to make the bitmap you pass to MapDrawParallax.
The source bitmap MUST be a multiple of your block size (ie if you are
using 32*32 blocks, 128*128 and 96*64 are valid, but 100*100 is not).
-------------------------------------------------
void MapDrawParallax (BITMAP * mapdestpt, BITMAP * parbm, int mapxo, int mapyo, int mapx, int mapy,
	int mapw, int maph);

This behaves like MapDrawBG etc, except the 'parbm' bitmap is created
with 'MapMakeParallaxBitmap' and this is tiled in the transparent
regions of the map. After calling this you should call 'MapDrawBGT'
instead of 'MapDrawBG' so the parallax layer isn't overwritten.
This is more efficient than drawing the whole screen as it only draws
in areas not obscured by higher layers, there is minimal overdraw
(pixels aren't overwritten by higher layers where possible).
See the source 'pbdemo3.c' for an example.
-------------------------------------------------


Don't forget to look at the example source code, I've documented them.


Version Changes:
================

MAPPYAL Release11
Slightly improved docs and examples (C++ and datafile notes)
Used good TEST2.FMP map for pbdemo2
11B: MapDecode fixed, new URL
11C: MapLoadVRAM fail when out of VRAM bug fixed
11D: Colours wrong in 32bit screen mode fixed

MAPPYAL Release10
Can draw left/right sides of pillars with MapDrawRow by 
checking 2nd or 3rd boxes in 'others' in MappyWin32

MAPPYAL Release9 (skipped)

MAPPYAL Release8
MapDrawRow function
pageflip fix for pbdemo2.c

MAPPYAL Release7
int MapGetBlockID (int blid, int usernum)
int MapLoadMAR (char * filename, int layer);
int MapDecodeMAR (unsigned char * marmemory, int layer);
(the above functions are untested!)
mapblocksinvidmem, mapblocksinsysmem (num blocks in each)
Added cool proja example

MAPPYAL Release6
This is a new streamlined version, much smaller but with the same
functionality. Also has full support for FMP1.0 and non-regular
maps (like isometric and hexagonal).
WARNING: The array values have changed! NOT compatible with Mappy tutorial 1!
They are now an index (0,1,2,3 etc) before they were byte offset (0,32,64 etc)

MAPPYAL Release5
Changed Win examples to use Allegro rather than WinAlleg
Added define (RBNOCUSTOMRENDER) in mappyal.c to remove a
big code section if you didn't need it

ADDED in release4:
Two parallax functions and a demo to show them (pbdemopx)

FIXED in release3:
MapDecode functions now work
Added MapLoadABM and MapDecodeABM functions (see below)
Added MapGenerateYLookup function
Added MapChangeLayer function
Added MapGetBlock function
Added MapSetBlock function
Modified mpgame.c to use new functions

FIXED in release2:
16bit maps (colours were loaded wrong, oops)
MapDrawFG when using software render
Transparency colour when loading high colour map to 8bit
