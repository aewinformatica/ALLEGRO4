CPP    = g++
RM     = rm -f
OBJS   = main.o \
         token.o \
         token_exception.o \
         tokenreader.o \
         Personagem.o \
         load_exception.o \
         funcs.o

LIBS   = -lalleg
CFLAGS =

.PHONY: Personagem.exe clean clean-after

all: Personagem.exe

clean:
	$(RM) $(OBJS) Personagem.exe

clean-after:
	$(RM) $(OBJS)

Personagem.exe: $(OBJS)
	$(CPP)  -o $@ $(OBJS) $(LIBS)

main.o: main.cpp Personagem.h funcs.h return_exception.h load_exception.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

token.o: token.cpp token.h token_exception.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

token_exception.o: token_exception.cpp token_exception.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

tokenreader.o: tokenreader.cpp token.h token_exception.h tokenreader.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

Personagem.o: Personagem.cpp Personagem.h token.h tokenreader.h token_exception.h load_exception.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

load_exception.o: load_exception.cpp load_exception.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

funcs.o: funcs.cpp funcs.h
	$(CPP)  -c $< -o $@ $(CFLAGS)

