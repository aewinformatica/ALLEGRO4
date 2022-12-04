CPP    = g++
WINDRES= windres
RM     = rm -f
OBJS   = GameCore/VideoSystem.o \
         GameCore/SdlManager.o \
         GameCore/global.o \
         GameCore/SffSpriteManager.o \
         GameCore/SffSpriteList.o \
         GameCore/AirList.o \
         GameCore/AirManager.o \
         GameCore/ImageList.o \
         GameCore/Player.o \
         GameCore/Compiler.o \
         GameCore/yacc.o \
         GameCore/flex.o \
         GameCore/main.o \
         GameCore/SDL_console.o \
         GameCore/internal.o \
         GameCore/DT_drawtext.o \
         GameCore/StateManager.o \
         GameCore/Stack.o \
         GameCore/VM.o \
         GameCore/airyacc.o \
         GameCore/airflex.o \
         GameCore/Game.o \
         GameCore/FightEngine.o \
         GameCore/SDL_framerate.o \
         GameCore/scale2x.o \
         GameCore/scale2xSDL.o \
         GameCore/cnsyacc.o \
         GameCore/cnsflex.o \
         GameCore/ControllerExecuter.o \
         GameCore/Input.o \
         AppResource.res

LIBS   = -mwindows -lmingw32  -lSDLmain -lSDL
CFLAGS =

.PHONY: OpenMugen.exe clean clean-after

all: OpenMugen.exe

clean:
	$(RM) $(OBJS) OpenMugen.exe

clean-after:
	$(RM) $(OBJS)

OpenMugen.exe: $(OBJS)
	$(CPP)  -o $@ $(OBJS) $(LIBS)

GameCore/VideoSystem.o: GameCore/VideoSystem.cpp GameCore/VideoSystem.h GameCore/DT_drawtext.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/SdlManager.o: GameCore/SdlManager.cpp GameCore/SdlManager.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/global.o: GameCore/global.cpp GameCore/global.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/SffSpriteManager.o: GameCore/SffSpriteManager.cpp GameCore/SffSpriteManager.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/SffSpriteList.o: GameCore/SffSpriteList.cpp GameCore/SffSpriteList.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/AirList.o: GameCore/AirList.cpp GameCore/structs.h GameCore/AirList.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/AirManager.o: GameCore/AirManager.cpp GameCore/AirManager.h GameCore/Compiler.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/ImageList.o: GameCore/ImageList.cpp GameCore/Global.h GameCore/ImageList.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/Player.o: GameCore/Player.cpp GameCore/VM.h GameCore/Player.h GameCore/Compiler.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/Compiler.o: GameCore/Compiler.cpp GameCore/Compiler.h GameCore/parse.h GameCore/lex.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/yacc.o: GameCore/yacc.cpp GameCore/lex.h GameCore/Structs.h GameCore/Player.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/flex.o: GameCore/flex.cpp GameCore/token.h GameCore/lex.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/main.o: GameCore/main.cpp GameCore/Game.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/SDL_console.o: GameCore/SDL_console.c GameCore/SDL_console.h GameCore/DT_drawtext.h GameCore/internal.h GameCore/SDL_image.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/internal.o: GameCore/internal.c GameCore/SDL.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/DT_drawtext.o: GameCore/DT_drawtext.c GameCore/SDL.h GameCore/DT_drawtext.h GameCore/internal.h GameCore/SDL_image.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/StateManager.o: GameCore/StateManager.cpp GameCore/StateManager.h GameCore/global.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/Stack.o: GameCore/Stack.cpp GameCore/stack.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/VM.o: GameCore/VM.cpp GameCore/Player.h GameCore/VM.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/airyacc.o: GameCore/airyacc.cpp GameCore/lex.h GameCore/Structs.h GameCore/Player.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/airflex.o: GameCore/airflex.cpp GameCore/airtoken.h GameCore/lex.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/Game.o: GameCore/Game.cpp GameCore/Game.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/FightEngine.o: GameCore/FightEngine.cpp GameCore/FightEngine.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/SDL_framerate.o: GameCore/SDL_framerate.c GameCore/SDL_framerate.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/scale2x.o: GameCore/scale2x.c GameCore/scale2x.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/scale2xSDL.o: GameCore/scale2xSDL.c GameCore/global.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/cnsyacc.o: GameCore/cnsyacc.cpp GameCore/lex.h GameCore/Structs.h GameCore/Player.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/cnsflex.o: GameCore/cnsflex.cpp GameCore/airtoken.h GameCore/lex.h GameCore/cnstoken.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/ControllerExecuter.o: GameCore/ControllerExecuter.cpp GameCore/ControllerExecuter.h GameCore/Player.h GameCore/token.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

GameCore/Input.o: GameCore/Input.cpp GameCore/Input.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

AppResource.res: AppResource.rc
	$(WINDRES) -i AppResource.rc -J rc -o AppResource.res -O coff

