CC     = gcc
WINDRES= windres
RM     = rm -f
OBJS   = 2xsai.o \
         actor.o \
         audio.o \
         boss.o \
         brick.o \
         endofdemo.o \
         enemy.o \
         font.o \
         iconlin.o \
         input.o \
         intro.o \
         item.o \
         launcher.o \
         level.o \
         lexgui.o \
         main.o \
         menu.o \
         osspec.o \
         player.o \
         quest.o \
         questover.o \
         scene.o \
         screenshot.o \
         sprite.o \
         util.o \
         video.o \
         AppResource.res

LIBS   = -mwindows -lalleg_s -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lole32 -ldinput -lddraw -ldxguid -lwinmm -ldsound
CFLAGS = -DALLEGRO_STATICLINK

.PHONY: Open\ Sonic.exe clean clean-after

all: Open\ Sonic.exe

clean:
	$(RM) $(OBJS) Open\ Sonic.exe

clean-after:
	$(RM) $(OBJS)

Open\ Sonic.exe: $(OBJS)
	$(CC)  -o '$@' $(OBJS) $(LIBS)

2xsai.o: 2xsai.c 2xsai.h
	$(CC)  -c $< -o $@ $(CFLAGS)

actor.o: actor.c global.h actor.h video.h input.h level.h
	$(CC)  -c $< -o $@ $(CFLAGS)

audio.o: audio.c audio.h util.h
	$(CC)  -c $< -o $@ $(CFLAGS)

boss.o: boss.c boss.h global.h audio.h actor.h player.h enemy.h input.h item.h brick.h level.h
	$(CC)  -c $< -o $@ $(CFLAGS)

brick.o: brick.c global.h video.h brick.h
	$(CC)  -c $< -o $@ $(CFLAGS)

endofdemo.o: endofdemo.c endofdemo.h scene.h util.h video.h font.h actor.h
	$(CC)  -c $< -o $@ $(CFLAGS)

enemy.o: enemy.c global.h audio.h actor.h player.h enemy.h input.h item.h brick.h level.h
	$(CC)  -c $< -o $@ $(CFLAGS)

font.o: font.c font.h sprite.h video.h
	$(CC)  -c $< -o $@ $(CFLAGS)

iconlin.o: iconlin.c
	$(CC)  -c $< -o $@ $(CFLAGS)

input.o: input.c input.h util.h video.h
	$(CC)  -c $< -o $@ $(CFLAGS)

intro.o: intro.c intro.h scene.h util.h video.h
	$(CC)  -c $< -o $@ $(CFLAGS)

item.o: item.c global.h player.h item.h enemy.h level.h audio.h quest.h
	$(CC)  -c $< -o $@ $(CFLAGS)

launcher.o: launcher.c global.h osspec.h lexgui.h
	$(CC)  -c $< -o $@ $(CFLAGS)

level.o: level.c global.h input.h scene.h video.h audio.h sprite.h util.h brick.h player.h item.h enemy.h level.h font.h quest.h boss.h
	$(CC)  -c $< -o $@ $(CFLAGS)

lexgui.o: lexgui.c lexgui.h osspec.h
	$(CC)  -c $< -o $@ $(CFLAGS)

main.o: main.c util.h scene.h video.h audio.h input.h actor.h brick.h player.h item.h enemy.h level.h quest.h menu.h intro.h endofdemo.h questover.h screenshot.h
	$(CC)  -c $< -o $@ $(CFLAGS)

menu.o: menu.c global.h menu.h util.h scene.h video.h actor.h font.h audio.h quest.h
	$(CC)  -c $< -o $@ $(CFLAGS)

osspec.o: osspec.c global.h osspec.h
	$(CC)  -c $< -o $@ $(CFLAGS)

player.o: player.c global.h player.h audio.h actor.h brick.h enemy.h item.h util.h input.h sprite.h level.h
	$(CC)  -c $< -o $@ $(CFLAGS)

quest.o: quest.c global.h util.h quest.h level.h scene.h
	$(CC)  -c $< -o $@ $(CFLAGS)

questover.o: questover.c questover.h scene.h util.h video.h font.h audio.h quest.h actor.h player.h input.h
	$(CC)  -c $< -o $@ $(CFLAGS)

scene.o: scene.c scene.h util.h
	$(CC)  -c $< -o $@ $(CFLAGS)

screenshot.o: screenshot.c screenshot.h util.h video.h input.h
	$(CC)  -c $< -o $@ $(CFLAGS)

sprite.o: sprite.c global.h sprite.h video.h util.h
	$(CC)  -c $< -o $@ $(CFLAGS)

util.o: util.c util.h video.h sprite.h audio.h
	$(CC)  -c $< -o $@ $(CFLAGS)

video.o: video.c global.h video.h sprite.h util.h brick.h font.h
	$(CC)  -c $< -o $@ $(CFLAGS)

AppResource.res: AppResource.rc
	$(WINDRES) -i AppResource.rc -J rc -o AppResource.res -O coff

